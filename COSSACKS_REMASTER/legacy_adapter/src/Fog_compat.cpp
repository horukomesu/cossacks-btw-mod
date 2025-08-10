#include "legacy/Fog_compat.hpp"

#include "legacy/MouseCursor_compat.hpp"
#include <cstdio>

namespace legacy { namespace fog_compat {

void LoadFog(int set) {
    // Original sets CurrentCursorGP = set + 5; We map that to cursor packs.
    // 0 -> Cursor_05, 1 -> Cursor_06, 2 -> Cursor_07, etc. If missing, keep current.
    char pack[32];
    int base = 5 + set;
    if (base < 0) base = 0;
    if (base > 99) base = 99;
    std::snprintf(pack, sizeof(pack), "Cursor_%02d", base);
    legacy::cursor_compat::set_cursor_pack(pack);
}

} } // namespace legacy::fog_compat



