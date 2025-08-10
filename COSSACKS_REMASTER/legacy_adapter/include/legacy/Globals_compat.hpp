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

// Low-level framebuffers accessors used by legacy cursor routines
// In our GL build these return pointers to shadow 8-bit surfaces maintained for compatibility.
uint8_t* get_backbuffer_ptr();
uint8_t* get_frontbuffer_ptr();
int get_scr_size_x();
int get_scr_size_y();
int get_rscr_size_x();
int get_rscr_size_y();
void push_frontbuffer_as_active();
void pop_active_buffer();

} } // namespace legacy::globals


