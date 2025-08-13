#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace legacy { namespace assets {

// Describes one animation sequence of a unit for a given state and angle bucket
struct AnimFrames {
    std::string gpStem;               // e.g., "Units\\Infantry\\musket"
    std::vector<int> frameIndices;    // frames for the sequence
    int dx{0};                        // optional anchor override (pixels)
    int dy{0};
};

// 8-way directional set of animations for a single state (idle/move/attack/death)
struct Dir8Set {
    AnimFrames dirs[8]; // N, NE, E, SE, S, SW, W, NW
};

// High-level unit asset definition (subset sufficient for rendering)
struct UnitGfx {
    Dir8Set stand;
    Dir8Set move;    // flattened from MotionL/MotionR
    Dir8Set attack;  // primary
    Dir8Set death;
    float shadowRadiusX{16.0f};
    float shadowRadiusY{8.0f};
};

// Registry API
const UnitGfx* get_unit_gfx(const std::string& unitId);
void register_unit_gfx(const std::string& unitId, UnitGfx gfx);

// Example: register a basic infantry set using GP stems identical to original naming
// Stems and frames should be ported from original NewAnimation tables.
void register_example_infantry();

// Map RealDir (0..255) to 8-way index 0..7 as in original
inline int dir256_to_dir8(int realDir) { return ((realDir + 16) & 255) >> 5; }

} } // namespace legacy::assets


