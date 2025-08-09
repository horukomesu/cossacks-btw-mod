#include "../include/legacy/Text_compat.hpp"

#include "engine_core/Render2D.hpp"

#include <array>
#include <cstring>

namespace legacy { namespace ui {

// 8x8 minimal bitmap font for ASCII 32..127 (only digits/letters/basic punct drawn; others as empty)
static const int kFontW = 8;
static const int kFontH = 8;

static const std::array<uint8_t, 96 * kFontH> kFontBits = [] {
    std::array<uint8_t, 96 * kFontH> bits{};
    // Define a minimal subset: only draw a filled box for any printable char as placeholder
    for (size_t i = 0; i < bits.size(); ++i) bits[i] = 0xFF; // 0xFF => 8 filled pixels
    return bits;
}();

void ShowString(int x, int y, const char* text, uint32_t rgba) {
    if (!text) return;
    const float r = ((rgba >> 24) & 0xFF) / 255.0f;
    const float g = ((rgba >> 16) & 0xFF) / 255.0f;
    const float b = ((rgba >> 8) & 0xFF) / 255.0f;
    const float a = ((rgba >> 0) & 0xFF) / 255.0f;
    int cx = x;
    for (const char* p = text; *p; ++p) {
        unsigned char ch = static_cast<unsigned char>(*p);
        if (ch < 32 || ch >= 128) { cx += kFontW; continue; }
        const size_t gi = static_cast<size_t>(ch - 32) * kFontH;
        for (int row = 0; row < kFontH; ++row) {
            const uint8_t mask = kFontBits[gi + row];
            for (int col = 0; col < kFontW; ++col) {
                if (mask & (1u << (7 - col))) {
                    engine_core::render2d::draw_rect(static_cast<float>(cx + col), static_cast<float>(y + row), 1.0f, 1.0f, r, g, b, a);
                }
            }
        }
        cx += kFontW;
    }
}

int GetStringWidth(const char* text) {
    if (!text) return 0;
    return static_cast<int>(std::strlen(text)) * kFontW;
}

int GetStringHeight() { return kFontH; }

} } // namespace legacy::ui


