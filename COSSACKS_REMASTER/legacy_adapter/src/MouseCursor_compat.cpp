#include "legacy/MouseCursor_compat.hpp"

#include "legacy/GP_Draw_compat.hpp"
#include "legacy/Input_compat.hpp"
#include "legacy/Globals_compat.hpp"
#include "engine_core/InputAdapter.hpp"
#include "resource_io/ResourceIO.hpp"
#include "resource_io/ColorPalette.hpp"
#include <algorithm>
#include <cstring>
#include <iostream>

namespace legacy { namespace cursor_compat {

// Current cursor resource and frame
static int g_cursorGPID = -1;
static int g_cursorFrame = 0;

// Backup buffers (32x32) like original
static uint8_t g_buf1[32 * 32];
static uint8_t g_buf2[32 * 32];
static uint8_t g_buf1o[32 * 32];
static uint8_t g_buf2o[32 * 32];
static int g_oldMX = 0;
static int g_oldMY = 0;
static int g_MX = 0;
static int g_MY = 0;
static bool g_lock = false;

// Cached cursor palette (1w.pal) converted to RGBA
static bool g_cursorPalLoaded = false;
static uint8_t g_cursorPalRGBA[256 * 4];

static bool ensure_cursor_palette_rgba() {
    if (g_cursorPalLoaded) return true;
    std::vector<unsigned char> bytes;
    // Prefer original UI base palette set 0
    if (!resource_io::read_file_anywhere("0\\AGEW_1.PAL", bytes)) {
        // Fallbacks: case/sep variations
        if (!resource_io::read_file_anywhere("0\\agew_1.pal", bytes) &&
            !resource_io::read_file_anywhere("AGEW_1.PAL", bytes) &&
            !resource_io::read_file_anywhere("agew_1.pal", bytes)) {
            return false;
        }
    }
    if (bytes.size() >= 1024) {
        std::memcpy(g_cursorPalRGBA, bytes.data(), 1024);
        g_cursorPalLoaded = true;
        return true;
    } else if (bytes.size() >= 768) {
        for (int i = 0; i < 256; ++i) {
            g_cursorPalRGBA[i * 4 + 0] = bytes[static_cast<size_t>(i) * 3 + 0];
            g_cursorPalRGBA[i * 4 + 1] = bytes[static_cast<size_t>(i) * 3 + 1];
            g_cursorPalRGBA[i * 4 + 2] = bytes[static_cast<size_t>(i) * 3 + 2];
            g_cursorPalRGBA[i * 4 + 3] = 255;
        }
        g_cursorPalLoaded = true;
        return true;
    }
    return false;
}

struct PaletteGuard {
    uint8_t backup[256 * 4];
    bool active{false};
    bool begin_with_cursor_palette() {
        if (!ensure_cursor_palette_rgba()) return false;
        std::memcpy(backup, resource_io::color_palette::get_rgba256x4(), 256 * 4);
        resource_io::color_palette::set_from_rgba(g_cursorPalRGBA);
        active = true;
        return true;
    }
    ~PaletteGuard() {
        if (active) {
            resource_io::color_palette::set_from_rgba(backup);
        }
    }
};

static inline void get_block(uint8_t* dest, const uint8_t* src, int x, int y, int SSizeX, int SSizeY) {
    int Lx = 32;
    int Ly = 32;
    int x1 = x;
    int y1 = y;
    int bx = 0;
    int by = 0;
    if (x1 < 0) { bx = -x1; Lx += x1; x1 = 0; }
    if (y1 < 0) { by = -y1; Ly += y1; y1 = 0; }
    if (x1 + 32 > SSizeX) Lx = SSizeX - x1;
    if (y1 + 32 > SSizeY) Ly = SSizeY - y1;
    if (Lx <= 0 || Ly <= 0) return;
    const uint8_t* s = src + x1 + y1 * SSizeX;
    uint8_t* d = dest + bx + (by << 5);
    const int adds = SSizeX - Lx;
    const int addd = 32 - Lx;
    for (int row = 0; row < Ly; ++row) {
        std::memcpy(d, s, static_cast<size_t>(Lx));
        s += adds + Lx;
        d += addd + Lx;
    }
}

static inline bool cmp_block(const uint8_t* dest, const uint8_t* src, int x, int y, int SSizeX, int SSizeY) {
    int Lx = 32;
    int Ly = 32;
    int x1 = x;
    int y1 = y;
    int bx = 0;
    int by = 0;
    if (x1 < 0) { bx = -x1; Lx += x1; x1 = 0; }
    if (y1 < 0) { by = -y1; Ly += y1; y1 = 0; }
    if (x1 + 32 > SSizeX) Lx = SSizeX - x1;
    if (y1 + 32 > SSizeY) Ly = SSizeY - y1;
    if (Lx <= 0 || Ly <= 0) return false;
    const uint8_t* s = src + x1 + y1 * SSizeX;
    const uint8_t* d = dest + bx + (by << 5);
    const int adds = SSizeX - Lx;
    const int addd = 32 - Lx;
    for (int row = 0; row < Ly; ++row) {
        if (std::memcmp(d, s, static_cast<size_t>(Lx)) != 0) return true;
        s += adds + Lx;
        d += addd + Lx;
    }
    return false;
}

static inline void restore_block(uint8_t* scrn, const uint8_t* buf, const uint8_t* comp, int x, int y, int SSizeX, int SSizeY) {
    int Lx = 32;
    int Ly = 32;
    int x1 = x;
    int y1 = y;
    int bx = 0;
    int by = 0;
    if (x1 < 0) { bx = -x1; Lx += x1; x1 = 0; }
    if (y1 < 0) { by = -y1; Ly += y1; y1 = 0; }
    if (x1 + 32 > SSizeX) Lx = SSizeX - x1;
    if (y1 + 32 > SSizeY) Ly = SSizeY - y1;
    if (Lx <= 0 || Ly <= 0) return;
    const uint8_t* s = buf + bx + (by << 5);
    const uint8_t* c = comp + bx + (by << 5);
    uint8_t* d = scrn + x1 + y1 * SSizeX;
    const int addscr = SSizeX - Lx;
    const int add32 = 32 - Lx;
    for (int row = 0; row < Ly; ++row) {
        for (int i = 0; i < Lx; ++i) {
            if (d[i] == c[i]) d[i] = s[i];
        }
        d += addscr + Lx;
        s += add32 + Lx;
        c += add32 + Lx;
    }
}

void set_cursor_pack(const char* gp_name) {
    legacy::gp::LocalGP pack(gp_name);
    g_cursorGPID = pack.GPID;
    std::cout << "[cursor] loaded: " << (g_cursorGPID >= 0 ? "true" : "false")
              << " pack=" << (gp_name ? gp_name : "<null>")
              << " gpid=" << g_cursorGPID << std::endl;
}

void set_cursor_frame(int frameIndex) { g_cursorFrame = frameIndex; }

static inline void compute_anchor(int frameIndex, int& outDx, int& outDy) {
    if (frameIndex == 8) { outDx = 16; outDy = 17; }
    else { outDx = 5; outDy = 5; }
}

static void redraw_offscreen_cursor() {
    if (g_cursorGPID < 0) return;
    int dx = 0, dy = 0;
    compute_anchor(g_cursorFrame, dx, dy);
    g_lock = true;
    int mx = 0, my = 0;
    engine_core::input::GetPointer(mx, my);
    g_MX = mx - dx;
    g_MY = my - dy;

    uint8_t* back = legacy::globals::get_backbuffer_ptr();
    const int scrW = legacy::globals::get_scr_size_x();
    const int scrH = legacy::globals::get_scr_size_y();
    if (!back || scrW <= 0 || scrH <= 0) return;

    restore_block(back, g_buf1, g_buf2, g_oldMX, g_oldMY, scrW, scrH);
    get_block(g_buf1, back, g_MX, g_MY, scrW, scrH);
    {
        PaletteGuard guard;
        if (guard.begin_with_cursor_palette()) {
            legacy::gp::GPS.ShowGP(g_MX, g_MY, g_cursorGPID, g_cursorFrame, 0);
        } else {
            legacy::gp::GPS.ShowGP(g_MX, g_MY, g_cursorGPID, g_cursorFrame, 0);
        }
    }
    get_block(g_buf2, back, g_MX, g_MY, scrW, scrH);
}

static void redraw_screen_cursor() {
    if (g_cursorGPID < 0) return;
    if (legacy::globals::window_mode) return; // match original bugfix
    uint8_t* front = legacy::globals::get_frontbuffer_ptr();
    const int fsx = legacy::globals::get_rscr_size_x();
    const int fsy = legacy::globals::get_rscr_size_y();
    if (!front || fsx <= 0 || fsy <= 0) return;

    restore_block(front, g_buf1o, g_buf2o, g_oldMX, g_oldMY, fsx, fsy);
    get_block(g_buf1o, front, g_MX, g_MY, fsx, fsy);

    legacy::globals::push_frontbuffer_as_active();
    {
        PaletteGuard guard;
        if (guard.begin_with_cursor_palette()) {
            legacy::gp::GPS.ShowGP(g_MX, g_MY, g_cursorGPID, g_cursorFrame, 0);
        } else {
            legacy::gp::GPS.ShowGP(g_MX, g_MY, g_cursorGPID, g_cursorFrame, 0);
        }
    }
    legacy::globals::pop_active_buffer();

    get_block(g_buf2o, front, g_MX, g_MY, fsx, fsy);
}

void draw_cursor() {
    if (g_cursorGPID < 0) return;
    if (g_lock) return;

    redraw_offscreen_cursor();
    redraw_screen_cursor();
    g_lock = false;
    g_oldMX = g_MX;
    g_oldMY = g_MY;
}

int get_current_gpid() { return g_cursorGPID; }

} } // namespace legacy::cursor_compat


