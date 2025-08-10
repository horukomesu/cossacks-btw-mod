#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

namespace legacy::sprite_cache {

// Draws an RLC subimage from path:index at pixel position (x,y).
// Returns false if the asset could not be loaded/decoded.
bool draw_rlc(const std::string& path, int subIndex, int x, int y);

// Query size of an RLC subimage; returns false if unavailable
bool query_size(const std::string& path, int subIndex, int& outW, int& outH);

// Draw with a palette remap table id (0..7; 0 identity). For now 1..7 map to pal1..pal7.
bool draw_rlc_pal(const std::string& path, int subIndex, int x, int y, int paletteId);

// Clears caches and releases GL textures
void clear();

// GP support
// Draw a GP frame from a resource stem (e.g., "Interface\\Main_Menu") at (x,y).
// paletteId: -1 = white RGB + alpha, 0 = identity palette, 1..7 = nation remap tables
bool draw_gp(const std::string& stem, int frameIndex, int x, int y);
bool draw_gp_pal(const std::string& stem, int frameIndex, int x, int y, int paletteId);
// Query the size of a decoded GP frame (aggregate over sub-chunks)
bool query_size_gp(const std::string& stem, int frameIndex, int& outW, int& outH);

} // namespace legacy::sprite_cache
