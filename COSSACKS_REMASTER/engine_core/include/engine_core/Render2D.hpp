#pragma once

#include <cstdint>

namespace engine_core::render2d {

bool initialize();
void shutdown();

void begin_frame(int framebufferWidth, int framebufferHeight);
void end_frame();

// Draw a solid color rectangle at pixel coords
void draw_rect(float x, float y, float w, float h, float r, float g, float b, float a);

// Draw a textured quad (texture must be GL texture name)
void draw_textured_quad(unsigned int texture, float x, float y, float w, float h);

// Draw a textured quad multiplied by a tint color (r,g,b,a).
// r,g,b,a are in [0..1]. Use (1,1,1,1) to draw unmodified texture.
void draw_textured_quad_tinted(unsigned int texture, float x, float y, float w, float h,
                               float r, float g, float b, float a);

// Draw a sub-rectangle of a texture with custom UVs [u0,v0]-[u1,v1], multiplied by tint
void draw_textured_quad_region_tinted(unsigned int texture, float x, float y, float w, float h,
                                      float u0, float v0, float u1, float v1,
                                      float r, float g, float b, float a);

// Draw a single textured triangle with per-vertex UVs and tint
void draw_textured_triangle_tinted(unsigned int texture,
                                   float x1, float y1, float u1, float v1,
                                   float x2, float y2, float u2, float v2,
                                   float x3, float y3, float u3, float v3,
                                   float r, float g, float b, float a);

// Terrain-specific GPU pipeline (indexed tiles + palette + masks/transitions)
// Resources must be set once via set_terrain_gpu_resources before drawing.
void set_terrain_gpu_resources(unsigned int atlasIndexTex,
                               unsigned int paletteTex,
                               unsigned int trans4Tex,
                               unsigned int trans8Tex,
                               unsigned int maskATexArray,
                               unsigned int maskBTexArray,
                               unsigned int maskCTexArray,
                               unsigned int maskDTexArray,
                               int atlasWidth,
                               int atlasHeight,
                               int maskALayers,
                               int maskBLayers,
                               int maskCLayers,
                               int maskDLayers);

// Draw a single terrain triangle using indexed atlas and GPU-side masking.
// UVs are canonical within a 64x64 tile: top (0.5,0.0), left (0.0,0.5), right (1.0,0.5), bottom (0.5,1.0).
// baseTileIndex: tile index to sample for base fill.
// maskOriginX/Y: x0,y0 offsets in [0..63] used for mask addressing.
// numOverlays: 0, 1 or 2; overlay tiles and mask ids specify overlay operations; pass -1 if unused.
void draw_terrain_triangle_indexed(float x1, float y1, float u1, float v1,
                                   float x2, float y2, float u2, float v2,
                                   float x3, float y3, float u3, float v3,
                                   int baseTileIndex,
                                   int maskOriginX, int maskOriginY,
                                   int baseYOffset,
                                   int numOverlays,
                                   int overlay0TileIndex, int overlay0MaskId,
                                   int overlay1TileIndex, int overlay1MaskId,
                                   float tintR, float tintG, float tintB, float tintA);

} // namespace engine_core::render2d
