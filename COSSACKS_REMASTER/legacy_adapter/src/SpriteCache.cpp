#include "legacy/SpriteCache.hpp"

#include "resource_io/RLC.hpp"
#include "resource_io/GP.hpp"
#include "engine_core/Texture.hpp"
#include "engine_core/Render2D.hpp"
#include "resource_io/Palettes.hpp"
#include "resource_io/ColorPalette.hpp"

#include <tuple>
#include <unordered_map>
#include <string>
#include <cstring>
#include <algorithm>

namespace legacy::sprite_cache {

struct Key {
    std::string path;
    int subIndex;
    int paletteId; // to differentiate cached textures per palette
    bool operator==(const Key& o) const { return subIndex == o.subIndex && path == o.path && paletteId == o.paletteId; }
};
struct KeyHasher {
    size_t operator()(const Key& k) const noexcept {
        return std::hash<std::string>()(k.path) ^ (static_cast<size_t>(k.subIndex) * 1315423911u) ^ (static_cast<size_t>(k.paletteId) * 2654435761u);
    }
};

struct Entry {
    engine_core::texture::TextureHandle tex{0};
    int w{0};
    int h{0};
    // When non-negative, indicates this texture was created with a specific palette remap id
    int paletteId{-1};
    int anchorDx{0};
    int anchorDy{0};
};

static std::unordered_map<Key, Entry, KeyHasher> gCache;

static bool ensure_texture(const std::string& path, int subIndex, int paletteId, Entry& out) {
    resource_io::rlc::RLCTableData tbl;
    if (!resource_io::rlc::load_rlc(path, tbl)) return false;
    int w = 0, h = 0;
    if (!resource_io::rlc::get_subimage_size(tbl, subIndex, w, h)) return false;
    int dx = 0, dy = 0;
    resource_io::rlc::get_subimage_anchor(tbl, subIndex, dx, dy);
    std::vector<uint8_t> rgba;
    if (paletteId >= 0) {
        // Use base palette for paletteId == 0; apply nation remap for paletteId > 0
        const uint8_t* baseRGBA = resource_io::color_palette::get_rgba256x4();
        uint8_t palRGBA[256 * 4];
        if (paletteId == 0) {
            // Identity mapping (no remap)
            std::memcpy(palRGBA, baseRGBA, sizeof(palRGBA));
        } else {
            const uint8_t* indexMap = resource_io::palettes::get_table(paletteId);
            for (int i = 0; i < 256; ++i) {
                const uint8_t srcIdx = indexMap[i];
                palRGBA[i * 4 + 0] = baseRGBA[srcIdx * 4 + 0];
                palRGBA[i * 4 + 1] = baseRGBA[srcIdx * 4 + 1];
                palRGBA[i * 4 + 2] = baseRGBA[srcIdx * 4 + 2];
                palRGBA[i * 4 + 3] = baseRGBA[srcIdx * 4 + 3];
            }
        }
        resource_io::rlc::decode_subimage_rgba_with_palette(tbl, subIndex, palRGBA, rgba);
    } else {
        // paletteId < 0: legacy white RGB + alpha
        resource_io::rlc::decode_subimage_rgba(tbl, subIndex, rgba);
    }
    out.tex = engine_core::texture::create_texture_rgba(w, h, rgba.data());
    out.w = w;
    out.h = h;
    out.paletteId = paletteId;
    out.anchorDx = dx;
    out.anchorDy = dy;
    return true;
}

static Entry* get_or_create(const std::string& path, int subIndex, int paletteId) {
    Key key{path, subIndex, paletteId};
    auto it = gCache.find(key);
    if (it == gCache.end()) {
        Entry e;
        if (!ensure_texture(path, subIndex, paletteId, e)) return nullptr;
        it = gCache.emplace(std::move(key), e).first;
    }
    return &it->second;
}

bool draw_rlc(const std::string& path, int subIndex, int x, int y) {
    Key key{path, subIndex, -1};
    auto it = gCache.find(key);
    if (it == gCache.end()) {
        Entry e;
        if (!ensure_texture(path, subIndex, /*paletteId*/ -1, e)) return false;
        it = gCache.emplace(std::move(key), e).first;
    }
    engine_core::render2d::draw_textured_quad(it->second.tex, static_cast<float>(x - it->second.anchorDx), static_cast<float>(y - it->second.anchorDy), static_cast<float>(it->second.w), static_cast<float>(it->second.h));
    return true;
}

bool query_size(const std::string& path, int subIndex, int& outW, int& outH) {
    Key key{path, subIndex};
    auto it = gCache.find(key);
    if (it != gCache.end()) {
        outW = it->second.w;
        outH = it->second.h;
        return true;
    }
    resource_io::rlc::RLCTableData tbl;
    if (!resource_io::rlc::load_rlc(path, tbl)) return false;
    return resource_io::rlc::get_subimage_size(tbl, subIndex, outW, outH);
}

bool draw_rlc_pal(const std::string& path, int subIndex, int x, int y, int paletteId) {
    Entry* e = get_or_create(path, subIndex, paletteId);
    if (!e) return false;
    engine_core::render2d::draw_textured_quad(e->tex, static_cast<float>(x - e->anchorDx), static_cast<float>(y - e->anchorDy), static_cast<float>(e->w), static_cast<float>(e->h));
    return true;
}

void clear() {
    for (auto& [k, e] : gCache) {
        engine_core::texture::destroy_texture(e.tex);
    }
    gCache.clear();
}

// ---------- GP support ----------
namespace {
struct GPKey {
    std::string stem; // base name without extension
    int frameIndex;
    int paletteId;
    bool operator==(const GPKey& o) const { return frameIndex == o.frameIndex && paletteId == o.paletteId && stem == o.stem; }
};
struct GPKeyHasher {
    size_t operator()(const GPKey& k) const noexcept {
        return std::hash<std::string>()(k.stem) ^ (static_cast<size_t>(k.frameIndex) * 16777619u) ^ (static_cast<size_t>(k.paletteId) * 2166136261u);
    }
};

static std::unordered_map<GPKey, Entry, GPKeyHasher> gGpCache;

static bool ensure_gp_texture(const std::string& stem, int frameIndex, int paletteId, Entry& out) {
    resource_io::gp::GPFile gp;
    if (!resource_io::gp::load_gp(stem, gp)) return false;
    resource_io::gp::GPFrameIndices fr;
    if (!resource_io::gp::decode_frame_indices(gp, frameIndex, fr)) return false;
    // Build RGBA from indices using palettes
    std::vector<uint8_t> rgba;
    rgba.resize(static_cast<size_t>(fr.width) * static_cast<size_t>(fr.height) * 4u);
    const uint8_t* baseRGBA = resource_io::color_palette::get_rgba256x4();
    uint8_t palRGBA[256 * 4];
    const uint8_t* palettePtr = nullptr;
    if (paletteId >= 0) {
        if (paletteId == 0) {
            std::memcpy(palRGBA, baseRGBA, sizeof(palRGBA));
        } else {
            const uint8_t* indexMap = resource_io::palettes::get_table(paletteId);
            for (int i = 0; i < 256; ++i) {
                const uint8_t srcIdx = indexMap[i];
                palRGBA[i * 4 + 0] = baseRGBA[srcIdx * 4 + 0];
                palRGBA[i * 4 + 1] = baseRGBA[srcIdx * 4 + 1];
                palRGBA[i * 4 + 2] = baseRGBA[srcIdx * 4 + 2];
                palRGBA[i * 4 + 3] = baseRGBA[srcIdx * 4 + 3];
            }
        }
        palettePtr = palRGBA;
    }
    for (int y = 0; y < fr.height; ++y) {
        for (int x = 0; x < fr.width; ++x) {
            const uint8_t idx = fr.indices[static_cast<size_t>(y) * static_cast<size_t>(fr.width) + static_cast<size_t>(x)];
            uint8_t* d = rgba.data() + (static_cast<size_t>(y) * static_cast<size_t>(fr.width) + static_cast<size_t>(x)) * 4u;
            if (idx == 0) {
                d[0] = d[1] = d[2] = d[3] = 0;
            } else if (palettePtr) {
                d[0] = palettePtr[idx * 4 + 0];
                d[1] = palettePtr[idx * 4 + 1];
                d[2] = palettePtr[idx * 4 + 2];
                d[3] = 255;
            } else {
                // White RGB + alpha based on index presence
                d[0] = d[1] = d[2] = 255;
                d[3] = 255;
            }
        }
    }
    out.tex = engine_core::texture::create_texture_rgba(fr.width, fr.height, rgba.data());
    out.w = fr.width;
    out.h = fr.height;
    out.paletteId = paletteId;
    out.anchorDx = -fr.minx; // shift so that (x,y) is at original GP frame origin
    out.anchorDy = -fr.miny;
    return true;
}
} // namespace

static Entry* get_or_create_gp(const std::string& stem, int frameIndex, int paletteId) {
    GPKey key{stem, frameIndex, paletteId};
    auto it = gGpCache.find(key);
    if (it == gGpCache.end()) {
        Entry e;
        if (!ensure_gp_texture(stem, frameIndex, paletteId, e)) return nullptr;
        it = gGpCache.emplace(std::move(key), e).first;
    }
    return &it->second;
}

bool draw_gp(const std::string& stem, int frameIndex, int x, int y) {
    GPKey key{stem, frameIndex, -1};
    auto it = gGpCache.find(key);
    if (it == gGpCache.end()) {
        Entry e;
        if (!ensure_gp_texture(stem, frameIndex, /*paletteId*/ -1, e)) return false;
        it = gGpCache.emplace(std::move(key), e).first;
    }
    engine_core::render2d::draw_textured_quad(it->second.tex, static_cast<float>(x - it->second.anchorDx), static_cast<float>(y - it->second.anchorDy), static_cast<float>(it->second.w), static_cast<float>(it->second.h));
    return true;
}

bool draw_gp_pal(const std::string& stem, int frameIndex, int x, int y, int paletteId) {
    Entry* e = get_or_create_gp(stem, frameIndex, paletteId);
    if (!e) return false;
    engine_core::render2d::draw_textured_quad(e->tex, static_cast<float>(x - e->anchorDx), static_cast<float>(y - e->anchorDy), static_cast<float>(e->w), static_cast<float>(e->h));
    return true;
}

bool query_size_gp(const std::string& stem, int frameIndex, int& outW, int& outH) {
    GPKey key{stem, frameIndex, 0};
    auto it = gGpCache.find(key);
    if (it != gGpCache.end()) {
        outW = it->second.w;
        outH = it->second.h;
        return true;
    }
    resource_io::gp::GPFile gp;
    if (!resource_io::gp::load_gp(stem, gp)) return false;
    int minx = 0, miny = 0, maxx = 0, maxy = 0;
    if (!resource_io::gp::compute_frame_bounds(gp, frameIndex, minx, miny, maxx, maxy)) return false;
    outW = std::max(1, maxx - minx);
    outH = std::max(1, maxy - miny);
    return true;
}

bool query_texture_gp(const std::string& stem, int frameIndex, unsigned int& outTex,
                      int& outW, int& outH, int& outDx, int& outDy, int paletteId) {
    Entry* e = get_or_create_gp(stem, frameIndex, paletteId);
    if (!e) return false;
    outTex = e->tex;
    outW = e->w;
    outH = e->h;
    outDx = e->anchorDx;
    outDy = e->anchorDy;
    return true;
}

} // namespace legacy::sprite_cache
