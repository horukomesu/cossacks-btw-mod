#pragma once

#include "resource_io/Map.hpp"

namespace legacy { namespace map_state {

// Store pointer to the currently active map for compat modules to access WaterDeep/Bright etc.
void set_current_map(resource_io::map::MapData* map);
resource_io::map::MapData* get_current_map();

} } // namespace legacy::map_state


