#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace world_render::terrain {

// Build GL textures from original tiles3.bmp atlas.
// Returns number of tile textures created.
int initialize_tiles();

// Set the current map data pointers used for rendering.
// heights: (vertInLine * maxTH) int16 values, tex: same length bytes.
void set_map_data(const int16_t* heights, const uint8_t* tex, int vertInLine, int maxTH);

// Submit terrain quads for a screen frame using current camera (WorldRender handles sorting/draw)
void submit_frame();

  // Notify terrain that a small vertex tile region [vx0..vx1), [vy0..vy1) changed (heights/tex indices).
  // Our renderer fetches from arrays each frame, so this is currently a no-op, but kept for parity
  // with legacy local regeneration hooks.
  void update_region(int vx0, int vy0, int vx1, int vy1);

  // Provide section map (TCES) for tile seam mask selection;
  // maxSector and maxTH must match values used to compute dimensions (SectInLine = maxSector*6).
  void set_sect_map(const std::uint8_t* sectMap, int maxSector, int maxTH);

  // Returns true if terrain has valid map arrays bound
  bool is_ready();

} // namespace world_render::terrain


