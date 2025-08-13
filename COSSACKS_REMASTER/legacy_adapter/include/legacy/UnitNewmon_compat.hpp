#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace legacy { namespace newmon_compat {

struct RLCRefEntry {
    std::string gpStem; // resource stem passed to GP loader
    int dx{0};
    int dy{0};
    bool isShadow{false};
    int numSprites{0};
};

struct FrameRef {
    int rlcRef{0};
    int spriteId{0};
    int dx{0};
    int dy{0};
};

struct AnimationDef {
    bool enabled{false};
    int rotations{0};
    int ticksPerFrame{1};
    std::vector<FrameRef> frames;
};

struct UnitDef {
    // Map from tags like "#STAND", "#MOTION_L", "#ATTACK" ... to animation definitions
    std::unordered_map<std::string, AnimationDef> anims;
};

// Global tables
bool load_weapon_ads();
const std::unordered_map<int, RLCRefEntry>& get_rlc_table();

// Load a single unit definition from <name>.md (name in any case)
bool load_unit_md(const std::string& name);
const UnitDef* get_unit_def(const std::string& name);

} } // namespace legacy::newmon_compat


