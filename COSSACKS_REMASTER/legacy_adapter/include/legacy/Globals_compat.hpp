#pragma once

#include <cstdint>

namespace legacy { namespace globals {

extern int RealLx;
extern int RealLy;
extern int SCRSizeX;
extern int RSCRSizeY;
extern int COPYSizeX;
extern int menu_x_off;
extern int menu_y_off;
extern int window_mode; // 0 windowed, 1 fullscreen (placeholder)

// Hook for engine to update sizes
void set_framebuffer_size(int width, int height);

} } // namespace legacy::globals


