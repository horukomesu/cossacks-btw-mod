#include "legacy/MouseCursor_compat.hpp"

#include "legacy/GP_Draw_compat.hpp"
#include "legacy/Input_compat.hpp"
#include "engine_core/InputAdapter.hpp"
#include <iostream>

namespace legacy { namespace cursor_compat {

static int g_cursorGPID = -1;
static int g_cursorFrame = 0;
static bool g_loggedLoad = false;

void set_cursor_pack(const char* gp_name) {
    legacy::gp::LocalGP pack(gp_name);
    g_cursorGPID = pack.GPID;
    // Debug: report whether the cursor sprite pack was found
    std::cout << "[cursor] loaded: " << (g_cursorGPID >= 0 ? "true" : "false")
              << " pack=" << (gp_name ? gp_name : "<null>")
              << " gpid=" << g_cursorGPID << std::endl;
    g_loggedLoad = true;
}

void set_cursor_frame(int frameIndex) { g_cursorFrame = frameIndex; }

void draw_cursor() {
    if (g_cursorGPID < 0) return;
    int cx = 0, cy = 0;
    engine_core::input::GetPointer(cx, cy);
    // Basic anchor like in legacy (5,5) except for frame 8 (gather point) (16,17)
    int dx = (g_cursorFrame == 8) ? 16 : 5;
    int dy = (g_cursorFrame == 8) ? 17 : 5;
    legacy::gp::GPS.ShowGP(cx - dx, cy - dy, g_cursorGPID, g_cursorFrame, 0);
}

} } // namespace legacy::cursor_compat


