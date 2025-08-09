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

} // namespace legacy::sprite_cache
