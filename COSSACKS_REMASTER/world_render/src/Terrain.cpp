#include "world_render/Terrain.hpp"
#include "world_render/WorldRender.hpp"

#include "engine_core/Texture.hpp"
#include "engine_core/Render2D.hpp"
#include "resource_io/ResourceIO.hpp"
#include "resource_io/ColorPalette.hpp"
#include "resource_io/RLC.hpp"
#include <iostream>
#include "resource_io/TexturesMeta.hpp"

#include <vector>
#include <cstring>
#include <cmath>
#include <unordered_map>

namespace world_render::terrain {

namespace {

static engine_core::texture::TextureHandle gTileAtlas = 0;
static bool gSimpleTerrain = false; // GPU indexed+masks pipeline is the default; no fallback path
static engine_core::texture::TextureHandle gAtlasIndexTex = 0;
static engine_core::texture::TextureHandle gPaletteTex = 0;
static engine_core::texture::TextureHandle gTrans4Tex = 0;
static engine_core::texture::TextureHandle gTrans8Tex = 0;
static engine_core::texture::TextureHandle gMaskATexArray = 0;
static engine_core::texture::TextureHandle gMaskBTexArray = 0;
static engine_core::texture::TextureHandle gMaskCTexArray = 0;
static engine_core::texture::TextureHandle gMaskDTexArray = 0;
static int gAtlasTilesX = 16;
static int gAtlasTilesY = 16;
static int gTileW = 64;
static int gTileH = 64;
static bool gAtlasIndexed8 = false;
static std::vector<unsigned char> gAtlasIdx8; // width*height, valid if gAtlasIndexed8
static int gAtlasW = 0, gAtlasH = 0;

static const int16_t* gHeights = nullptr;
static const uint8_t* gTexIdx = nullptr;
static int gVertInLine = 0;
static int gMaxTH = 0;
static const uint8_t* gSectMap = nullptr; // size maxSector*maxTH*6
static int gMaxSector = 0;

// Fog transition tables used by original masks blending
static std::vector<unsigned char> gTrans4; // 65536 bytes
static std::vector<unsigned char> gTrans8; // 65536 bytes

// Decoded mask frames (A/B/C/D) — each entry is width*height 8-bit mask (0 = no-op, 1..255 = apply)
struct MaskSet {
    std::vector<std::vector<unsigned char>> frames;
    std::vector<int> widths;
    std::vector<int> heights;
};
static MaskSet gMaskA, gMaskB, gMaskC, gMaskD;
static bool gMasksReady = false;
static const int kTriUnit = 32; // legacy TriUnit = 32, diamond tile is 64x64
static int gDebugPrinted = 0;
static bool gGpuResourcesBound = false;

// BMP reader for tiles3.bmp (8-bit indexed, starts at offset 0x436 in original). Here we load the full RGBA via ResourceIO convenience.
// We rely on ResourceIO to read raw file and interpret as 8-bit indexed using color palette module for simplicity.
static bool load_tiles3_rgba(std::vector<unsigned char>& outRGBA, int& outW, int& outH) {
    std::vector<unsigned char> bytes;
    // Locate by suffix anywhere in resources (archives or disk), case-insensitive
    std::string found;
    if (!resource_io::try_find_entry_by_suffix("tiles3.bmp", found)) {
        found = "tiles3.bmp";
    }
    if (!resource_io::read_file_anywhere(found, bytes)) {
        std::cerr << "[terrain] Failed to load tile atlas 'tiles3.bmp'" << std::endl;
        return false;
    }
    if (bytes.size() < 54u) {
        std::cerr << "[terrain] tiles3.bmp too small" << std::endl;
        return false;
    }
    const unsigned char* p = bytes.data();
    // BITMAPFILEHEADER
    if (p[0] != 'B' || p[1] != 'M') {
        std::cerr << "[terrain] tiles3.bmp not a BM file" << std::endl;
        return false;
    }
    auto rd_u16 = [](const unsigned char* q) -> unsigned int { return (unsigned int)q[0] | ((unsigned int)q[1] << 8); };
    auto rd_u32 = [](const unsigned char* q) -> unsigned int { return (unsigned int)q[0] | ((unsigned int)q[1] << 8) | ((unsigned int)q[2] << 16) | ((unsigned int)q[3] << 24); };
    const unsigned int offBits = rd_u32(p + 10);
    const unsigned int biSize = rd_u32(p + 14);
    if (bytes.size() < 14u + biSize) {
        std::cerr << "[terrain] tiles3.bmp bad header size" << std::endl;
        return false;
    }
    const unsigned char* bi = p + 14;
    const int width  = (int)rd_u32(bi + 4);
    int height = (int)rd_u32(bi + 8);
    const unsigned int planes = rd_u16(bi + 12);
    const unsigned int bpp    = rd_u16(bi + 14);
    const unsigned int compression = rd_u32(bi + 16);
    const unsigned int clrUsed = rd_u32(bi + 32);
    if (planes != 1 || (bpp != 8 && bpp != 24) || (compression != 0)) {
        std::cerr << "[terrain] tiles3.bmp unsupported format (bpp=" << bpp << ", comp=" << compression << ")" << std::endl;
        return false;
    }
    const bool topDown = (height < 0);
    if (height < 0) height = -height;
    outW = width;
    outH = height;
    // Read palette if 8bpp
    std::vector<unsigned char> paletteRGBA;
    if (bpp == 8) {
        unsigned int numColors = clrUsed ? clrUsed : 256;
        // Palette starts after BITMAPFILEHEADER (14) and info header (biSize)
        const size_t palStart = 14u + biSize;
        const size_t palBytes = static_cast<size_t>(numColors) * 4u;
        if (bytes.size() < palStart + palBytes) {
            std::cerr << "[terrain] tiles3.bmp palette truncated" << std::endl;
            return false;
        }
        paletteRGBA.resize(numColors * 4u);
        for (unsigned int i = 0; i < numColors; ++i) {
            const unsigned char B = bytes[palStart + i * 4u + 0];
            const unsigned char G = bytes[palStart + i * 4u + 1];
            const unsigned char R = bytes[palStart + i * 4u + 2];
            // bytes[palStart + i*4 + 3] is reserved (0)
            paletteRGBA[i * 4u + 0] = R;
            paletteRGBA[i * 4u + 1] = G;
            paletteRGBA[i * 4u + 2] = B;
            paletteRGBA[i * 4u + 3] = 255u;
        }
    }
    // Decode pixel data
    if (bytes.size() < offBits) {
        std::cerr << "[terrain] tiles3.bmp offBits beyond file" << std::endl;
        return false;
    }
    const unsigned char* pix = p + offBits;
    const size_t dstBytes = static_cast<size_t>(width) * static_cast<size_t>(height) * 4u;
    outRGBA.assign(dstBytes, 0);
    if (bpp == 8) {
        const size_t rowStride = ((static_cast<size_t>(width) + 3u) & ~3u);
        gAtlasIndexed8 = true;
        gAtlasIdx8.assign(static_cast<size_t>(width) * static_cast<size_t>(height), 0);
        for (int y = 0; y < height; ++y) {
            const int srcY = topDown ? y : (height - 1 - y);
            const unsigned char* src = pix + static_cast<size_t>(srcY) * rowStride;
            for (int x = 0; x < width; ++x) {
                const unsigned char idx = src[x];
                const unsigned char* c = (idx < paletteRGBA.size() / 4u) ? &paletteRGBA[idx * 4u] : resource_io::color_palette::get_rgba256x4() + idx * 4u;
                const size_t di = (static_cast<size_t>(y) * static_cast<size_t>(width) + static_cast<size_t>(x)) * 4u;
                outRGBA[di + 0] = c[0];
                outRGBA[di + 1] = c[1];
                outRGBA[di + 2] = c[2];
                outRGBA[di + 3] = 255u;
                gAtlasIdx8[static_cast<size_t>(y) * static_cast<size_t>(width) + static_cast<size_t>(x)] = idx;
            }
        }
    } else if (bpp == 24) {
        const size_t rowStride = ((static_cast<size_t>(width) * 3u + 3u) & ~3u);
        gAtlasIdx8.assign(static_cast<size_t>(width) * static_cast<size_t>(height), 0);
        // Build RGB->index map from current palette; assume exact matches for tiles3.bmp colors
        const unsigned char* pal = resource_io::color_palette::get_rgba256x4();
        std::unordered_map<uint32_t, uint8_t> rgb2idx;
        rgb2idx.reserve(256);
        for (int i = 0; i < 256; ++i) {
            const uint8_t r = pal[i * 4 + 0];
            const uint8_t g = pal[i * 4 + 1];
            const uint8_t b = pal[i * 4 + 2];
            const uint32_t key = (static_cast<uint32_t>(r) << 16) | (static_cast<uint32_t>(g) << 8) | b;
            rgb2idx[key] = static_cast<uint8_t>(i);
        }
        for (int y = 0; y < height; ++y) {
            const int srcY = topDown ? y : (height - 1 - y);
            const unsigned char* src = pix + static_cast<size_t>(srcY) * rowStride;
            for (int x = 0; x < width; ++x) {
                const uint8_t B = src[x * 3 + 0];
                const uint8_t G = src[x * 3 + 1];
                const uint8_t R = src[x * 3 + 2];
                const size_t di = (static_cast<size_t>(y) * static_cast<size_t>(width) + static_cast<size_t>(x));
                const size_t di4 = di * 4u;
                outRGBA[di4 + 0] = R;
                outRGBA[di4 + 1] = G;
                outRGBA[di4 + 2] = B;
                outRGBA[di4 + 3] = 255u;
                const uint32_t key = (static_cast<uint32_t>(R) << 16) | (static_cast<uint32_t>(G) << 8) | B;
                auto it = rgb2idx.find(key);
                if (it != rgb2idx.end()) {
                    gAtlasIdx8[di] = it->second;
                } else {
                    // Fallback: nearest palette color
                    int bestIdx = 0; int bestDist = 1 << 30;
                    for (int i = 0; i < 256; ++i) {
                        int dr = int(pal[i * 4 + 0]) - int(R);
                        int dg = int(pal[i * 4 + 1]) - int(G);
                        int db = int(pal[i * 4 + 2]) - int(B);
                        int d = dr * dr + dg * dg + db * db;
                        if (d < bestDist) { bestDist = d; bestIdx = i; if (d == 0) break; }
                    }
                    gAtlasIdx8[di] = static_cast<uint8_t>(bestIdx);
                }
            }
        }
        gAtlasIndexed8 = true;
    }
    gAtlasW = width; gAtlasH = height;
    return true;
}

} // namespace

// Forward declarations for helper functions defined later in this file
static bool load_transitions_tables();
static engine_core::texture::TextureHandle upload_mask_set_to_array(const MaskSet& set);
static void ensure_masks_loaded();
static void ensure_gpu_terrain_resources();

int initialize_tiles() {
    if (gTileAtlas) return gAtlasTilesX * gAtlasTilesY;
    // Load tiling and textures meta for seam logic later
    (void)resource_io::terrain_meta::load_from_files();
    std::vector<unsigned char> rgba;
    int w = 0, h = 0;
    if (!load_tiles3_rgba(rgba, w, h)) {
        std::cerr << "[terrain] tiles3.bmp decode failed" << std::endl;
        return 0;
    }
    gTileAtlas = engine_core::texture::create_texture_rgba(w, h, rgba.data());
    // Derive atlas grid from actual image size (original uses 64x64 tiles laid out in a grid)
    if (gTileW > 0 && gTileH > 0) {
        gAtlasTilesX = w / gTileW;
        gAtlasTilesY = h / gTileH;
        if (gAtlasTilesX <= 0) gAtlasTilesX = 16;
        if (gAtlasTilesY <= 0) gAtlasTilesY = 16;
    }
    std::cout << "[terrain] tile atlas loaded: " << w << "x" << h
              << " (tiles " << gAtlasTilesX << "x" << gAtlasTilesY << ")"
              << ", bppIdx=" << (gAtlasIndexed8 ? 8 : 0) << std::endl;
    std::cout << "[terrain] atlasW=" << gAtlasW << " atlasH=" << gAtlasH
              << " tileW=" << gTileW << " tileH=" << gTileH << std::endl;
    // Try to enable GPU indexed path if possible
    ensure_gpu_terrain_resources();
    return gAtlasTilesX * gAtlasTilesY;
}

void set_map_data(const int16_t* heights, const uint8_t* tex, int vertInLine, int maxTH) {
    gHeights = heights;
    gTexIdx = tex;
    gVertInLine = vertInLine;
    gMaxTH = maxTH;
    std::cout << "[terrain] set_map_data: heights=" << (void*)gHeights
              << " tex=" << (void*)gTexIdx
              << " vertInLine=" << gVertInLine
              << " maxTH=" << gMaxTH << std::endl;
}

void set_sect_map(const std::uint8_t* sectMap, int maxSector, int maxTH) {
    gSectMap = sectMap;
    gMaxSector = maxSector;
    (void)maxTH; // same as gMaxTH; used for sanity only
}

bool is_ready() {
    return gHeights != nullptr && gTexIdx != nullptr && gVertInLine > 1 && gMaxTH > 1;
}

static inline float compute_light_at_vertex(int vx, int vy) {
    auto getH = [&](int x, int y) -> int {
        if (x < 0) x = 0; if (y < 0) y = 0;
        if (x >= gVertInLine) x = gVertInLine - 1; if (y >= gMaxTH) y = gMaxTH - 1;
        return static_cast<int>(gHeights[y * gVertInLine + x]);
    };
    const int dy = getH(vx + 1, vy) - getH(vx - 1, vy);
    const int dx = getH(vx, vy + 1) - getH(vx, vy - 1);
    const float Lx = 0.0f, Ly = 20.0f, Lz = 30.0f;
    const float len = std::sqrt(dx * dx + dy * dy + 64.0f * 64.0f);
    float lig = 16.0f + ((dx * Lx + dy * Ly + 64.0f * Lz) / (len + 1e-5f)) / 16.0f;
    if (lig < 2.0f) lig = 2.0f; if (lig > 31.0f) lig = 31.0f;
    const float k = (32.0f - lig) / 32.0f;
    return 0.5f + 0.5f * k;
}

// --------------------- Masks and transitions (legacy port) ---------------------

static bool load_transitions_tables() {
    if (!gTrans4.empty() && !gTrans8.empty()) return true;
    std::vector<unsigned char> data;
    // Try default set '0\\' first, then root
    if (!resource_io::read_file_anywhere("0\\agew_tr4.grd", data)) {
        data.clear(); (void)resource_io::read_file_anywhere("agew_tr4.grd", data);
    }
    if (data.size() >= 65536) {
        gTrans4.assign(data.begin(), data.begin() + 65536);
    } else {
        gTrans4.assign(65536, 0);
        for (int a = 0; a < 256; ++a) for (int b = 0; b < 256; ++b) gTrans4[(a << 8) | b] = static_cast<unsigned char>((a + b) >> 1);
    }
    data.clear();
    if (!resource_io::read_file_anywhere("0\\agew_tr.grd", data)) {
        data.clear(); (void)resource_io::read_file_anywhere("agew_tr.grd", data);
    }
    if (data.size() >= 65536) {
        gTrans8.assign(data.begin(), data.begin() + 65536);
    } else {
        gTrans8.assign(65536, 0);
        for (int a = 0; a < 256; ++a) for (int b = 0; b < 256; ++b) gTrans8[(a << 8) | b] = static_cast<unsigned char>((3 * a + b) >> 2);
    }
    return true;
}

static bool load_masks_rlc(const char* path, MaskSet& out) {
    resource_io::rlc::RLCTableData tbl;
    if (!resource_io::rlc::load_rlc(path, tbl)) return false;
    // Heuristically assume up to 64 frames; decode sequentially until failure
    out.frames.clear(); out.widths.clear(); out.heights.clear();
    for (int i = 0; i < 128; ++i) {
        int w = 0, h = 0; if (!resource_io::rlc::get_subimage_size(tbl, i, w, h)) break;
        std::vector<unsigned char> idx; if (!resource_io::rlc::decode_subimage_indexed(tbl, i, idx)) break;
        // Normalize to 64x64 canvas at top-left; pad if needed
        std::vector<unsigned char> mask64(64 * 64, 0);
        const int cw = std::min(64, w); const int ch = std::min(64, h);
        for (int y = 0; y < ch; ++y) {
            std::memcpy(&mask64[y * 64], idx.data() + static_cast<size_t>(y) * static_cast<size_t>(w), cw);
        }
        out.frames.emplace_back(std::move(mask64));
        out.widths.emplace_back(cw);
        out.heights.emplace_back(ch);
    }
    return !out.frames.empty();
}

static engine_core::texture::TextureHandle upload_mask_set_to_array(const MaskSet& set) {
    if (set.frames.empty()) return 0;
    const int layers = static_cast<int>(set.frames.size());
    std::vector<unsigned char> concat;
    concat.reserve(static_cast<size_t>(64) * 64u * static_cast<size_t>(layers));
    for (int i = 0; i < layers; ++i) {
        const std::vector<unsigned char>& frm = set.frames[static_cast<size_t>(i)];
        if (frm.size() >= 64u * 64u) {
            concat.insert(concat.end(), frm.begin(), frm.begin() + 64u * 64u);
        } else {
            // pad
            std::vector<unsigned char> tmp(64u * 64u, 0);
            std::memcpy(tmp.data(), frm.data(), frm.size());
            concat.insert(concat.end(), tmp.begin(), tmp.end());
        }
    }
    return engine_core::texture::create_texture_r8_array(64, 64, layers, concat.data());
}

static void ensure_masks_loaded() {
    if (gMasksReady) return;
    (void)load_transitions_tables();
    // Preflight: if any mask resource missing, skip masks entirely (GPU path will render base only)
    std::string fA, fB, fC, fD;
    bool haveA = resource_io::try_find_entry_by_suffix("ms_a.msk", fA);
    bool haveB = resource_io::try_find_entry_by_suffix("ms_b.msk", fB);
    bool haveC = resource_io::try_find_entry_by_suffix("ms_c.msk", fC);
    bool haveD = resource_io::try_find_entry_by_suffix("ms_d.msk", fD);
    if (!(haveA && haveB && haveC && haveD)) {
        gMasksReady = false;
        std::cout << "[mask] resources missing; A=" << (haveA?1:0) << " B=" << (haveB?1:0)
                  << " C=" << (haveC?1:0) << " D=" << (haveD?1:0) << " — masks disabled" << std::endl;
        return;
    }
    bool okA = load_masks_rlc(fA.c_str(), gMaskA);
    bool okB = load_masks_rlc(fB.c_str(), gMaskB);
    bool okC = load_masks_rlc(fC.c_str(), gMaskC);
    bool okD = load_masks_rlc(fD.c_str(), gMaskD);
    gMasksReady = okA && okB && okC && okD;
    std::cout << "[mask] loaded A=" << (okA?1:0)
              << " B=" << (okB?1:0)
              << " C=" << (okC?1:0)
              << " D=" << (okD?1:0)
              << " ready=" << (gMasksReady?1:0) << std::endl;
}

static void ensure_gpu_terrain_resources() {
    if (!gAtlasIndexed8 || !gAtlasW || !gAtlasH) return;
    // Upload atlas index texture if needed
    if (!gAtlasIndexTex) {
        gAtlasIndexTex = engine_core::texture::create_texture_r8(gAtlasW, gAtlasH, gAtlasIdx8.data());
    }
    // Upload palette texture (256x1 RGBA)
    if (!gPaletteTex) {
        const unsigned char* pal = resource_io::color_palette::get_rgba256x4();
        gPaletteTex = engine_core::texture::create_texture_rgba(256, 1, pal);
    }
    // Load transition tables and upload as R8 256x256
    ensure_masks_loaded();
    if (!gTrans4Tex && !gTrans4.empty()) {
        gTrans4Tex = engine_core::texture::create_texture_r8(256, 256, gTrans4.data());
    }
    if (!gTrans8Tex && !gTrans8.empty()) {
        gTrans8Tex = engine_core::texture::create_texture_r8(256, 256, gTrans8.data());
    }
    // Upload mask arrays if available
    if (!gMaskATexArray && gMasksReady) gMaskATexArray = upload_mask_set_to_array(gMaskA);
    if (!gMaskBTexArray && gMasksReady) gMaskBTexArray = upload_mask_set_to_array(gMaskB);
    if (!gMaskCTexArray && gMasksReady) gMaskCTexArray = upload_mask_set_to_array(gMaskC);
    if (!gMaskDTexArray && gMasksReady) gMaskDTexArray = upload_mask_set_to_array(gMaskD);
    // Bind resources into terrain pipeline (even if masks missing; overlays will be disabled internally)
    if (!gGpuResourcesBound && gAtlasIndexTex && gPaletteTex) {
        engine_core::render2d::set_terrain_gpu_resources(
            gAtlasIndexTex,
            gPaletteTex,
            gTrans4Tex,
            gTrans8Tex,
            gMaskATexArray,
            gMaskBTexArray,
            gMaskCTexArray,
            gMaskDTexArray,
            gAtlasW,
            gAtlasH,
            static_cast<int>(gMaskA.frames.size()),
            static_cast<int>(gMaskB.frames.size()),
            static_cast<int>(gMaskC.frames.size()),
            static_cast<int>(gMaskD.frames.size()));
        gGpuResourcesBound = true;
        gSimpleTerrain = false;
        std::cout << "[terrain] GPU indexed terrain enabled (atlasIdx R8, palette 256x1, masks="
                  << (gMasksReady?"on":"off") << ")" << std::endl;
    }
}

static inline int tile_index_to_atlas_ofs(int tileIndex) {
    const int tileU = tileIndex % gAtlasTilesX;
    const int tileV = tileIndex / gAtlasTilesX;
    return tileU * gTileW + tileV * gTileH * gAtlasW;
}

// Base triangle fill (type 1): upward oriented (like PrepareIntersection1)
static void base_triangle_type1_fill_idx(uint8_t* dst64x64, const unsigned char* atlasIdx, int atlasOfs, int x0, int y0) {
    // Two trapezoids of height 16 each, widths growing then shrinking by step 2
    int width = 2;
    for (int row = 0; row < 16; ++row) {
        const int y = (y0 + row) & 63;
        for (int dx = 0; dx < width && dx < 64; ++dx) {
            const int x = (x0 + dx) & 63;
            // sample from tile atlas
            const int srcX = x; const int srcY = y;
            const int atlasIndex = atlasOfs + srcX + srcY * gAtlasW;
            dst64x64[y * 64 + x] = atlasIdx[atlasIndex];
        }
        width += 2; if (width > 64) width = 64;
    }
    width -= 2;
    for (int row = 16; row < 32; ++row) {
        const int y = (y0 + row) & 63;
        for (int dx = 0; dx < width && dx < 64; ++dx) {
            const int x = (x0 + dx) & 63;
            const int srcX = x; const int srcY = y;
            const int atlasIndex = atlasOfs + srcX + srcY * gAtlasW;
            dst64x64[y * 64 + x] = atlasIdx[atlasIndex];
        }
        width -= 2; if (width < 0) width = 0;
    }
}

// Base triangle fill (type 2): downward/right oriented (like PrepareIntersection2)
static void base_triangle_type2_fill_idx(uint8_t* dst64x64, const unsigned char* atlasIdx, int atlasOfs, int x0, int y0) {
    // Mirror of type1 around the center; starting from (x0+32, y0-16) and copying right-to-left
    int width = 2;
    int curX = (x0 + 32) & 63;
    int curY = (y0 + 64 - 16) & 63; // y0-16
    for (int row = 0; row < 16; ++row) {
        const int y = (curY + row) & 63;
        for (int dx = 0; dx < width && dx < 64; ++dx) {
            int x = (curX - dx) & 63;
            const int atlasIndex = atlasOfs + x + y * gAtlasW;
            dst64x64[y * 64 + x] = atlasIdx[atlasIndex];
        }
        width += 2; if (width > 64) width = 64;
    }
    width -= 2;
    for (int row = 16; row < 32; ++row) {
        const int y = (curY + row) & 63;
        for (int dx = 0; dx < width && dx < 64; ++dx) {
            int x = (curX - dx) & 63;
            const int atlasIndex = atlasOfs + x + y * gAtlasW;
            dst64x64[y * 64 + x] = atlasIdx[atlasIndex];
        }
        width -= 2; if (width < 0) width = 0;
    }
}

static void copy_mask_layer(uint8_t* dst64x64, const unsigned char* atlasIdx, int atlasOfs, int x0, int y0,
                            const MaskSet& set, int maskId, int mode /*0=A, 4=B, 8=C, 12=D*/) {
    if (maskId < 0 || maskId >= static_cast<int>(set.frames.size())) return;
    const std::vector<unsigned char>& mask = set.frames[maskId];
    const int w = 64; const int h = 64;
    for (int my = 0; my < h; ++my) {
        for (int mx = 0; mx < w; ++mx) {
            if (!mask[static_cast<size_t>(my) * 64u + static_cast<size_t>(mx)]) continue;
            const int dstX = (x0 + mx) & 63;
            const int dstY = (y0 + my) & 63;
            const int atlasIndex = atlasOfs + dstX + dstY * gAtlasW;
            const unsigned char src = atlasIdx[atlasIndex];
            unsigned char& dest = dst64x64[dstY * 64 + dstX];
            switch (mode) {
                case 0: // A: copy
                    dest = src; break;
                case 4: // B: trans4(dest, src)
                    dest = gTrans4[(static_cast<int>(dest) << 8) | src]; break;
                case 8: // C: trans8(dest, src)
                    dest = gTrans8[(static_cast<int>(dest) << 8) | src]; break;
                case 12: // D: trans4(src, dest) (order swapped)
                    dest = gTrans4[(static_cast<int>(src) << 8) | dest]; break;
                default:
                    dest = src; break;
            }
        }
    }
}

static void copy_mask_all_layers(uint8_t* dst64x64, const unsigned char* atlasIdx, int atlasOfs, int x0, int y0, int maskId) {
    // Apply layers A, B, C, D in order as in CopyMaskedBitmap
    copy_mask_layer(dst64x64, atlasIdx, atlasOfs, x0, y0, gMaskA, maskId, 0);
    copy_mask_layer(dst64x64, atlasIdx, atlasOfs, x0, y0, gMaskB, maskId, 4);
    copy_mask_layer(dst64x64, atlasIdx, atlasOfs, x0, y0, gMaskC, maskId, 8);
    copy_mask_layer(dst64x64, atlasIdx, atlasOfs, x0, y0, gMaskD, maskId, 12);
}

static void compose_prepare_intersection1(uint8_t* outIdx64, int bm1, int bm2, int bm3, int x0, int y0, int s1, int s2, int s3) {
    std::memset(outIdx64, 0, 64 * 64);
    ensure_masks_loaded(); if (!gAtlasIndexed8) return;
    const unsigned char* atlasIdx = gAtlasIdx8.data();
    const int ofs1 = tile_index_to_atlas_ofs(bm1);
    const int ofs2 = tile_index_to_atlas_ofs(bm2);
    const int ofs3 = tile_index_to_atlas_ofs(bm3);
    // Base: type1
    base_triangle_type1_fill_idx(outIdx64, atlasIdx, ofs1, x0, y0);
    // Mask selection as in original
    if (bm1 == bm2) {
        if (bm3 < bm2) {
            copy_mask_all_layers(outIdx64, atlasIdx, ofs3, x0, y0, 27 + (2 - s2) * 3 + s3);
        } else {
            copy_mask_all_layers(outIdx64, atlasIdx, ofs3, x0, y0, 18 + (2 - s2) * 3 + s3);
        }
        return;
    }
    if (bm2 == bm3) {
        if (bm1 < bm3) {
            copy_mask_all_layers(outIdx64, atlasIdx, ofs1, x0, y0, 45 + (2 - s3) * 3 + s1);
        } else {
            copy_mask_all_layers(outIdx64, atlasIdx, ofs1, x0, y0, 36 + (2 - s3) * 3 + s1);
        }
        return;
    }
    if (bm1 == bm3) {
        if (bm2 < bm3) {
            copy_mask_all_layers(outIdx64, atlasIdx, ofs2, x0, y0, 9 + (2 - s1) * 3 + s2);
        } else {
            copy_mask_all_layers(outIdx64, atlasIdx, ofs2, x0, y0, (2 - s1) * 3 + s2);
        }
        return;
    }
    // Triple intersections
    if (bm1 < bm2 && bm1 < bm3) {
        copy_mask_all_layers(outIdx64, atlasIdx, ofs2, x0, y0, (2 - s1) * 3 + s2);
        copy_mask_all_layers(outIdx64, atlasIdx, ofs3, x0, y0, 18 + (2 - s2) * 3 + s3);
        return;
    }
    if (bm2 < bm1 && bm2 < bm3) {
        copy_mask_all_layers(outIdx64, atlasIdx, ofs1, x0, y0, 36 + (2 - s3) * 3 + s1);
        copy_mask_all_layers(outIdx64, atlasIdx, ofs3, x0, y0, 18 + (2 - s2) * 3 + s3);
        return;
    }
    copy_mask_all_layers(outIdx64, atlasIdx, ofs1, x0, y0, 36 + (2 - s3) * 3 + s1);
    copy_mask_all_layers(outIdx64, atlasIdx, ofs2, x0, y0, (2 - s1) * 3 + s2);
}

static void compose_prepare_intersection2(uint8_t* outIdx64, int bm1, int bm2, int bm3, int x0, int y01, int s1, int s2, int s3) {
    std::memset(outIdx64, 0, 64 * 64);
    ensure_masks_loaded(); if (!gAtlasIndexed8) return;
    const unsigned char* atlasIdx = gAtlasIdx8.data();
    const int ofs1 = tile_index_to_atlas_ofs(bm1);
    const int ofs2 = tile_index_to_atlas_ofs(bm2);
    const int ofs3 = tile_index_to_atlas_ofs(bm3);
    // Base: type2 uses different orientation
    base_triangle_type2_fill_idx(outIdx64, atlasIdx, ofs1, x0, y01);
    const int y0 = y01 - 31; // original uses y01 then y0=y01-31 for B overlays
    if (bm1 == bm2) {
        if (bm3 < bm2) {
            copy_mask_all_layers(outIdx64, atlasIdx, ofs3, x0, y0, 54 + 27 + (2 - s2) * 3 + s3);
        } else {
            copy_mask_all_layers(outIdx64, atlasIdx, ofs3, x0, y0, 54 + 18 + (2 - s2) * 3 + s3);
        }
        return;
    }
    if (bm2 == bm3) {
        if (bm1 < bm3) {
            copy_mask_all_layers(outIdx64, atlasIdx, ofs1, x0, y0, 54 + 45 + (2 - s3) * 3 + s1);
        } else {
            copy_mask_all_layers(outIdx64, atlasIdx, ofs1, x0, y0, 54 + 36 + (2 - s3) * 3 + s1);
        }
        return;
    }
    if (bm1 == bm3) {
        if (bm2 < bm3) {
            copy_mask_all_layers(outIdx64, atlasIdx, ofs2, x0, y0, 54 + 9 + (2 - s1) * 3 + s2);
        } else {
            copy_mask_all_layers(outIdx64, atlasIdx, ofs2, x0, y0, 54 + (2 - s1) * 3 + s2);
        }
        return;
    }
    if (bm1 < bm2 && bm1 < bm3) {
        copy_mask_all_layers(outIdx64, atlasIdx, ofs2, x0, y0, 54 + (2 - s1) * 3 + s2);
        copy_mask_all_layers(outIdx64, atlasIdx, ofs3, x0, y0, 54 + 18 + (2 - s2) * 3 + s3);
        return;
    }
    if (bm2 < bm1 && bm2 < bm3) {
        copy_mask_all_layers(outIdx64, atlasIdx, ofs1, x0, y0, 54 + 36 + (2 - s3) * 3 + s1);
        copy_mask_all_layers(outIdx64, atlasIdx, ofs3, x0, y0, 54 + 18 + (2 - s2) * 3 + s3);
        return;
    }
    copy_mask_all_layers(outIdx64, atlasIdx, ofs1, x0, y0, 54 + 36 + (2 - s3) * 3 + s1);
    copy_mask_all_layers(outIdx64, atlasIdx, ofs2, x0, y0, 54 + (2 - s1) * 3 + s2);
}

static engine_core::texture::TextureHandle upload_idx64_to_rgba_texture(const uint8_t* idx64) {
    // Convert to RGBA using current palette (from ResourceIO main palette)
    const unsigned char* pal = resource_io::color_palette::get_rgba256x4();
    std::vector<unsigned char> rgba(64 * 64 * 4u);
    for (int y = 0; y < 64; ++y) {
        for (int x = 0; x < 64; ++x) {
            const unsigned char idx = idx64[y * 64 + x];
            const unsigned char* c = pal + static_cast<size_t>(idx) * 4u;
            const size_t di = (static_cast<size_t>(y) * 64u + static_cast<size_t>(x)) * 4u;
            rgba[di + 0] = c[0]; rgba[di + 1] = c[1]; rgba[di + 2] = c[2]; rgba[di + 3] = 255u;
        }
    }
    return engine_core::texture::create_texture_rgba(64, 64, rgba.data());
}

// Compute mask origin (x0,y0) for a given cell (ix,iy) and triangle case using legacy formulas
static void compute_mask_origin_case0(int ix, int iy, int& outX0, int& outY0) {
    const int TriStartX = ix * (4 * kTriUnit);
    const int TriStartY = iy * (2 * kTriUnit);
    const int x1 = TriStartX;
    const int y1 = TriStartY;
    const int x2 = TriStartX;
    const int y2 = TriStartY + (2 * kTriUnit);
    const int x3 = TriStartX + (2 * kTriUnit);
    const int y3 = TriStartY + (1 * kTriUnit);
    int xt1 = -(x1 >> 1) - y1;
    int yt1 = (((x1 + x1 + x1) >> 1) - y1) >> 1;
    int xt2 = -(x2 >> 1) - y2;
    int yt2 = (((x2 + x2 + x2) >> 1) - y2) >> 1;
    int xt3 = -(x3 >> 1) - y3;
    int yt3 = (((x3 + x3 + x3) >> 1) - y3) >> 1;
    int xmin = std::min(xt1, std::min(xt2, xt3));
    int ymin = std::min(yt1, std::min(yt2, yt3));
    xmin -= (xmin & 63);
    ymin -= (ymin & 63);
    outX0 = (xt2 - xmin) & 63;
    outY0 = (yt2 - ymin) & 63;
}

static void compute_mask_origin_case3(int ix, int iy, int& outX0, int& outY0) {
    const int TriStartX = ix * (4 * kTriUnit);
    const int TriStartY = iy * (2 * kTriUnit);
    const int x1 = TriStartX + (4 * kTriUnit);
    const int y1 = TriStartY;
    const int x2 = TriStartX + (4 * kTriUnit);
    const int y2 = TriStartY + (2 * kTriUnit);
    const int x3 = TriStartX + (2 * kTriUnit);
    const int y3 = TriStartY + (1 * kTriUnit);
    int xt1 = -(x1 >> 1) - y1;
    int yt1 = (((x1 + x1 + x1) >> 1) - y1) >> 1;
    int xt2 = -(x2 >> 1) - y2;
    int yt2 = (((x2 + x2 + x2) >> 1) - y2) >> 1;
    int xt3 = -(x3 >> 1) - y3;
    int yt3 = (((x3 + x3 + x3) >> 1) - y3) >> 1;
    int xmin = std::min(xt1, std::min(xt2, xt3));
    int ymin = std::min(yt1, std::min(yt2, yt3));
    xmin -= (xmin & 63);
    ymin -= (ymin & 63);
    outX0 = (xt2 - xmin) & 63;
    outY0 = (yt2 - ymin) & 63;
}

static void compute_mask_origin_case1(int ix, int iy, int& outX0, int& outY0) {
    const int TriStartX = ix * (4 * kTriUnit);
    const int TriStartY = iy * (2 * kTriUnit);
    const int x1 = TriStartX;
    const int y1 = TriStartY;
    const int x2 = TriStartX + (2 * kTriUnit);
    const int y2 = TriStartY - (1 * kTriUnit);
    const int x3 = TriStartX + (2 * kTriUnit);
    const int y3 = TriStartY + (1 * kTriUnit);
    int xt1 = -(x1 >> 1) - y1;
    int yt1 = (((x1 + x1 + x1) >> 1) - y1) >> 1;
    int xt2 = -(x2 >> 1) - y2;
    int yt2 = (((x2 + x2 + x2) >> 1) - y2) >> 1;
    int xt3 = -(x3 >> 1) - y3;
    int yt3 = (((x3 + x3 + x3) >> 1) - y3) >> 1;
    int xmin = std::min(xt1, std::min(xt2, xt3));
    int ymin = std::min(yt1, std::min(yt2, yt3));
    xmin -= (xmin & 63);
    ymin -= (ymin & 63);
    outX0 = (xt3 - xmin) & 63;
    outY0 = (yt3 - ymin) & 63;
}

static void compute_mask_origin_case2(int ix, int iy, int& outX0, int& outY0) {
    // Symmetric to case1 but shifted to the right/bottom quadrant
    const int TriStartX = ix * (4 * kTriUnit);
    const int TriStartY = iy * (2 * kTriUnit);
    const int x1 = TriStartX + (4 * kTriUnit);
    const int y1 = TriStartY + (2 * kTriUnit);
    const int x2 = TriStartX + (2 * kTriUnit);
    const int y2 = TriStartY + (1 * kTriUnit);
    const int x3 = TriStartX + (2 * kTriUnit);
    const int y3 = TriStartY + (3 * kTriUnit);
    int xt1 = -(x1 >> 1) - y1;
    int yt1 = (((x1 + x1 + x1) >> 1) - y1) >> 1;
    int xt2 = -(x2 >> 1) - y2;
    int yt2 = (((x2 + x2 + x2) >> 1) - y2) >> 1;
    int xt3 = -(x3 >> 1) - y3;
    int yt3 = (((x3 + x3 + x3) >> 1) - y3) >> 1;
    int xmin = std::min(xt1, std::min(xt2, xt3));
    int ymin = std::min(yt1, std::min(yt2, yt3));
    xmin -= (xmin & 63);
    ymin -= (ymin & 63);
    outX0 = (xt3 - xmin) & 63;
    outY0 = (yt3 - ymin) & 63;
}

void submit_frame() {
    if (!gHeights || !gTexIdx || gVertInLine <= 1 || gMaxTH <= 1) {
        std::cout << "[terrain] submit skipped: map arrays invalid heights=" << (void*)gHeights
                  << " tex=" << (void*)gTexIdx
                  << " VIL=" << gVertInLine
                  << " MTH=" << gMaxTH << std::endl;
        return;
    }
    // Ensure resources exist
    if (!gTileAtlas) {
        (void)initialize_tiles();
    }
    ensure_gpu_terrain_resources();

    // Iterate over diamond "cells" formed by four adjacent height vertices
    // Vertices (grid):
    //   v00 = (vx,   vy)
    //   v10 = (vx+1, vy)
    //   v01 = (vx,   vy+1)
    //   v11 = (vx+1, vy+1)
    // Screen-space corners of the diamond (top, left, right, bottom) match these vertices via isometric transform
    for (int vy = 0; vy < gMaxTH - 1; ++vy) {
        const int rowOfs = vy * gVertInLine;
        for (int vx = 0; vx < gVertInLine - 1; ++vx) {
            const int v00 = rowOfs + vx;
            const int v10 = v00 + 1;
            const int v01 = v00 + gVertInLine;
            const int v11 = v01 + 1;

            // Pick base texture by top-left vertex (exact seam rules will be applied below when blending tiles)
            const uint8_t t = gTexIdx[v00];
            const int tileU = t % gAtlasTilesX;
            const int tileV = t / gAtlasTilesX;
            const float atlasW = float(gAtlasTilesX * gTileW);
            const float atlasH = float(gAtlasTilesY * gTileH);
            const float u0 = (tileU * gTileW) / atlasW;
            const float u1 = ((tileU + 1) * gTileW) / atlasW;
            // Flip V for OpenGL bottom-left origin to match legacy top-left atlas layout
            const float vTop = (tileV * gTileH) / atlasH;
            const float vBottom = ((tileV + 1) * gTileH) / atlasH;
            const float v0 = 1.0f - vTop;
            const float v1 = 1.0f - vBottom;
            const float uMid = 0.5f * (u0 + u1);
            const float vMid = 0.5f * (v0 + v1);

            // Heights at vertices (half-pixels, as in original)
            const int h00 = static_cast<int>(gHeights[v00]);
            const int h10 = static_cast<int>(gHeights[v10]);
            const int h01 = static_cast<int>(gHeights[v01]);
            const int h11 = static_cast<int>(gHeights[v11]);

            // Project to screen using legacy isometric transform (camera handled inside world_to_screen)
            int topX = 0, topY = 0;     world_render::world_to_screen(vx,     vy,     h00, topX, topY);
            int rightX = 0, rightY = 0; world_render::world_to_screen(vx + 1, vy,     h10, rightX, rightY);
            int leftX = 0, leftY = 0;   world_render::world_to_screen(vx,     vy + 1, h01, leftX, leftY);
            int botX = 0, botY = 0;     world_render::world_to_screen(vx + 1, vy + 1, h11, botX, botY);

            // Simple per-vertex lighting approximation (original uses gradients and fog blend per pixel)
            const float lTop   = compute_light_at_vertex(vx,     vy);
            const float lRight = compute_light_at_vertex(vx + 1, vy);
            const float lLeft  = compute_light_at_vertex(vx,     vy + 1);
            const float lBot   = compute_light_at_vertex(vx + 1, vy + 1);

            // Compute SectMap-based mask IDs as in legacy (StartSide depends on cell address and MaxSector)
            const int maxSector = gMaxSector > 0 ? gMaxSector : ((gVertInLine - 1) / 2);
            // Map grid vertex (vx,vy) to diamond index (ix,iy): each diamond spans two vertices per axis
            const int ix = vx / 2;
            const int iy = vy / 2;
            // Legacy раскладка: на колонку сектора приходится 4 ромба по Y (2 по X в вершинах)
            const int startTriCol = ix;               // по X в «ячейках»
            const int startTriRow = iy;               // по Y в «ячейках»
            const int sectorX = startTriCol % maxSector;
            const int sectorY = startTriRow;          // упрощение: одна строка сектора на 2 строки ромбов
            const int StartSide = (sectorX + sectorY * maxSector) * 6;
            const int SectInLine = maxSector * 6;
            auto sect_safe = [&](int idx)->int{
                if (!gSectMap) return 0;
                const size_t total = static_cast<size_t>(maxSector) * static_cast<size_t>(gMaxTH) * 6u;
                if (idx < 0) return 0;
                if (static_cast<size_t>(idx) >= total) return 0;
                return gSectMap[idx];
            };
            const int s_case0_s1 = sect_safe(StartSide);
            const int s_case0_s2 = sect_safe(StartSide + 2);
            const int s_case0_s3 = sect_safe(StartSide + SectInLine + 1);
            const int s_case1_s1 = sect_safe(StartSide + 2);
            const int s_case1_s2 = sect_safe(StartSide + 3);
            const int s_case1_s3 = sect_safe(StartSide + 1);
            const int s_case2_s1 = sect_safe(StartSide + 3);
            const int s_case2_s2 = sect_safe(StartSide + 4);
            const int s_case2_s3 = sect_safe(StartSide + 5);
            const int s_case3_s1 = sect_safe(StartSide + SectInLine + 4);
            const int s_case3_s2 = sect_safe(StartSide + 6);
            const int s_case3_s3 = sect_safe(StartSide + 5);

            // Decide parity for mapping to legacy TriType (0/1) vs (2/3)
            const bool oddParity = ((ix + iy) & 1) != 0;

            // Helper lambdas to translate legacy PrepareIntersection logic into GPU overlay descriptors
            auto overlays_prepare1 = [](int bm1, int bm2, int bm3, int s1, int s2, int s3,
                                         int& outCount, int& o0tile, int& o0mask, int& o1tile, int& o1mask){
                outCount = 0; o0tile = o1tile = -1; o0mask = o1mask = -1;
                if (bm1 == bm2) {
                    int mid = (bm3 < bm2) ? (27 + (2 - s2) * 3 + s3) : (18 + (2 - s2) * 3 + s3);
                    outCount = 1; o0tile = bm3; o0mask = mid; return;
                }
                if (bm2 == bm3) {
                    int mid = (bm1 < bm3) ? (45 + (2 - s3) * 3 + s1) : (36 + (2 - s3) * 3 + s1);
                    outCount = 1; o0tile = bm1; o0mask = mid; return;
                }
                if (bm1 == bm3) {
                    int mid = (bm2 < bm3) ? (9 + (2 - s1) * 3 + s2) : ((2 - s1) * 3 + s2);
                    outCount = 1; o0tile = bm2; o0mask = mid; return;
                }
                if (bm1 < bm2 && bm1 < bm3) {
                    outCount = 2; o0tile = bm2; o0mask = (2 - s1) * 3 + s2; o1tile = bm3; o1mask = 18 + (2 - s2) * 3 + s3; return;
                }
                if (bm2 < bm1 && bm2 < bm3) {
                    outCount = 2; o0tile = bm1; o0mask = 36 + (2 - s3) * 3 + s1; o1tile = bm3; o1mask = 18 + (2 - s2) * 3 + s3; return;
                }
                outCount = 2; o0tile = bm1; o0mask = 36 + (2 - s3) * 3 + s1; o1tile = bm2; o1mask = (2 - s1) * 3 + s2;
            };
            auto overlays_prepare2 = [](int bm1, int bm2, int bm3, int s1, int s2, int s3,
                                         int& outCount, int& o0tile, int& o0mask, int& o1tile, int& o1mask){
                outCount = 0; o0tile = o1tile = -1; o0mask = o1mask = -1;
                if (bm1 == bm2) {
                    int mid = 54 + ((bm3 < bm2) ? (27 + (2 - s2) * 3 + s3) : (18 + (2 - s2) * 3 + s3));
                    outCount = 1; o0tile = bm3; o0mask = mid; return;
                }
                if (bm2 == bm3) {
                    int mid = 54 + ((bm1 < bm3) ? (45 + (2 - s3) * 3 + s1) : (36 + (2 - s3) * 3 + s1));
                    outCount = 1; o0tile = bm1; o0mask = mid; return;
                }
                if (bm1 == bm3) {
                    int mid = 54 + ((bm2 < bm3) ? (9 + (2 - s1) * 3 + s2) : ((2 - s1) * 3 + s2));
                    outCount = 1; o0tile = bm2; o0mask = mid; return;
                }
                if (bm1 < bm2 && bm1 < bm3) {
                    outCount = 2; o0tile = bm2; o0mask = 54 + (2 - s1) * 3 + s2; o1tile = bm3; o1mask = 54 + 18 + (2 - s2) * 3 + s3; return;
                }
                if (bm2 < bm1 && bm2 < bm3) {
                    outCount = 2; o0tile = bm1; o0mask = 54 + 36 + (2 - s3) * 3 + s1; o1tile = bm3; o1mask = 54 + 18 + (2 - s2) * 3 + s3; return;
                }
                outCount = 2; o0tile = bm1; o0mask = 54 + 36 + (2 - s3) * 3 + s1; o1tile = bm2; o1mask = 54 + (2 - s1) * 3 + s2;
            };

            // GPU indexed pipeline path (no fallback)
            if (gAtlasIndexTex && gPaletteTex) {
                // Triangle 1 (legacy TriType 0): top-left-right, PrepareIntersection1 with bm1=Tex2, bm2=Tex1, bm3=Tex3
                {
                    const int Tex1 = static_cast<int>(gTexIdx[v00]);
                    const int Tex2 = static_cast<int>(gTexIdx[v01]);
                    const int Tex3 = static_cast<int>(gTexIdx[v11]);
                    const float tint = (lTop + lLeft + lRight) * (1.0f / 3.0f);
                    int x0 = 0, y0 = 0;
                    compute_mask_origin_case0(ix, iy, x0, y0);
                    int ovN = 0, o0t = -1, o0m = -1, o1t = -1, o1m = -1;
                    overlays_prepare1(Tex2, Tex1, Tex3, s_case0_s1, s_case0_s2, s_case0_s3, ovN, o0t, o0m, o1t, o1m);
                    // canonical tile UVs
                    const float tu1 = 0.5f, tv1 = 0.0f; // top
                    const float tu2 = 0.0f, tv2 = 0.5f; // left
                    const float tu3 = 1.0f, tv3 = 0.5f; // right
                    engine_core::render2d::draw_terrain_triangle_indexed(
                        static_cast<float>(topX),  static_cast<float>(topY),  tu1, tv1,
                        static_cast<float>(leftX), static_cast<float>(leftY), tu2, tv2,
                        static_cast<float>(rightX),static_cast<float>(rightY),tu3, tv3,
                        Tex2,
                        x0, y0,
                        0,
                        ovN,
                        o0t, o0m,
                        o1t, o1m,
                        tint, tint, tint, 1.0f);
                }

                // Triangle 2 (legacy TriType 1): bottom-right-left, PrepareIntersection2 with bm1=Tex1, bm2=Tex3, bm3=Tex2
                {
                    const int Tex1 = static_cast<int>(gTexIdx[v00]);
                    const int Tex2 = static_cast<int>(gTexIdx[v01]);
                    const int Tex3 = static_cast<int>(gTexIdx[v11]);
                    const float tint = (lBot + lLeft + lRight) * (1.0f / 3.0f);
                    int x0 = 0, y0 = 0;
                    compute_mask_origin_case1(ix, iy, x0, y0);
                    int ovN = 0, o0t = -1, o0m = -1, o1t = -1, o1m = -1;
                    overlays_prepare2(Tex1, Tex3, Tex2, s_case1_s1, s_case1_s2, s_case1_s3, ovN, o0t, o0m, o1t, o1m);
                    // canonical tile UVs
                    const float tu1 = 0.5f, tv1 = 1.0f; // bottom
                    const float tu2 = 1.0f, tv2 = 0.5f; // right
                    const float tu3 = 0.0f, tv3 = 0.5f; // left
                    engine_core::render2d::draw_terrain_triangle_indexed(
                        static_cast<float>(botX),  static_cast<float>(botY),  tu1, tv1,
                        static_cast<float>(rightX),static_cast<float>(rightY),tu2, tv2,
                        static_cast<float>(leftX), static_cast<float>(leftY), tu3, tv3,
                        Tex1,
                        x0, y0,
                        -31,
                        ovN,
                        o0t, o0m,
                        o1t, o1m,
                        tint, tint, tint, 1.0f);
                }
            }
        }
    }
}

void update_region(int vx0, int vy0, int vx1, int vy1) {
    // No cached tiles to rebuild yet; left intentionally empty.
    // Region parameters are accepted to mirror legacy hooks and to allow future caching.
    (void)vx0; (void)vy0; (void)vx1; (void)vy1;
}

} // namespace world_render::terrain


