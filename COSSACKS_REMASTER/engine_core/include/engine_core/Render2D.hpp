#pragma once

#include <cstdint>

namespace engine_core::render2d {

bool initialize();
void shutdown();

void begin_frame(int framebufferWidth, int framebufferHeight);
void end_frame();

// Draw a solid color rectangle at pixel coords
void draw_rect(float x, float y, float w, float h, float r, float g, float b, float a);

// Draw a textured quad (texture must be GL texture name)
void draw_textured_quad(unsigned int texture, float x, float y, float w, float h);

} // namespace engine_core::render2d
