#include "../include/legacy/Text_compat.hpp"

#include "legacy/RLC_compat.hpp"
#include "resource_io/ColorPalette.hpp"

#include <cstring>

namespace legacy { namespace ui {

// Use RLC-based renderer for ASCII 32..127 using RF32W.RLC (wide white font)
static legacy::rlc_compat::RLCTable g_rlcFont = nullptr;
static bool g_loaded = false;

static void ensure_font_loaded() {
    if (!g_loaded) {
        g_loaded = legacy::rlc_compat::LoadRLC("RF32W.RLC", &g_rlcFont);
        // Ensure palette is initialized (fonts rely on palette indices)
        (void)resource_io::color_palette::load_from_path("resources/1W.pal");
    }
}

void ShowString(int x, int y, const char* text, uint32_t /*rgba*/) {
    if (!text) return;
    ensure_font_loaded();
    if (!g_loaded || !g_rlcFont) return;
    int cx = x;
    const unsigned char* p = reinterpret_cast<const unsigned char*>(text);
    while (*p) {
        unsigned char ch = *p++;
        if (ch < 32) ch = 32;
        const int idx = static_cast<int>(ch);
        legacy::rlc_compat::ShowRLCItem(cx, y, &g_rlcFont, idx, /*nt*/ 0);
        int adv = legacy::rlc_compat::GetRLCWidth(g_rlcFont, static_cast<legacy::rlc_compat::byte>(idx));
        if (adv <= 0) adv = 8;
        cx += adv;
    }
}

int GetStringWidth(const char* text) {
    if (!text) return 0;
    ensure_font_loaded();
    if (!g_loaded || !g_rlcFont) return 0;
    int w = 0;
    const unsigned char* p = reinterpret_cast<const unsigned char*>(text);
    while (*p) {
        unsigned char ch = *p++;
        if (ch < 32) ch = 32;
        int adv = legacy::rlc_compat::GetRLCWidth(g_rlcFont, static_cast<legacy::rlc_compat::byte>(ch));
        if (adv <= 0) adv = 8;
        w += adv;
    }
    return w;
}

int GetStringHeight() {
    ensure_font_loaded();
    if (!g_loaded || !g_rlcFont) return 16;
    int h = legacy::rlc_compat::GetRLCHeight(g_rlcFont, static_cast<legacy::rlc_compat::byte>('A'));
    if (h <= 0) h = 16;
    return h;
}

} } // namespace legacy::ui


