#include "../include/legacy/Globals_compat.hpp"

namespace legacy { namespace globals {

int RealLx = 1280;
int RealLy = 720;
int SCRSizeX = 1280;
int RSCRSizeY = 720;
int COPYSizeX = 1280;
int menu_x_off = 0;
int menu_y_off = 0;
int window_mode = 0;

void set_framebuffer_size(int width, int height) {
    RealLx = SCRSizeX = COPYSizeX = width;
    RealLy = RSCRSizeY = height;
    // Center UI designed for 1024x768 by default
    menu_x_off = (RealLx - 1024) / 2; if (menu_x_off < 0) menu_x_off = 0;
    menu_y_off = (RealLy - 768) / 2; if (menu_y_off < 0) menu_y_off = 0;
}

} } // namespace legacy::globals


