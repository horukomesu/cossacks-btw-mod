#include "../include/legacy/Globals_compat.hpp"
// For now we keep small shadow 8-bit buffers just to support legacy cursor blit logic.
// The rest of rendering goes through OpenGL; these buffers are only used by MouseCursor_compat.

#include <vector>

namespace legacy { namespace globals {

int RealLx = 1280;
int RealLy = 720;
int SCRSizeX = 1280;
int RSCRSizeY = 720;
int COPYSizeX = 1280;
int menu_x_off = 0;
int menu_y_off = 0;
int window_mode = 0;
int cam_tile_x = 0;
int cam_tile_y = 0;
int cam_pix_x = 0;
int cam_pix_y = 0;

static std::vector<uint8_t> g_backbuffer8;
static std::vector<uint8_t> g_frontbuffer8;
static bool g_front_is_active = false;

void set_framebuffer_size(int width, int height) {
    RealLx = SCRSizeX = COPYSizeX = width;
    RealLy = RSCRSizeY = height;
    // Center UI designed for 1024x768 by default
    menu_x_off = (RealLx - 1024) / 2; if (menu_x_off < 0) menu_x_off = 0;
    menu_y_off = (RealLy - 768) / 2; if (menu_y_off < 0) menu_y_off = 0;
    g_backbuffer8.assign(static_cast<size_t>(width) * static_cast<size_t>(height), 0);
    g_frontbuffer8.assign(static_cast<size_t>(width) * static_cast<size_t>(height), 0);
}

uint8_t* get_backbuffer_ptr() { return g_backbuffer8.empty() ? nullptr : g_backbuffer8.data(); }
uint8_t* get_frontbuffer_ptr() { return g_frontbuffer8.empty() ? nullptr : g_frontbuffer8.data(); }
int get_scr_size_x() { return SCRSizeX; }
int get_scr_size_y() { return RealLy; }
int get_rscr_size_x() { return RealLx; }
int get_rscr_size_y() { return RSCRSizeY; }
void push_frontbuffer_as_active() { g_front_is_active = true; }
void pop_active_buffer() { g_front_is_active = false; }

} } // namespace legacy::globals


