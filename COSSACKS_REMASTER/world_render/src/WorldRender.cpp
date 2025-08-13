#include "world_render/WorldRender.hpp"

#include "engine_core/Render2D.hpp"
#include "legacy/SpriteCache.hpp"
#include "engine_core/Texture.hpp"
#include "legacy/Globals_compat.hpp"
#include "legacy/UnitAssets_compat.hpp"
#include <glad/gl.h>
#include "world_render/Decor.hpp"

#include <algorithm>
#include <cmath>
#include <vector>
#include <string>
#include <cstring>

namespace world_render {

namespace {
struct RenderItem {
    // screen-space rectangle
    float x;
    float y;
    float w;
    float h;
    unsigned int texture;
    float zOrder; // for stable sorting
    int layer{static_cast<int>(Layer::Units)};
    float tintR{1.0f}, tintG{1.0f}, tintB{1.0f}, tintA{1.0f};
};

static std::vector<RenderItem> gItems;
static bool gClipEnabled = false;
static int gClipX = 0, gClipY = 0, gClipW = 0, gClipH = 0;
static int gFbW = 0, gFbH = 0;
static int gCamTileX = 0, gCamTileY = 0;
static int gCamPixX = 0, gCamPixY = 0;
static engine_core::texture::TextureHandle gShadowTex = 0;
static engine_core::texture::TextureHandle gFogTex = 0;
static int gFogW = 0, gFogH = 0;

static void ensure_shadow_texture() {
    if (gShadowTex) return;
    // Generate a soft circular alpha mask (256x256), black color with soft edges
    const int w = 256;
    const int h = 256;
    std::vector<unsigned char> rgba(static_cast<size_t>(w) * static_cast<size_t>(h) * 4u);
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            const float nx = (static_cast<float>(x) + 0.5f) / static_cast<float>(w);
            const float ny = (static_cast<float>(y) + 0.5f) / static_cast<float>(h);
            const float dx = (nx - 0.5f) * 2.0f;
            const float dy = (ny - 0.5f) * 2.0f;
            const float r = std::sqrt(dx*dx + dy*dy);
            // Soft edge: full alpha at r<=0.6, then falloff to 0 at r>=1.0
            float a = 0.0f;
            if (r <= 0.6f) a = 1.0f; else if (r < 1.0f) a = (1.0f - (r - 0.6f) / 0.4f);
            const unsigned char A = static_cast<unsigned char>(std::clamp(a, 0.0f, 1.0f) * 255.0f + 0.5f);
            const size_t idx = (static_cast<size_t>(y) * static_cast<size_t>(w) + static_cast<size_t>(x)) * 4u;
            rgba[idx + 0] = 0;
            rgba[idx + 1] = 0;
            rgba[idx + 2] = 0;
            rgba[idx + 3] = A;
        }
    }
    gShadowTex = engine_core::texture::create_texture_rgba(w, h, rgba.data());
}

inline void push_item(const RenderItem& it) { gItems.emplace_back(it); }

// Simple isometric transform matching original tile metrics (32x16 diamond), with optional elevation h in pixels.
inline void world_to_screen_local(int tileX, int tileY, int elevHalfPixels, int& outX, int& outY) {
    // Original used (x - y) * 16 horizontally and (x + y) * 8 vertically, adjusted by camera offsets.
    const int tx = tileX - gCamTileX;
    const int ty = tileY - gCamTileY;
    const int sx = (tx - ty) * 16 + gCamPixX;
    const int sy = (tx + ty) * 8 - elevHalfPixels + gCamPixY;
    // Apply global menu offsets to center content (reuse legacy globals to keep consistent with UI centering)
    outX = sx + legacy::globals::menu_x_off;
    outY = sy + legacy::globals::menu_y_off;
}

} // namespace

void begin_frame() {
    gItems.clear();
}

// Render helper: draw a unit animation frame by logical unitId and realDir (0..255)
void submit_unit_frame(const std::string& unitId, const char* state, int realDir,
                       int tileX, int tileY, int elevHalfPixels, int frameTick,
                       int paletteId, Layer layer) {
    const legacy::assets::UnitGfx* gfx = legacy::assets::get_unit_gfx(unitId);
    if (!gfx) return;
    const legacy::assets::Dir8Set* set = nullptr;
    if (std::strcmp(state, "stand") == 0) set = &gfx->stand;
    else if (std::strcmp(state, "move") == 0) set = &gfx->move;
    else if (std::strcmp(state, "attack") == 0) set = &gfx->attack;
    else if (std::strcmp(state, "death") == 0) set = &gfx->death;
    else return;
    const int dir8 = legacy::assets::dir256_to_dir8(realDir) & 7;
    const legacy::assets::AnimFrames& af = set->dirs[dir8];
    if (af.gpStem.empty() || af.frameIndices.empty()) return;
    const int idx = af.frameIndices[static_cast<size_t>(frameTick) % af.frameIndices.size()];
    submit_sprite_gp(af.gpStem.c_str(), idx, tileX, tileY, elevHalfPixels, 0.0f, paletteId, layer, 1.0f, 1.0f, 1.0f, 1.0f);
}

