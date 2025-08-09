#include "legacy/SpriteCache.hpp"

#include "resource_io/RLC.hpp"
#include "engine_core/Texture.hpp"
#include "engine_core/Render2D.hpp"
#include "resource_io/Palettes.hpp"
#include "resource_io/ColorPalette.hpp"

#include <tuple>
#include <unordered_map>

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
    if (paletteId > 0) {
        const uint8_t* indexMap = resource_io::palettes::get_table(paletteId);
        // Base color palette (agew_1.pal or identity if not loaded)
        const uint8_t* baseRGBA = resource_io::color_palette::get_rgba256x4();
        // Build remapped RGBA palette using index map
        uint8_t palRGBA[256 * 4];
        for (int i = 0; i < 256; ++i) {
            const uint8_t srcIdx = indexMap[i];
            palRGBA[i * 4 + 0] = baseRGBA[srcIdx * 4 + 0];
            palRGBA[i * 4 + 1] = baseRGBA[srcIdx * 4 + 1];
            palRGBA[i * 4 + 2] = baseRGBA[srcIdx * 4 + 2];
            palRGBA[i * 4 + 3] = baseRGBA[srcIdx * 4 + 3];
        }
        resource_io::rlc::decode_subimage_rgba_with_palette(tbl, subIndex, palRGBA, rgba);
    } else {
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

} // namespace legacy::sprite_cache
