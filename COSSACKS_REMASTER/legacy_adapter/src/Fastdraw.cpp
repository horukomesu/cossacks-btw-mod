#include "legacy/Fastdraw_compat.hpp"

#include <glad/glad.h>
#include "engine_core/Render2D.hpp"
#include "resource_io/ColorPalette.hpp"

namespace legacy::fastdraw {

int WindX = 0;
int WindY = 0;
int WindLx = 1280;
int WindLy = 720;
int WindX1 = 0;
int WindY1 = 0;
int ScrWidth = 1280;
int ScrHeight = 720;

void SetRLCWindow(int x, int y, int lx, int ly, int /*slx*/) {
    WindX = x;
    WindY = y;
    WindLx = lx;
    WindLy = ly;
    WindX1 = x + lx - 1;
    WindY1 = y + ly - 1;
}

void ClearScreen() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

// Convert 0..255 palette index to RGBA from currently loaded color palette
static inline void idx_to_rgba(uint8_t idx, float& r, float& g, float& b, float& a) {
    const uint8_t* pal = resource_io::color_palette::get_rgba256x4();
    const size_t p = static_cast<size_t>(idx) * 4u;
    r = static_cast<float>(pal[p + 0]) / 255.0f;
    g = static_cast<float>(pal[p + 1]) / 255.0f;
    b = static_cast<float>(pal[p + 2]) / 255.0f;
    a = static_cast<float>(pal[p + 3]) / 255.0f;
}

void CBar(int x, int y, int Lx, int Ly, uint8_t colorIndex) {
    if (Lx <= 0 || Ly <= 0) return;
    // Clip to current RLC window
    int rx = x;
    int ry = y;
    int rw = Lx;
    int rh = Ly;
    if (rx < WindX) { rw -= (WindX - rx); rx = WindX; }
    if (ry < WindY) { rh -= (WindY - ry); ry = WindY; }
    if (rw <= 0 || rh <= 0) return;
    if (rx + rw > WindX1 + 1) { rw = (WindX1 + 1) - rx; if (rw <= 0) return; }
    if (ry + rh > WindY1 + 1) { rh = (WindY1 + 1) - ry; if (rh <= 0) return; }
    float r, g, b, a;
    idx_to_rgba(colorIndex, r, g, b, a);
    engine_core::render2d::draw_rect(static_cast<float>(rx), static_cast<float>(ry), static_cast<float>(rw), static_cast<float>(rh), r, g, b, a);
}

void CopyToScreen(int /*x*/, int /*y*/, int /*Lx*/, int /*Ly*/) {
    // Our rendering is immediate to the backbuffer; copy happens on buffer swap (FlipPages)
    // Keep function for compatibility; nothing to do here.
}

} // namespace legacy::fastdraw
