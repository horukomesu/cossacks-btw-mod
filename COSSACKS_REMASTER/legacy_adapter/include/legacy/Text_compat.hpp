#pragma once

#include <cstdint>

namespace legacy { namespace ui {

// Minimal text adapter for UI. Renders ASCII using a built-in 8x8 bitmap font.
void ShowString(int x, int y, const char* text, uint32_t rgba = 0xFFFFFFFF);
int GetStringWidth(const char* text);
int GetStringHeight();

} } // namespace legacy::ui


