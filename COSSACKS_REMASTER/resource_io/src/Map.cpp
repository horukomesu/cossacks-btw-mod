#include "resource_io/Map.hpp"
#include "resource_io/ResourceIO.hpp"
#include "resource_io/TexturesMeta.hpp"
#include "resource_io/ColorPalette.hpp"

#include <cstdint>
#include <cstring>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <fstream>

namespace resource_io::map {

namespace {
// Read little-endian 16/32-bit from byte span
static inline uint16_t rd_u16(const uint8_t* p) {
    return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}
static inline uint32_t rd_u32(const uint8_t* p) {
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

// Scan chunked PMD3 stream and return vector of pointers to chunks (id, size, payload)
struct Chunk { uint32_t id; const uint8_t* data; uint32_t size; };

static void parse_chunks(const std::vector<uint8_t>& blob, size_t startOffset, std::vector<Chunk>& out) {
    out.clear();
    size_t pos = startOffset;
    while (pos + 8u <= blob.size()) {
        uint32_t id = rd_u32(blob.data() + pos);
        uint32_t sz = rd_u32(blob.data() + pos + 4);
        pos += 8u;
        // The original format stores chunk size as (4 + payloadBytes)
        if (sz < 4u) break;
        uint32_t payload = sz - 4u;
        if (pos + payload > blob.size()) break;
        out.push_back(Chunk{ id, blob.data() + pos, payload });
        pos += payload;
        if (id == 'MDNE') break; // End marker per original
    }
}

} // namespace

bool load_m3d(const std::string& name, MapData& out) {
    std::vector<unsigned char> data;
    if (!resource_io::read_file_anywhere(name, data)) return false;
    if (data.size() < 12u) return false;

    const uint8_t* p = data.data();
    const uint32_t sig = rd_u32(p + 0);
    // Signature is 'PMD3' + ADDSH - 1, where ADDSH ∈ [1..3]
    int addShift = (int)(sig - 'PMD3') + 1;
    if (addShift < 1 || addShift > 3) return false;

    // Header has VertInLine and MaxTH but the original recomputes from addShift too; we’ll recompute and trust chunks
    const uint32_t vertInLineHeader = rd_u32(p + 4);
    const uint32_t maxTHHeader = rd_u32(p + 8);
    (void)vertInLineHeader; (void)maxTHHeader;

    // Recompute like SetupArrays: MaxSector = 128 << ADDSH; MaxTH = MaxSector*2; VertInLine = MaxSector*2 + 1
    const int maxSector = 128 << addShift;
    const int maxTH = maxSector * 2;
    const int vertInLine = maxTH + 1;

    // map size in tiles (msx, msy) = 240 << ADDSH
    const int mapTiles = 240 << addShift;

    // Prepare output buffers
    out.addShift = addShift;
    out.mapTilesX = mapTiles;
    out.mapTilesY = mapTiles;
    out.maxTH = maxTH;
    out.vertInLine = vertInLine;
    out.heights.assign(static_cast<size_t>(vertInLine) * static_cast<size_t>(maxTH), 0);
    out.texIndices.assign(static_cast<size_t>(vertInLine) * static_cast<size_t>(maxTH), 0);

    // Parse chunks after 12-byte header
    std::vector<Chunk> chunks;
    parse_chunks(data, 12u, chunks);
    out.chunks.clear();
    out.chunks.reserve(chunks.size());

    // Known IDs from original: 'FRUS' (surface heights), 'ELIT' (tiles), 'TCES' (sections), '1VIR' (rivers), etc.
    for (const Chunk& c : chunks) {
        out.chunks.push_back({ c.id, c.size });
        if (c.id == 'FRUS') {
            // Heights: little-endian int16_t array of (maxTH+1)*maxTH
            const size_t need = static_cast<size_t>(vertInLine) * static_cast<size_t>(maxTH) * sizeof(int16_t);
            if (c.size >= need) {
                std::memcpy(out.heights.data(), c.data, need);
            }
        } else if (c.id == 'ELIT') {
            // Texture indices: bytes array of same length as heights elements
            const size_t need = static_cast<size_t>(vertInLine) * static_cast<size_t>(maxTH);
            if (c.size >= need) {
                std::memcpy(out.texIndices.data(), c.data, need);
            }
        } else if (c.id == '2AES') {
            // Water cost: Lx, Ly then rows of Deep and Bright with stride MaxWX (= MAPSX>>1). Store compact copy Lx*Ly for both
            if (c.size >= 8u) {
                const int Lx = (int)rd_u32(c.data + 0);
                const int Ly = (int)rd_u32(c.data + 4);
                const size_t rowsz = static_cast<size_t>(Lx);
                out.waterDeep.resize(static_cast<size_t>(Lx) * static_cast<size_t>(Ly));
                out.waterBright.resize(static_cast<size_t>(Lx) * static_cast<size_t>(Ly));
                out.waterLx = Lx; out.waterLy = Ly;
                size_t pos = 8u;
                for (int y = 0; y < Ly; ++y) {
                    if (pos + rowsz > c.size) break;
                    std::memcpy(out.waterDeep.data() + static_cast<size_t>(y) * rowsz, c.data + pos, rowsz);
                    pos += rowsz;
                    if (pos + rowsz > c.size) break;
                    std::memcpy(out.waterBright.data() + static_cast<size_t>(y) * rowsz, c.data + pos, rowsz);
                    pos += rowsz;
                }
            }
        } else if (c.id == '1POT') {
            // Land topology (raw blob after size): store raw; decoding will be added when needed
            out.topLand.assign(c.data, c.data + c.size);
        } else if (c.id == 'WPOT') {
            // Water topology (raw blob)
            out.topWater.assign(c.data, c.data + c.size);
        }
        // Other chunks ignored for now; they can be added later one-to-one
    }

    // Generate simple minimap (msx/2 x msy/2) using TexMap colors and nation palette for visibility
    const int miniW = out.mapTilesX / 2;
    const int miniH = out.mapTilesY / 2;
    out.minimapRGBA.assign(static_cast<size_t>(miniW) * static_cast<size_t>(miniH) * 4u, 0);
    const uint8_t* tileMap = resource_io::terrain_meta::get_tile_map_16x16();
    const uint8_t* pal = resource_io::color_palette::get_rgba256x4();
    for (int y = 0; y < miniH; ++y) {
        for (int x = 0; x < miniW; ++x) {
            const int vx = x * 2; const int vy = y * 2;
            const size_t vidx = static_cast<size_t>(vy) * static_cast<size_t>(vertInLine) + static_cast<size_t>(vx);
            const uint8_t t = out.texIndices[vidx];
            const uint8_t colorIdx = static_cast<uint8_t>(tileMap[t]);
            const uint8_t* rgba = pal + static_cast<size_t>(colorIdx) * 4u;
            const size_t di = (static_cast<size_t>(y) * static_cast<size_t>(miniW) + static_cast<size_t>(x)) * 4u;
            out.minimapRGBA[di + 0] = rgba[0];
            out.minimapRGBA[di + 1] = rgba[1];
            out.minimapRGBA[di + 2] = rgba[2];
            out.minimapRGBA[di + 3] = 255u;
        }
    }
    // Precompute simple per-vertex light
    out.vertexLight.assign(static_cast<size_t>(vertInLine) * static_cast<size_t>(maxTH), 255);
    auto getH = [&](int x, int y) -> int {
        if (x < 0) x = 0; if (y < 0) y = 0;
        if (x >= vertInLine) x = vertInLine - 1; if (y >= maxTH) y = maxTH - 1;
        return static_cast<int>(out.heights[static_cast<size_t>(y) * static_cast<size_t>(vertInLine) + static_cast<size_t>(x)]);
    };
    for (int vy = 0; vy < maxTH; ++vy) {
        for (int vx = 0; vx < vertInLine; ++vx) {
            const int dy = getH(vx + 1, vy) - getH(vx - 1, vy);
            const int dx = getH(vx, vy + 1) - getH(vx, vy - 1);
            const float len = std::sqrt(dx * dx + dy * dy + 64.0f * 64.0f);
            float lig = 16.0f + (64.0f * 30.0f / (len + 1e-5f)) / 16.0f;
            if (lig < 2.0f) lig = 2.0f; if (lig > 31.0f) lig = 31.0f;
            const float k = (32.0f - lig) / 32.0f;
            out.vertexLight[static_cast<size_t>(vy) * static_cast<size_t>(vertInLine) + static_cast<size_t>(vx)] = static_cast<uint8_t>(std::clamp(k * 255.0f, 0.0f, 255.0f));
        }
    }
    return true;
}

void log_map_summary(const MapData& md) {
    // Log header
    std::cout << "[map] ADDSH=" << md.addShift
              << " tiles=" << md.mapTilesX << "x" << md.mapTilesY
              << " vertInLine=" << md.vertInLine << " maxTH=" << md.maxTH << std::endl;
    // Chunks present
    std::cout << "[map] chunks:";
    for (const auto& ch : md.chunks) {
        char id[5] = {0,0,0,0,0};
        id[0] = static_cast<char>(ch.id & 0xFF);
        id[1] = static_cast<char>((ch.id >> 8) & 0xFF);
        id[2] = static_cast<char>((ch.id >> 16) & 0xFF);
        id[3] = static_cast<char>((ch.id >> 24) & 0xFF);
        std::cout << ' ' << id << '(' << ch.size << ')';
    }
    std::cout << std::endl;
    // Heights/tiles
    std::cout << "[map] heights=" << md.heights.size() << " int16, texIndices=" << md.texIndices.size() << " bytes" << std::endl;
    if (md.waterLx > 0 && md.waterLy > 0) {
        std::cout << "[map] water cost Lx=" << md.waterLx << " Ly=" << md.waterLy << std::endl;
    }
    // Texture meta information (tile usage categories)
    const uint8_t* tileMap = resource_io::terrain_meta::get_tile_map_16x16();
    if (tileMap) {
        // Count usage per 256 atlas indices present in texIndices
        std::vector<int> counts(256, 0);
        for (uint8_t t : md.texIndices) counts[t]++;
        int nonZero = 0; for (int c : counts) if (c) nonZero++;
        std::cout << "[map] distinct tile indices used=" << nonZero << "/256" << std::endl;
        // List top few tiles
        std::vector<std::pair<int,int>> pairs;
        for (int i = 0; i < 256; ++i) if (counts[i]) pairs.emplace_back(counts[i], i);
        std::sort(pairs.begin(), pairs.end(), [](auto& a, auto& b){ return a.first > b.first; });
        const int topN = std::min<int>(8, static_cast<int>(pairs.size()));
        if (topN > 0) {
            std::cout << "[map] top tiles:";
            for (int k = 0; k < topN; ++k) {
                const int idx = pairs[k].second;
                const int cnt = pairs[k].first;
                const int cat = tileMap[idx];
                std::cout << " #" << idx << "(count=" << cnt << ", cat=" << cat << ")";
            }
            std::cout << std::endl;
        }
    }
}

// -------------------------- Full scene loader/writer --------------------------
namespace {
static inline void wr_u32(std::vector<uint8_t>& buf, uint32_t v) {
    buf.push_back(static_cast<uint8_t>(v & 0xFF));
    buf.push_back(static_cast<uint8_t>((v >> 8) & 0xFF));
    buf.push_back(static_cast<uint8_t>((v >> 16) & 0xFF));
    buf.push_back(static_cast<uint8_t>((v >> 24) & 0xFF));
}
static inline void wr_u16(std::vector<uint8_t>& buf, uint16_t v) {
    buf.push_back(static_cast<uint8_t>(v & 0xFF));
    buf.push_back(static_cast<uint8_t>((v >> 8) & 0xFF));
}
static inline void wr_bytes(std::vector<uint8_t>& buf, const void* p, size_t n) {
    const uint8_t* b = static_cast<const uint8_t*>(p);
    buf.insert(buf.end(), b, b + n);
}
static inline void compute_header_from_addshift(int addShift, int& outMaxSector, int& outMaxTH, int& outVertInLine, int& outMapTiles) {
    outMaxSector = 128 << addShift;
    outMaxTH = outMaxSector * 2;
    outVertInLine = outMaxTH + 1;
    outMapTiles = 240 << addShift;
}
} // namespace

bool load_m3d_scene(const std::string& name, MapSceneData& out) {
    std::vector<unsigned char> data;
    if (!resource_io::read_file_anywhere(name, data)) return false;
    if (data.size() < 12u) return false;

    const uint8_t* p = data.data();
    const uint32_t sig = rd_u32(p + 0);
    const int addShift = (int)(sig - 'PMD3') + 1;
    if (addShift < 1 || addShift > 3) return false;
    (void)rd_u32(p + 4); (void)rd_u32(p + 8);

    int maxSector, maxTH, vertInLine, mapTiles;
    compute_header_from_addshift(addShift, maxSector, maxTH, vertInLine, mapTiles);

    out.addShift = addShift;
    out.maxSector = maxSector;
    out.maxTH = maxTH;
    out.vertInLine = vertInLine;
    out.mapTilesX = mapTiles;
    out.mapTilesY = mapTiles;

    std::vector<Chunk> chunks;
    parse_chunks(data, 12u, chunks);
    out.chunks.clear();
    out.chunks.reserve(chunks.size());

    for (const Chunk& c : chunks) {
        out.chunks.push_back({ c.id, c.size });
        switch (c.id) {
            case 'WEIV': {
                if (c.size >= 292u * 190u) {
                    out.previewWEIV.assign(c.data, c.data + 292u * 190u);
                }
                break;
            }
            case 'FRUS': {
                const size_t need = static_cast<size_t>(vertInLine) * static_cast<size_t>(maxTH) * sizeof(int16_t);
                if (c.size >= need) {
                    out.heights.resize(need / sizeof(int16_t));
                    std::memcpy(out.heights.data(), c.data, need);
                }
                break;
            }
            case 'ELIT': {
                const size_t need = static_cast<size_t>(vertInLine) * static_cast<size_t>(maxTH);
                if (c.size >= need) {
                    out.texIndices.resize(need);
                    std::memcpy(out.texIndices.data(), c.data, need);
                }
                break;
            }
            case 'TCES': {
                const size_t need = static_cast<size_t>(maxSector) * static_cast<size_t>(maxTH) * 6u;
                if (c.size >= need) {
                    out.sectMap.resize(need);
                    std::memcpy(out.sectMap.data(), c.data, need);
                }
                break;
            }
            case 'EERT': {
                if (c.size >= 4u) {
                    const uint8_t* q = c.data;
                    uint32_t ns = rd_u32(q); q += 4;
                    out.decorSprites.clear();
                    out.decorSprites.reserve(ns);
                    for (uint32_t i = 0; i < ns; ++i) {
                        if ((size_t)(c.data + c.size - q) < 12u) break;
                        MapSceneData::DecorSprite ds{};
                        ds.sign = rd_u16(q); q += 2;
                        ds.x = (int32_t)rd_u32(q); q += 4;
                        ds.y = (int32_t)rd_u32(q); q += 4;
                        ds.sgIndex = rd_u16(q); q += 2;
                        out.decorSprites.push_back(ds);
                    }
                }
                break;
            }
            case 'TINU': {
                if (c.size >= 4u) {
                    const uint8_t* q = c.data;
                    uint32_t nu = rd_u32(q); q += 4;
                    out.unitsV1.clear(); out.unitsV1.reserve(nu);
                    for (uint32_t i = 0; i < nu; ++i) {
                        if ((size_t)(c.data + c.size - q) < 48u) break;
                        MapSceneData::UnitV1 u{};
                        u.NI = q[0]; q += 1;
                        u.NIndex = rd_u16(q); q += 2;
                        u.x = (int32_t)rd_u32(q); q += 4;
                        u.y = (int32_t)rd_u32(q); q += 4;
                        u.life = rd_u16(q); q += 2;
                        u.stage = rd_u16(q); q += 2;
                        std::memcpy(u.name, q, 33); q += 33;
                        out.unitsV1.push_back(u);
                    }
                }
                break;
            }
            case '2INU': {
                if (c.size >= 4u) {
                    const uint8_t* q = c.data;
                    uint32_t nu = rd_u32(q); q += 4;
                    out.unitsV2.clear(); out.unitsV2.reserve(nu);
                    for (uint32_t i = 0; i < nu; ++i) {
                        if ((size_t)(c.data + c.size - q) < 52u) break;
                        MapSceneData::UnitV2 u{};
                        u.NI = q[0]; q += 1;
                        u.NIndex = rd_u16(q); q += 2;
                        u.x = (int32_t)rd_u32(q); q += 4;
                        u.y = (int32_t)rd_u32(q); q += 4;
                        u.life = rd_u16(q); q += 2;
                        u.stage = rd_u16(q); q += 2;
                        u.wx = (int16_t)rd_u16(q); q += 2;
                        u.wy = (int16_t)rd_u16(q); q += 2;
                        std::memcpy(u.name, q, 33); q += 33;
                        out.unitsV2.push_back(u);
                    }
                }
                break;
            }
            case '3INU': {
                if (c.size >= 4u) {
                    const uint8_t* q = c.data;
                    uint32_t nu = rd_u32(q); q += 4;
                    out.unitsV3.clear(); out.unitsV3.reserve(nu);
                    for (uint32_t i = 0; i < nu; ++i) {
                        if ((size_t)(c.data + c.size - q) < 54u) break;
                        MapSceneData::UnitV3 u{};
                        u.NI = q[0]; q += 1;
                        u.NIndex = rd_u16(q); q += 2;
                        u.x = (int32_t)rd_u32(q); q += 4;
                        u.y = (int32_t)rd_u32(q); q += 4;
                        u.life = rd_u16(q); q += 2;
                        u.stage = rd_u16(q); q += 2;
                        u.wx = (int16_t)rd_u16(q); q += 2;
                        u.wy = (int16_t)rd_u16(q); q += 2;
                        u.dir = *q++; u.opt = *q++;
                        std::memcpy(u.name, q, 33); q += 33;
                        out.unitsV3.push_back(u);
                    }
                }
                break;
            }
            case 'LLAW':
            case '1LAW':
            case '2LAW': {
                const uint8_t* q = c.data;
                if (c.size < 4u) break;
                uint32_t nClusters = rd_u32(q); q += 4;
                if (c.id == 'LLAW') out.wallsV0.clear();
                if (c.id == '1LAW') out.wallsV1.clear();
                if (c.id == '2LAW') out.wallsV2.clear();
                for (uint32_t i = 0; i < nClusters; ++i) {
                    if ((size_t)(c.data + c.size - q) < (1u + 4u + 4u)) break;
                    MapSceneData::WallClusterHeader hdr{ q[0], rd_u32(q + 1), rd_u32(q + 5) };
                    q += 1 + 4 + 4;
                    if (c.id == 'LLAW') {
                        MapSceneData::WallClusterV0 cl{}; cl.hdr = hdr; cl.cells.resize(hdr.nCells);
                        for (uint32_t j = 0; j < hdr.nCells; ++j) {
                            if ((size_t)(c.data + c.size - q) < 2u+2u+1u+1u+1u+1u+2u+2u+1u+1u+2u) break;
                            MapSceneData::WallCellV0 wc{};
                            wc.x = (int16_t)rd_u16(q); q += 2;
                            wc.y = (int16_t)rd_u16(q); q += 2;
                            wc.type = *q++;
                            wc.NI = *q++;
                            wc.stage = *q++;
                            wc.maxStage = *q++;
                            wc.health = rd_u16(q); q += 2;
                            wc.maxHealth = rd_u16(q); q += 2;
                            wc.sprite = *q++;
                            wc.sprBase = *q++;
                            wc.clusterIndex = rd_u16(q); q += 2;
                            cl.cells[j] = wc;
                        }
                        out.wallsV0.push_back(std::move(cl));
                    } else if (c.id == '1LAW') {
                        MapSceneData::WallClusterV1 cl{}; cl.hdr = hdr; cl.cells.resize(hdr.nCells);
                        for (uint32_t j = 0; j < hdr.nCells; ++j) {
                            if ((size_t)(c.data + c.size - q) < 2u+2u+1u+1u+1u+1u+2u+2u+1u+1u+2u+1u) break;
                            MapSceneData::WallCellV1 wc{};
                            wc.x = (int16_t)rd_u16(q); q += 2;
                            wc.y = (int16_t)rd_u16(q); q += 2;
                            wc.type = *q++;
                            wc.NI = *q++;
                            wc.stage = *q++;
                            wc.maxStage = *q++;
                            wc.health = rd_u16(q); q += 2;
                            wc.maxHealth = rd_u16(q); q += 2;
                            wc.sprite = *q++;
                            wc.sprBase = *q++;
                            wc.clusterIndex = rd_u16(q); q += 2;
                            wc.visible = *q++;
                            cl.cells[j] = wc;
                        }
                        out.wallsV1.push_back(std::move(cl));
                    } else { // '2LAW'
                        MapSceneData::WallClusterV2 cl{}; cl.hdr = hdr; cl.cells.resize(hdr.nCells);
                        for (uint32_t j = 0; j < hdr.nCells; ++j) {
                            if ((size_t)(c.data + c.size - q) < (2u+2u+1u+1u+1u+1u+2u+2u+1u+1u+2u+1u + 2u+4u+1u+2u)) break;
                            MapSceneData::WallCellV2 wc{};
                            wc.x = (int16_t)rd_u16(q); q += 2;
                            wc.y = (int16_t)rd_u16(q); q += 2;
                            wc.type = *q++;
                            wc.NI = *q++;
                            wc.stage = *q++;
                            wc.maxStage = *q++;
                            wc.health = rd_u16(q); q += 2;
                            wc.maxHealth = rd_u16(q); q += 2;
                            wc.sprite = *q++;
                            wc.sprBase = *q++;
                            wc.clusterIndex = rd_u16(q); q += 2;
                            wc.visible = *q++;
                            wc.gateIndex = rd_u16(q); q += 2;
                            wc.locks = rd_u32(q); q += 4;
                            wc.dirMask = *q++;
                            wc.oIndex = rd_u16(q); q += 2;
                            cl.cells[j] = wc;
                        }
                        out.wallsV2.push_back(std::move(cl));
                    }
                }
                break;
            }
            case '1COL': {
                if (c.size >= 8u) {
                    const uint8_t* q = c.data;
                    uint32_t nLock = rd_u32(q); q += 4;
                    uint32_t nUnlock = rd_u32(q); q += 4;
                    out.lockBars.resize(nLock);
                    out.unlockBars.resize(nUnlock);
                    for (uint32_t i = 0; i < nLock; ++i) {
                        if ((size_t)(c.data + c.size - q) < 4u) break;
                        MapSceneData::BlockCell bc{}; bc.x = rd_u16(q); bc.y = rd_u16(q+2); q += 4; out.lockBars[i] = bc;
                    }
                    for (uint32_t i = 0; i < nUnlock; ++i) {
                        if ((size_t)(c.data + c.size - q) < 4u) break;
                        MapSceneData::BlockCell bc{}; bc.x = rd_u16(q); bc.y = rd_u16(q+2); q += 4; out.unlockBars[i] = bc;
                    }
                }
                break;
            }
            case '2AES': {
                if (c.size >= 8u) {
                    const uint8_t* q = c.data;
                    int Lx = (int)rd_u32(q); q += 4;
                    int Ly = (int)rd_u32(q); q += 4;
                    out.waterLx = Lx; out.waterLy = Ly;
                    out.waterDeep.assign(static_cast<size_t>(Lx) * static_cast<size_t>(Ly), 0);
                    out.waterBright.assign(static_cast<size_t>(Lx) * static_cast<size_t>(Ly), 0);
                    for (int y = 0; y < Ly; ++y) {
                        size_t rowsz = static_cast<size_t>(Lx);
                        if ((size_t)(c.data + c.size - q) < rowsz) break;
                        std::memcpy(out.waterDeep.data() + static_cast<size_t>(y) * rowsz, q, rowsz); q += rowsz;
                        if ((size_t)(c.data + c.size - q) < rowsz) break;
                        std::memcpy(out.waterBright.data() + static_cast<size_t>(y) * rowsz, q, rowsz); q += rowsz;
                    }
                }
                break;
            }
            case 'USER': {
                if (c.size >= sizeof(int32_t) * 8u * 8u) {
                    const uint8_t* q = c.data;
                    for (int i = 0; i < 8; ++i) {
                        for (int j = 0; j < 8; ++j) {
                            out.RES8x8[i][j] = (int32_t)rd_u32(q); q += 4;
                        }
                    }
                }
                break;
            }
            case '1POT': {
                out.landTopoRaw.assign(c.data, c.data + c.size);
                const uint8_t* q = c.data;
                if (c.size < 4u) break;
                uint32_t nAreas = rd_u32(q); q += 4;
                out.landTopo.areas.clear(); out.landTopo.areas.reserve(nAreas);
                for (uint32_t i = 0; i < nAreas; ++i) {
                    if ((size_t)(c.data + c.size - q) < sizeof(MapSceneData::AreaDisk32)) break;
                    MapSceneData::AreaDisk32 ad{}; std::memcpy(&ad, q, sizeof(MapSceneData::AreaDisk32)); q += sizeof(MapSceneData::AreaDisk32);
                    MapSceneData::AreaRuntime ar{}; ar.x = ad.x; ar.y = ad.y; ar.importance = ad.importance; ar.nTrees = ad.nTrees; ar.nStones = ad.nStones;
                    if (ad.nMines) {
                        ar.minesIdx.resize(ad.nMines);
                        size_t bytes = static_cast<size_t>(ad.nMines) * 2u;
                        if ((size_t)(c.data + c.size - q) < bytes) break;
                        std::memcpy(ar.minesIdx.data(), q, bytes); q += bytes;
                    }
                    if (ad.nLinks) {
                        ar.linksPairs.resize(static_cast<size_t>(ad.nLinks) * 2u);
                        size_t bytes = static_cast<size_t>(ad.nLinks) * 4u;
                        if ((size_t)(c.data + c.size - q) < bytes) break;
                        std::memcpy(ar.linksPairs.data(), q, bytes); q += bytes;
                    }
                    out.landTopo.areas.push_back(std::move(ar));
                }
                {
                    size_t nA = out.landTopo.areas.size();
                    size_t linksBytes = nA * nA * 2u;
                    if ((size_t)(c.data + c.size - q) >= linksBytes) {
                        out.landTopo.motionLinks.resize(nA * nA);
                        std::memcpy(out.landTopo.motionLinks.data(), q, linksBytes); q += linksBytes;
                    }
                    if ((size_t)(c.data + c.size - q) >= linksBytes) {
                        out.landTopo.linksDist.resize(nA * nA);
                        std::memcpy(out.landTopo.linksDist.data(), q, linksBytes); q += linksBytes;
                    }
                    size_t topRefBytes = static_cast<size_t>(out.mapTilesX >> 2) * static_cast<size_t>(out.mapTilesY >> 2) * 2u;
                    if ((size_t)(c.data + c.size - q) >= topRefBytes) {
                        out.landTopo.topRef.resize(topRefBytes / 2u);
                        std::memcpy(out.landTopo.topRef.data(), q, topRefBytes); q += topRefBytes;
                    }
                }
                break;
            }
            case 'WPOT': {
                out.waterTopoRaw.assign(c.data, c.data + c.size);
                const uint8_t* q = c.data;
                if (c.size < 4u) break;
                uint32_t nAreas = rd_u32(q); q += 4;
                out.waterTopo.areas.clear(); out.waterTopo.areas.reserve(nAreas);
                for (uint32_t i = 0; i < nAreas; ++i) {
                    if ((size_t)(c.data + c.size - q) < sizeof(MapSceneData::AreaDisk32)) break;
                    MapSceneData::AreaDisk32 ad{}; std::memcpy(&ad, q, sizeof(MapSceneData::AreaDisk32)); q += sizeof(MapSceneData::AreaDisk32);
                    MapSceneData::AreaRuntime ar{}; ar.x = ad.x; ar.y = ad.y; ar.importance = ad.importance; ar.nTrees = ad.nTrees; ar.nStones = ad.nStones;
                    if (ad.nMines) {
                        ar.minesIdx.resize(ad.nMines);
                        size_t bytes = static_cast<size_t>(ad.nMines) * 2u;
                        if ((size_t)(c.data + c.size - q) < bytes) break;
                        std::memcpy(ar.minesIdx.data(), q, bytes); q += bytes;
                    }
                    if (ad.nLinks) {
                        ar.linksPairs.resize(static_cast<size_t>(ad.nLinks) * 2u);
                        size_t bytes = static_cast<size_t>(ad.nLinks) * 4u;
                        if ((size_t)(c.data + c.size - q) < bytes) break;
                        std::memcpy(ar.linksPairs.data(), q, bytes); q += bytes;
                    }
                    out.waterTopo.areas.push_back(std::move(ar));
                }
                {
                    size_t nA = out.waterTopo.areas.size();
                    size_t linksBytes = nA * nA * 2u;
                    if ((size_t)(c.data + c.size - q) >= linksBytes) {
                        out.waterTopo.motionLinks.resize(nA * nA);
                        std::memcpy(out.waterTopo.motionLinks.data(), q, linksBytes); q += linksBytes;
                    }
                    if ((size_t)(c.data + c.size - q) >= linksBytes) {
                        out.waterTopo.linksDist.resize(nA * nA);
                        std::memcpy(out.waterTopo.linksDist.data(), q, linksBytes); q += linksBytes;
                    }
                    size_t topRefBytes = static_cast<size_t>(out.mapTilesX >> 2) * static_cast<size_t>(out.mapTilesY >> 2) * 2u;
                    if ((size_t)(c.data + c.size - q) >= topRefBytes) {
                        out.waterTopo.topRef.resize(topRefBytes / 2u);
                        std::memcpy(out.waterTopo.topRef.data(), q, topRefBytes); q += topRefBytes;
                    }
                }
                break;
            }
            case '1TAG': {
                const uint8_t* q = c.data;
                if (c.size < 8u) break;
                out.nGates = (int)rd_u32(q); q += 4;
                out.maxGates = (int)rd_u32(q); q += 4;
                out.gates.clear();
                out.gates.reserve(out.nGates);
                for (int i = 0; i < out.nGates; ++i) {
                    if ((size_t)(c.data + c.size - q) < 10u) break;
                    MapSceneData::Gate g{}; g.x = (int16_t)rd_u16(q); q += 2; g.y = (int16_t)rd_u16(q); q += 2;
                    g.NI = *q++; g.nMask = *q++; g.state = *q++; g.delay = *q++; g.locked = *q++; g.charId = *q++;
                    out.gates.push_back(g);
                }
                break;
            }
            case '1NOZ': { out.zonesGroupsRaw.assign(c.data, c.data + c.size); break; }
            case 'MROF':
            case '1ROF': { out.formationsRaw.assign(c.data, c.data + c.size); break; }
            case 'LLD.': { if (c.size >= 200u) std::memcpy(out.mapScenaryDLL, c.data, 200); break; }
            case 'AIIA': { if (c.size >= 12u) { const uint8_t* q = c.data; out.RM_LandType = (int32_t)rd_u32(q); q += 4; out.RM_Resstart = (int32_t)rd_u32(q); q += 4; out.RM_Restot = (int32_t)rd_u32(q); } break; }
            case 'SMSP': { out.smsRaw.assign(c.data, c.data + c.size); break; }
            case 'PEAC': { out.peacetimeRaw.assign(c.data, c.data + c.size); break; }
            case '1VIR': { size_t half = c.size / 2u; out.riversDir.assign(c.data, c.data + half); out.riversVol.assign(c.data + half, c.data + c.size); break; }
            default: break;
        }
    }

    return true;
}

bool save_m3d_scene(const std::string& name, const MapSceneData& s) {
    std::vector<uint8_t> buf;
    // Header
    wr_u32(buf, (uint32_t)('PMD3' + (s.addShift - 1)));
    int maxSector, maxTH, vertInLine, mapTiles;
    compute_header_from_addshift(s.addShift, maxSector, maxTH, vertInLine, mapTiles);
    wr_u32(buf, (uint32_t)vertInLine);
    wr_u32(buf, (uint32_t)maxTH);

    auto write_chunk = [&](uint32_t id, const std::vector<uint8_t>& payload){
        wr_u32(buf, id);
        wr_u32(buf, (uint32_t)(4u + payload.size()));
        if (!payload.empty()) wr_bytes(buf, payload.data(), payload.size());
    };

    // WEIV
    if (s.previewWEIV.size() == 292u * 190u) { std::vector<uint8_t> pl = s.previewWEIV; write_chunk('WEIV', pl); }

    // FRUS
    if (s.heights.size() == (size_t)vertInLine * (size_t)maxTH) {
        std::vector<uint8_t> pl; pl.resize(s.heights.size() * sizeof(int16_t));
        std::memcpy(pl.data(), s.heights.data(), pl.size());
        write_chunk('FRUS', pl);
    }
    // ELIT
    if (s.texIndices.size() == (size_t)vertInLine * (size_t)maxTH) { write_chunk('ELIT', s.texIndices); }
    // TCES
    if (s.sectMap.size() == (size_t)maxSector * (size_t)maxTH * 6u) { write_chunk('TCES', s.sectMap); }
    // EERT
    if (!s.decorSprites.empty()) {
        std::vector<uint8_t> pl; pl.reserve(4 + s.decorSprites.size() * 12);
        wr_u32(pl, (uint32_t)s.decorSprites.size());
        for (const auto& ds : s.decorSprites) { wr_u16(pl, ds.sign); wr_u32(pl, (uint32_t)ds.x); wr_u32(pl, (uint32_t)ds.y); wr_u16(pl, ds.sgIndex); }
        write_chunk('EERT', pl);
    }
    // Units: prefer V3 -> V2 -> V1
    if (!s.unitsV3.empty()) {
        std::vector<uint8_t> pl; wr_u32(pl, (uint32_t)s.unitsV3.size());
        for (const auto& u : s.unitsV3) {
            pl.push_back(u.NI); wr_u16(pl, u.NIndex); wr_u32(pl, (uint32_t)u.x); wr_u32(pl, (uint32_t)u.y); wr_u16(pl, u.life); wr_u16(pl, u.stage); wr_u16(pl, (uint16_t)u.wx); wr_u16(pl, (uint16_t)u.wy); pl.push_back(u.dir); pl.push_back(u.opt); wr_bytes(pl, u.name, 33);
        } write_chunk('3INU', pl);
    } else if (!s.unitsV2.empty()) {
        std::vector<uint8_t> pl; wr_u32(pl, (uint32_t)s.unitsV2.size());
        for (const auto& u : s.unitsV2) { pl.push_back(u.NI); wr_u16(pl, u.NIndex); wr_u32(pl, (uint32_t)u.x); wr_u32(pl, (uint32_t)u.y); wr_u16(pl, u.life); wr_u16(pl, u.stage); wr_u16(pl, (uint16_t)u.wx); wr_u16(pl, (uint16_t)u.wy); wr_bytes(pl, u.name, 33); } write_chunk('2INU', pl);
    } else if (!s.unitsV1.empty()) {
        std::vector<uint8_t> pl; wr_u32(pl, (uint32_t)s.unitsV1.size());
        for (const auto& u : s.unitsV1) { pl.push_back(u.NI); wr_u16(pl, u.NIndex); wr_u32(pl, (uint32_t)u.x); wr_u32(pl, (uint32_t)u.y); wr_u16(pl, u.life); wr_u16(pl, u.stage); wr_bytes(pl, u.name, 33); } write_chunk('TINU', pl);
    }
    // Walls V2 -> V1 -> V0 (write whichever present)
    if (!s.wallsV2.empty()) {
        std::vector<uint8_t> pl; wr_u32(pl, (uint32_t)s.wallsV2.size());
        for (const auto& cl : s.wallsV2) { pl.push_back(cl.hdr.type); wr_u32(pl, cl.hdr.nCells); wr_u32(pl, cl.hdr.nmIndex); for (const auto& wc : cl.cells) { wr_u16(pl, (uint16_t)wc.x); wr_u16(pl, (uint16_t)wc.y); pl.push_back(wc.type); pl.push_back(wc.NI); pl.push_back(wc.stage); pl.push_back(wc.maxStage); wr_u16(pl, wc.health); wr_u16(pl, wc.maxHealth); pl.push_back(wc.sprite); pl.push_back(wc.sprBase); wr_u16(pl, wc.clusterIndex); pl.push_back(wc.visible); wr_u16(pl, wc.gateIndex); wr_u32(pl, wc.locks); pl.push_back(wc.dirMask); wr_u16(pl, wc.oIndex); } } write_chunk('2LAW', pl);
    } else if (!s.wallsV1.empty()) {
        std::vector<uint8_t> pl; wr_u32(pl, (uint32_t)s.wallsV1.size());
        for (const auto& cl : s.wallsV1) { pl.push_back(cl.hdr.type); wr_u32(pl, cl.hdr.nCells); wr_u32(pl, cl.hdr.nmIndex); for (const auto& wc : cl.cells) { wr_u16(pl, (uint16_t)wc.x); wr_u16(pl, (uint16_t)wc.y); pl.push_back(wc.type); pl.push_back(wc.NI); pl.push_back(wc.stage); pl.push_back(wc.maxStage); wr_u16(pl, wc.health); wr_u16(pl, wc.maxHealth); pl.push_back(wc.sprite); pl.push_back(wc.sprBase); wr_u16(pl, wc.clusterIndex); pl.push_back(wc.visible); } } write_chunk('1LAW', pl);
    } else if (!s.wallsV0.empty()) {
        std::vector<uint8_t> pl; wr_u32(pl, (uint32_t)s.wallsV0.size());
        for (const auto& cl : s.wallsV0) { pl.push_back(cl.hdr.type); wr_u32(pl, cl.hdr.nCells); wr_u32(pl, cl.hdr.nmIndex); for (const auto& wc : cl.cells) { wr_u16(pl, (uint16_t)wc.x); wr_u16(pl, (uint16_t)wc.y); pl.push_back(wc.type); pl.push_back(wc.NI); pl.push_back(wc.stage); pl.push_back(wc.maxStage); wr_u16(pl, wc.health); wr_u16(pl, wc.maxHealth); pl.push_back(wc.sprite); pl.push_back(wc.sprBase); wr_u16(pl, wc.clusterIndex); } } write_chunk('LLAW', pl);
    }
    // 1COL
    if (!s.lockBars.empty() || !s.unlockBars.empty()) { std::vector<uint8_t> pl; wr_u32(pl, (uint32_t)s.lockBars.size()); wr_u32(pl, (uint32_t)s.unlockBars.size()); for (const auto& bc : s.lockBars) { wr_u16(pl, bc.x); wr_u16(pl, bc.y); } for (const auto& bc : s.unlockBars) { wr_u16(pl, bc.x); wr_u16(pl, bc.y); } write_chunk('1COL', pl); }
    // 2AES
    if (s.waterLx > 0 && s.waterLy > 0 && s.waterDeep.size() == (size_t)s.waterLx * (size_t)s.waterLy && s.waterBright.size() == (size_t)s.waterLx * (size_t)s.waterLy) {
        std::vector<uint8_t> pl; wr_u32(pl, (uint32_t)s.waterLx); wr_u32(pl, (uint32_t)s.waterLy); for (int y = 0; y < s.waterLy; ++y) { wr_bytes(pl, s.waterDeep.data() + (size_t)y * (size_t)s.waterLx, (size_t)s.waterLx); wr_bytes(pl, s.waterBright.data() + (size_t)y * (size_t)s.waterLx, (size_t)s.waterLx); } write_chunk('2AES', pl);
    }
    // USER
    { std::vector<uint8_t> pl; pl.reserve(8*8*4); for (int i = 0; i < 8; ++i) for (int j = 0; j < 8; ++j) wr_u32(pl, (uint32_t)s.RES8x8[i][j]); if (!pl.empty()) write_chunk('USER', pl); }
    // 1POT
    if (!s.landTopo.areas.empty() || !s.landTopo.motionLinks.empty() || !s.landTopo.topRef.empty()) {
        const uint32_t nAreas = (uint32_t)s.landTopo.areas.size();
        std::vector<uint8_t> pl; wr_u32(pl, nAreas);
        for (const auto& ar : s.landTopo.areas) {
            MapSceneData::AreaDisk32 ad{}; ad.x = ar.x; ad.y = ar.y; ad.importance = ar.importance; ad.nTrees = ar.nTrees; ad.nStones = ar.nStones; ad.nMines = (uint16_t)ar.minesIdx.size(); ad.minesIdxPtr = 0; ad.nLinks = (uint16_t)(ar.linksPairs.size() / 2u); ad.linkPtr = 0; ad.maxLink = ad.nLinks; wr_bytes(pl, &ad, sizeof(MapSceneData::AreaDisk32)); if (!ar.minesIdx.empty()) wr_bytes(pl, ar.minesIdx.data(), ar.minesIdx.size() * sizeof(uint16_t)); if (!ar.linksPairs.empty()) wr_bytes(pl, ar.linksPairs.data(), ar.linksPairs.size() * sizeof(uint16_t));
        }
        if (!s.landTopo.motionLinks.empty()) wr_bytes(pl, s.landTopo.motionLinks.data(), s.landTopo.motionLinks.size() * sizeof(uint16_t));
        if (!s.landTopo.linksDist.empty()) wr_bytes(pl, s.landTopo.linksDist.data(), s.landTopo.linksDist.size() * sizeof(uint16_t));
        if (!s.landTopo.topRef.empty()) wr_bytes(pl, s.landTopo.topRef.data(), s.landTopo.topRef.size() * sizeof(uint16_t));
        write_chunk('1POT', pl);
    }
    // WPOT
    if (!s.waterTopo.areas.empty() || !s.waterTopo.motionLinks.empty() || !s.waterTopo.topRef.empty()) {
        const uint32_t nAreas = (uint32_t)s.waterTopo.areas.size();
        std::vector<uint8_t> pl; wr_u32(pl, nAreas);
        for (const auto& ar : s.waterTopo.areas) { MapSceneData::AreaDisk32 ad{}; ad.x = ar.x; ad.y = ar.y; ad.importance = ar.importance; ad.nTrees = ar.nTrees; ad.nStones = ar.nStones; ad.nMines = (uint16_t)ar.minesIdx.size(); ad.minesIdxPtr = 0; ad.nLinks = (uint16_t)(ar.linksPairs.size() / 2u); ad.linkPtr = 0; ad.maxLink = ad.nLinks; wr_bytes(pl, &ad, sizeof(MapSceneData::AreaDisk32)); if (!ar.minesIdx.empty()) wr_bytes(pl, ar.minesIdx.data(), ar.minesIdx.size() * sizeof(uint16_t)); if (!ar.linksPairs.empty()) wr_bytes(pl, ar.linksPairs.data(), ar.linksPairs.size() * sizeof(uint16_t)); }
        if (!s.waterTopo.motionLinks.empty()) wr_bytes(pl, s.waterTopo.motionLinks.data(), s.waterTopo.motionLinks.size() * sizeof(uint16_t));
        if (!s.waterTopo.linksDist.empty()) wr_bytes(pl, s.waterTopo.linksDist.data(), s.waterTopo.linksDist.size() * sizeof(uint16_t));
        if (!s.waterTopo.topRef.empty()) wr_bytes(pl, s.waterTopo.topRef.data(), s.waterTopo.topRef.size() * sizeof(uint16_t));
        write_chunk('WPOT', pl);
    }
    // 1TAG
    if (!s.gates.empty()) { std::vector<uint8_t> pl; wr_u32(pl, (uint32_t)s.gates.size()); wr_u32(pl, (uint32_t)std::max(s.maxGates, (int)s.gates.size())); for (const auto& g : s.gates) { wr_u16(pl, (uint16_t)g.x); wr_u16(pl, (uint16_t)g.y); pl.push_back(g.NI); pl.push_back(g.nMask); pl.push_back(g.state); pl.push_back(g.delay); pl.push_back(g.locked); pl.push_back(g.charId); } write_chunk('1TAG', pl); }
    // 1NOZ
    if (!s.zonesGroupsRaw.empty()) write_chunk('1NOZ', s.zonesGroupsRaw);
    // 1ROF / MROF (prefer new)
    if (!s.formationsRaw.empty()) write_chunk('1ROF', s.formationsRaw);
    // LLD.
    { std::vector<uint8_t> pl; pl.resize(200, 0); std::memcpy(pl.data(), s.mapScenaryDLL, 200); write_chunk('LLD.', pl); }
    // AIIA
    { std::vector<uint8_t> pl; wr_u32(pl, (uint32_t)s.RM_LandType); wr_u32(pl, (uint32_t)s.RM_Resstart); wr_u32(pl, (uint32_t)s.RM_Restot); write_chunk('AIIA', pl); }
    // SMSP
    if (!s.smsRaw.empty()) write_chunk('SMSP', s.smsRaw);
    // PEAC
    if (!s.peacetimeRaw.empty()) write_chunk('PEAC', s.peacetimeRaw);
    // 1VIR
    if (!s.riversDir.empty() && s.riversDir.size() == s.riversVol.size()) { std::vector<uint8_t> pl; pl.reserve(s.riversDir.size() + s.riversVol.size()); wr_bytes(pl, s.riversDir.data(), s.riversDir.size()); wr_bytes(pl, s.riversVol.data(), s.riversVol.size()); write_chunk('1VIR', pl); }
    // MDNE: terminator id only (no size)
    wr_u32(buf, 'MDNE');

    // Write to disk
    std::ofstream ofs(name, std::ios::binary);
    if (!ofs) return false;
    ofs.write(reinterpret_cast<const char*>(buf.data()), (std::streamsize)buf.size());
    return ofs.good();
}

} // namespace resource_io::map


