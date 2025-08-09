#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

namespace legacy { namespace ui {

class RLCFont {
public:
    // Construct from GPID resolved by legacy::gp (RLC resource)
    explicit RLCFont(int gpid);
    // Construct from explicit resource path (e.g., "rom10.rlc")
    explicit RLCFont(const char* path);

    // Color controls (tint). Alpha is ignored (comes from glyph texture).
    void SetWhiteColor();
    void SetRedColor();
    void SetBlackColor();
    void SetColorRGBA(uint8_t r, uint8_t g, uint8_t b);
    void SetColorTable(int idx); // Approximate common tables: 3=orange, 4=white

    // Layout controls
    void SetBaseChar(uint8_t ch); // default 32 (space)
    int GetCharWidth(unsigned char ch) const;
    int GetCharHeight() const;

    // Metrics
    int GetRLCStrWidth(const char* text) const;

    // Render
    void ShowString(int x, int y, const char* text) const;

private:
    std::string m_path;
    uint8_t m_colorR{255}, m_colorG{255}, m_colorB{255};
    uint8_t m_baseChar{32};

    struct GlyphKey { int subIndex; uint32_t color; bool operator==(const GlyphKey& o) const { return subIndex==o.subIndex && color==o.color; } };
    struct GlyphKeyHash { size_t operator()(const GlyphKey& k) const noexcept { return static_cast<size_t>(k.subIndex) * 1315423911u ^ k.color; } };
    mutable std::unordered_map<GlyphKey, unsigned int, GlyphKeyHash> m_texCache; // GL texture per glyph & color
    mutable std::unordered_map<int, std::pair<int,int>> m_sizeCache; // subIndex -> (w,h)

    int glyphIndex(unsigned char ch) const { if (ch < m_baseChar) return -1; return static_cast<int>(ch) - static_cast<int>(m_baseChar); }
    unsigned int ensureGlyphTexture(int subIndex) const;
    std::pair<int,int> ensureGlyphSize(int subIndex) const;
};

} } // namespace legacy::ui


