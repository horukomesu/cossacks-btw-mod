#pragma once

namespace world_render {

enum class Layer : int {
  Terrain = 0,
  TerrainDecor = 1,
  StaticObjects = 2,
  Buildings = 3,
  Units = 4,
  Projectiles = 5,
  Overlays = 6,
  HUD = 7
};

// Begin collecting world render items for this frame
void begin_frame();

// Submit a GP sprite in world tile coordinates (tileX, tileY) with optional elevation in half-pixels.
// zBias allows fine ordering adjustments; paletteId matches legacy nation remaps; tint multiplies the sprite.
void submit_sprite_gp(const char* stem, int frameIndex,
                      int tileX, int tileY, int elevHalfPixels,
                      float zBias,
                      int paletteId,
                      Layer layer = Layer::Units,
                      float tintR = 1.0f, float tintG = 1.0f, float tintB = 1.0f, float tintA = 1.0f);

// Draw a simple shadow ellipse (temporary; can be replaced by a textured ellipse)
void draw_shadow_ellipse(float centerX, float centerY, float radiusX, float radiusY, float alpha);

// Render all queued items respecting isometric sort order
void flush();

// Optional: define a clipping rectangle in screen pixels (top-left origin).
// Call with width/height <= 0 to clear clipping.
void set_clip_rect(int x, int y, int width, int height);

// Must be called by engine per-frame for proper scissor translation (top-left to OpenGL bottom-left)
void set_framebuffer_size(int width, int height);

// Set camera offset for world-to-screen transform.
// tileOffset shifts world coordinates by tiles before projection; pixelOffset adds pixel shift after projection.
void set_camera(int tileOffsetX, int tileOffsetY, int pixelOffsetX, int pixelOffsetY);

// Utility to compute screen position of a world tile with elevation for external modules (e.g., terrain)
// Returns (sx, sy) in screen pixels (top-left origin) via out parameters.
void world_to_screen(int tileX, int tileY, int elevHalfPixels, int& outX, int& outY);

// Fog of war: provide a precomputed screen-space alpha mask (w x h, 8-bit alpha per pixel) to be drawn over the world.
// Pass nullptr to clear current mask.
void set_fog_screen_mask(const unsigned char* alphaMask, int width, int height);
// Draw the fog mask fullscreen (scaled to framebuffer) with black tint.
void draw_fog_fullscreen(float intensity = 1.0f);

// Utilities to submit by GP resource stem (helper for asset tables)
inline void submit_gp_stem(const char* stem, int frameIndex, int tileX, int tileY, int elevHalfPixels,
                           Layer layer, float zBias = 0.0f, int paletteId = 0,
                           float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) {
    submit_sprite_gp(stem, frameIndex, tileX, tileY, elevHalfPixels, zBias, paletteId, layer, r, g, b, a);
}

} // namespace world_render


