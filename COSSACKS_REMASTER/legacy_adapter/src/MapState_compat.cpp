#include "legacy/MapState_compat.hpp"

namespace legacy { namespace map_state {

static resource_io::map::MapData* g_current_map = nullptr;

void set_current_map(resource_io::map::MapData* map) { g_current_map = map; }
resource_io::map::MapData* get_current_map() { return g_current_map; }

} } // namespace legacy::map_state