void submit_sprite_gp(const char* stem, int frameIndex, int worldX, int worldY, int elevHalfPixels,
                      float zBias, int paletteId, Layer layer, float tintR, float tintG, float tintB, float tintA) {
    unsigned int tex = 0; int w = 0, h = 0, dx = 0, dy = 0;
    if (!legacy::sprite_cache::query_texture_gp(stem, frameIndex, tex, w, h, dx, dy, paletteId)) return;
    int sx = 0, sy = 0; world_to_screen_local(worldX, worldY, elevHalfPixels, sx, sy);
    RenderItem it;
    it.x = static_cast<float>(sx - dx);
    it.y = static_cast<float>(sy - dy);
    it.w = static_cast<float>(w);
    it.h = static_cast<float>(h);
    it.texture = tex;
    // Sort key: by screen Y for isometric correct layering (objects lower on screen drawn last)
    it.zOrder = static_cast<float>(sy) + zBias;
    it.tintR = tintR; it.tintG = tintG; it.tintB = tintB; it.tintA = tintA;
    it.layer = static_cast<int>(layer);
    push_item(it);
}

void draw_shadow_ellipse(float centerX, float centerY, float radiusX, float radiusY, float alpha) {
    ensure_shadow_texture();
    RenderItem it;
    it.x = centerX - radiusX;
    it.y = centerY - radiusY;
    it.w = radiusX * 2.0f;
    it.h = radiusY * 2.0f;
    it.texture = gShadowTex;
    it.zOrder = centerY - 1.0f; // slightly underneath the sprite
    it.tintR = 1.0f; it.tintG = 1.0f; it.tintB = 1.0f; it.tintA = std::clamp(alpha, 0.0f, 1.0f);
    push_item(it);
}

void flush() {
    // Apply scissor if requested
    if (gClipEnabled && gClipW > 0 && gClipH > 0 && gFbW > 0 && gFbH > 0) {
        // Convert top-left coords to OpenGL bottom-left scissor box
        const int scX = std::max(0, gClipX);
        const int scY = std::max(0, gFbH - (gClipY + gClipH));
        const int scW = std::max(0, std::min(gClipW, gFbW - scX));
        const int scH = std::max(0, std::min(gClipH, gFbH - scY));
        glEnable(GL_SCISSOR_TEST);
        glScissor(scX, scY, scW, scH);
    }
    // Sort by layer, then by zOrder ascending so later drawn items appear on top
    std::stable_sort(gItems.begin(), gItems.end(), [](const RenderItem& a, const RenderItem& b){
        if (a.layer != b.layer) return a.layer < b.layer;
        return a.zOrder < b.zOrder;
    });
    for (const RenderItem& it : gItems) {
        if (it.texture != 0u) {
            engine_core::render2d::draw_textured_quad_tinted(it.texture, it.x, it.y, it.w, it.h, it.tintR, it.tintG, it.tintB, it.tintA);
        } else {
            // Fallback: solid rect for simple primitives (shadows/overlays). Alpha is respected.
            engine_core::render2d::draw_rect(it.x, it.y, it.w, it.h, it.tintR, it.tintG, it.tintB, it.tintA);
        }
    }
    if (gClipEnabled) {
        glDisable(GL_SCISSOR_TEST);
    }
}

void set_clip_rect(int x, int y, int width, int height) {
    gClipX = x; gClipY = y; gClipW = width; gClipH = height; gClipEnabled = (width > 0 && height > 0);
}

void set_framebuffer_size(int width, int height) {
    gFbW = width; gFbH = height;
}

void set_camera(int tileOffsetX, int tileOffsetY, int pixelOffsetX, int pixelOffsetY) {
    gCamTileX = tileOffsetX; gCamTileY = tileOffsetY; gCamPixX = pixelOffsetX; gCamPixY = pixelOffsetY;
}

void world_to_screen(int tileX, int tileY, int elevHalfPixels, int& outX, int& outY) {
    world_to_screen_local(tileX, tileY, elevHalfPixels, outX, outY);
}

void set_fog_screen_mask(const unsigned char* alphaMask, int width, int height) {
    if (!alphaMask || width <= 0 || height <= 0) {
        if (gFogTex) {
            engine_core::texture::destroy_texture(gFogTex);
            gFogTex = 0; gFogW = gFogH = 0;
        }
        return;
    }
    std::vector<unsigned char> rgba(static_cast<size_t>(width) * static_cast<size_t>(height) * 4u);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const unsigned char a = alphaMask[static_cast<size_t>(y) * static_cast<size_t>(width) + static_cast<size_t>(x)];
            const size_t idx = (static_cast<size_t>(y) * static_cast<size_t>(width) + static_cast<size_t>(x)) * 4u;
            rgba[idx + 0] = 0;
            rgba[idx + 1] = 0;
            rgba[idx + 2] = 0;
            rgba[idx + 3] = a;
        }
    }
    if (gFogTex) {
        engine_core::texture::destroy_texture(gFogTex);
    }
    gFogTex = engine_core::texture::create_texture_rgba(width, height, rgba.data());
    gFogW = width; gFogH = height;
}

void draw_fog_fullscreen(float intensity) {
    if (!gFogTex || gFogW <= 0 || gFogH <= 0 || gFbW <= 0 || gFbH <= 0) return;
    const float tintA = std::clamp(intensity, 0.0f, 1.0f);
    // Draw scaled to framebuffer size. Maintain aspect ratio by stretching.
    engine_core::render2d::draw_textured_quad_tinted(gFogTex, 0.0f, 0.0f,
                                                     static_cast<float>(gFbW), static_cast<float>(gFbH),
                                                     1.0f, 1.0f, 1.0f, tintA);
}

} // namespace world_render


