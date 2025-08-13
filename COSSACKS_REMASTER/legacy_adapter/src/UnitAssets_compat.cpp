#include "legacy/UnitAssets_compat.hpp"

#include <unordered_map>
#include <utility>
#include <string>

namespace legacy { namespace assets {

static std::unordered_map<std::string, UnitGfx> g_registry;

const UnitGfx* get_unit_gfx(const std::string& unitId) {
    auto it = g_registry.find(unitId);
    if (it == g_registry.end()) return nullptr;
    return &it->second;
}

void register_unit_gfx(const std::string& unitId, UnitGfx gfx) {
    g_registry[unitId] = std::move(gfx);
}

void register_example_infantry() {
    UnitGfx gfx;
    // NOTE: The stems and frames must match original GP packs. Placeholder names illustrate structure.
    // Stand: use frame 0 for all directions (to be replaced with actual frames per dir)
    for (int d = 0; d < 8; ++d) {
        gfx.stand.dirs[d].gpStem = "Units\\Infantry\\musket";
        gfx.stand.dirs[d].frameIndices = {0};
    }
    // Move: simple 4-frame loop (replace with MotionL/MotionR interleave mapping)
    for (int d = 0; d < 8; ++d) {
        gfx.move.dirs[d].gpStem = "Units\\Infantry\\musket";
        gfx.move.dirs[d].frameIndices = {1,2,3,4};
    }
    // Attack: 3 frames
    for (int d = 0; d < 8; ++d) {
        gfx.attack.dirs[d].gpStem = "Units\\Infantry\\musket";
        gfx.attack.dirs[d].frameIndices = {10,11,12};
    }
    // Death: 4 frames, non-directional (reuse dir 0)
    for (int d = 0; d < 8; ++d) {
        gfx.death.dirs[d].gpStem = "Units\\Infantry\\musket";
        gfx.death.dirs[d].frameIndices = {20,21,22,23};
    }
    gfx.shadowRadiusX = 14.0f;
    gfx.shadowRadiusY = 7.0f;
    register_unit_gfx("infantry_musket", std::move(gfx));
}

} } // namespace legacy::assets


