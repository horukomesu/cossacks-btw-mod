#include "../include/legacy/RLCFont_compat.hpp"

#include "engine_core/Render2D.hpp"
#include "engine_core/Texture.hpp"
#include "resource_io/RLC.hpp"
#include "legacy/GP_Draw_compat.hpp"
#include "resource_io/ResourceIO.hpp"
static bool load_rlc_any(const std::string& name, resource_io::rlc::RLCTableData& tbl) {
    if (resource_io::rlc::load_rlc(name, tbl)) return true;
    // Try suffix search
    std::string found;
    if (resource_io::try_find_entry_by_suffix(name, found)) {
        if (resource_io::rlc::load_rlc(found, tbl)) return true;
    }
    // Try adding .RLC / .rlc when name has no extension
    const bool hasDot = (name.find('.') != std::string::npos);
    if (!hasDot) {
        if (resource_io::try_find_entry_by_suffix(name + ".RLC", found)) {
            if (resource_io::rlc::load_rlc(found, tbl)) return true;
        }
        if (resource_io::try_find_entry_by_suffix(name + ".rlc", found)) {
            if (resource_io::rlc::load_rlc(found, tbl)) return true;
        }
    }
    return false;
}

#include <vector>

namespace legacy { namespace ui {

RLCFont::RLCFont(int gpid) : m_path() {
    std::string path;
    if (legacy::gp::GPS.ResolveGPIDPath(gpid, path)) {
        m_path = path;
    }
}

RLCFont::RLCFont(const char* path) : m_path(path ? path : "") {}

void RLCFont::SetWhiteColor() { SetColorRGBA(255,255,255); }
void RLCFont::SetRedColor() { SetColorRGBA(255,0,0); }
void RLCFont::SetBlackColor() { SetColorRGBA(0,0,0); }
void RLCFont::SetColorRGBA(uint8_t r, uint8_t g, uint8_t b) { m_colorR=r; m_colorG=g; m_colorB=b; }
void RLCFont::SetColorTable(int idx) { if (idx==3) SetColorRGBA(255,128,0); else if (idx==4) SetWhiteColor(); }
void RLCFont::SetBaseChar(uint8_t ch) { m_baseChar = ch; }

std::pair<int,int> RLCFont::ensureGlyphSize(int subIndex) const {
    auto it = m_sizeCache.find(subIndex);
    if (it != m_sizeCache.end()) return it->second;
    resource_io::rlc::RLCTableData tbl;
    if (!load_rlc_any(m_path, tbl)) return {0,0};
    int w=0,h=0;
    if (!resource_io::rlc::get_subimage_size(tbl, subIndex, w, h)) return {0,0};
    m_sizeCache.emplace(subIndex, std::make_pair(w,h));
    return {w,h};
}

unsigned int RLCFont::ensureGlyphTexture(int subIndex) const {
    const uint32_t keyColor = (static_cast<uint32_t>(m_colorR)<<16) | (static_cast<uint32_t>(m_colorG)<<8) | static_cast<uint32_t>(m_colorB);
    GlyphKey key{subIndex, keyColor};
    auto it = m_texCache.find(key);
    if (it != m_texCache.end()) return it->second;

    resource_io::rlc::RLCTableData tbl;
    if (!load_rlc_any(m_path, tbl)) return 0;
    int w=0,h=0;
    if (!resource_io::rlc::get_subimage_size(tbl, subIndex, w, h)) return 0;
    std::vector<uint8_t> indices;
    if (!resource_io::rlc::decode_subimage_indexed(tbl, subIndex, indices)) return 0;
    // Build RGBA from base color tinted by current color; index 0 transparent
    std::vector<uint8_t> rgba(static_cast<size_t>(w)*static_cast<size_t>(h)*4u);
    for (int i=0;i<w*h;++i){
        const uint8_t idx = indices[static_cast<size_t>(i)];
        const size_t p = static_cast<size_t>(i)*4u;
        rgba[p+0] = (idx==0)?0:m_colorR;
        rgba[p+1] = (idx==0)?0:m_colorG;
        rgba[p+2] = (idx==0)?0:m_colorB;
        rgba[p+3] = (idx==0)?0:255;
    }
    unsigned int tex = engine_core::texture::create_texture_rgba(w,h,rgba.data());
    m_texCache.emplace(key, tex);
    return tex;
}

int RLCFont::GetCharWidth(unsigned char ch) const {
    const int gi = glyphIndex(ch);
    if (gi < 0) return 0;
    return ensureGlyphSize(gi).first;
}

int RLCFont::GetCharHeight() const {
    // Assume height by space glyph
    return ensureGlyphSize(0).second;
}

int RLCFont::GetRLCStrWidth(const char* text) const {
    if (!text) return 0;
    int sum=0;
    for (const char* p=text;*p;++p){ sum += GetCharWidth(static_cast<unsigned char>(*p)); }
    return sum;
}

void RLCFont::ShowString(int x, int y, const char* text) const {
    if (!text) return;
    int cx = x;
    for (const char* p=text;*p;++p){
        const unsigned char ch = static_cast<unsigned char>(*p);
        const int gi = glyphIndex(ch);
        if (gi < 0) { cx += GetCharWidth(' '); continue; }
        const auto [w,h] = ensureGlyphSize(gi);
        const unsigned int tex = ensureGlyphTexture(gi);
        if (tex && w>0 && h>0){
            engine_core::render2d::draw_textured_quad(tex, static_cast<float>(cx), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h));
        }
        cx += w;
    }
}

} } // namespace legacy::ui


