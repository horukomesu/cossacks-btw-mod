#include "../include/legacy/RLC_compat.hpp"

#include "../include/legacy/Fastdraw_compat.hpp"
#include "../include/legacy/GP_Draw_compat.hpp"

#include <vector>
#include <cstring>

#include "../../engine_core/include/engine_core/Render2D.hpp"
#include "../../engine_core/include/engine_core/Texture.hpp"
#include "../../resource_io/include/resource_io/RLC.hpp"
#include "../../resource_io/include/resource_io/Palettes.hpp"
#include "../../resource_io/include/resource_io/ColorPalette.hpp"

namespace legacy { namespace rlc_compat {

static inline const xRLCTable* asTable(const resource_io::rlc::RLCTableData& d) {
    return reinterpret_cast<const xRLCTable*>(d.bytes.data());
}

bool LoadRLC(const char* fileName, RLCTable* outRlcTable) {
    if (!outRlcTable) return false;
    resource_io::rlc::RLCTableData tbl;
    if (!resource_io::rlc::load_rlc(fileName, tbl)) return false;
    const int size = static_cast<int>(tbl.bytes.size());
    // Allocate legacy-like blob with an extra 4-byte Size prefix, but KEEP OfsTable as relative offsets from &sign
    xRLCTable* r = reinterpret_cast<xRLCTable*>(std::malloc(static_cast<size_t>(size) + 4u));
    if (!r) return false;
    r->Size = size + 4;
    std::memcpy(&r->sign, tbl.bytes.data(), static_cast<size_t>(size));
    *outRlcTable = r;
    return true;
}

// Internal: draw a single subimage using current palette remap id (-1 = white tint, 0 = base palette, >0 = palN)
static void draw_pic_core(int x, int y, const void* picPtr, int paletteId) {
    if (!picPtr) return;
    const auto* hdr = reinterpret_cast<const RLCHeader*>(picPtr);
    const int w = static_cast<int>(static_cast<uint16_t>(hdr->SizeX));
    const int h = static_cast<int>(static_cast<uint16_t>(hdr->SizeY));
    if (w <= 0 || h <= 0) return;

    // Clip to legacy window (Fastdraw_compat)
    using namespace legacy::fastdraw;
    int rx = x, ry = y, rw = w, rh = h;
    if (ry + rh - 1 < WindY || ry > WindY1 || rx + rw <= WindX || rx > WindX1) return;
    if (rx < WindX) { int d = WindX - rx; rx += d; rw -= d; }
    if (ry < WindY) { int d = WindY - ry; ry += d; rh -= d; }
    if (rx + rw > WindX1 + 1) { rw = (WindX1 + 1) - rx; }
    if (ry + rh > WindY1 + 1) { rh = (WindY1 + 1) - ry; }
    if (rw <= 0 || rh <= 0) return;

    // Decode indices for the visible rect; simplest path – decode full to indices once
    // Using resource_io decoder requires the table and subindex; our picPtr is absolute pointer
    // into a loaded legacy RLCTable, so rebuild a small table view around this image.
    // Build indices by walking encoded stream manually to preserve exact shape.
    const std::uint8_t* ptr = reinterpret_cast<const std::uint8_t*>(picPtr) + sizeof(RLCHeader);
    std::vector<std::uint8_t> indices(static_cast<size_t>(w) * static_cast<size_t>(h), 0);
    for (int yy = 0; yy < h; ++yy) {
        std::uint8_t segs = *ptr++;
        int sx = 0;
        for (std::uint8_t s = 0; s < segs; ++s) {
            const std::uint8_t skip = *ptr++;
            sx += skip;
            const std::uint8_t run = *ptr++;
            for (int i = 0; i < run; ++i) {
                const std::uint8_t v = *ptr++;
                const int px = sx + i;
                if (px >= 0 && px < w) {
                    indices[static_cast<size_t>(yy) * static_cast<size_t>(w) + static_cast<size_t>(px)] = v;
                }
            }
            sx += run;
        }
        // Advance to next line start if encoder pads lines with zero segments
        // (safety: clamp pointer to buffer bounds handled by caller)
    }

    // Build RGBA for the clipped rect
    std::vector<std::uint8_t> rgba(static_cast<size_t>(rw) * static_cast<size_t>(rh) * 4u, 0);
    const std::uint8_t* baseRGBA = resource_io::color_palette::get_rgba256x4();
    std::uint8_t palRGBA[256 * 4];
    if (paletteId < 0) {
        // white tint: map 0->transparent; non-zero -> white 255,255,255,255
        for (int j = 0; j < rh; ++j) {
            for (int i = 0; i < rw; ++i) {
                const int sx = i + (rx - x);
                const int sy = j + (ry - y);
                const std::uint8_t idx = indices[static_cast<size_t>(sy) * static_cast<size_t>(w) + static_cast<size_t>(sx)];
                const size_t o = (static_cast<size_t>(j) * static_cast<size_t>(rw) + static_cast<size_t>(i)) * 4u;
                rgba[o + 0] = rgba[o + 1] = rgba[o + 2] = (idx == 0 ? 0 : 255);
                rgba[o + 3] = (idx == 0 ? 0 : 255);
            }
        }
    } else {
        if (paletteId == 0) {
            std::memcpy(palRGBA, baseRGBA, sizeof(palRGBA));
        } else {
            const std::uint8_t* map = resource_io::palettes::get_table(paletteId);
            for (int k = 0; k < 256; ++k) {
                const int si = static_cast<int>(map[k]) * 4;
                palRGBA[k * 4 + 0] = baseRGBA[si + 0];
                palRGBA[k * 4 + 1] = baseRGBA[si + 1];
                palRGBA[k * 4 + 2] = baseRGBA[si + 2];
                palRGBA[k * 4 + 3] = baseRGBA[si + 3];
            }
        }
        for (int j = 0; j < rh; ++j) {
            for (int i = 0; i < rw; ++i) {
                const int sx = i + (rx - x);
                const int sy = j + (ry - y);
                const std::uint8_t idx = indices[static_cast<size_t>(sy) * static_cast<size_t>(w) + static_cast<size_t>(sx)];
                const size_t o = (static_cast<size_t>(j) * static_cast<size_t>(rw) + static_cast<size_t>(i)) * 4u;
                const size_t p = static_cast<size_t>(idx) * 4u;
                // exact palette color
                rgba[o + 0] = palRGBA[p + 0];
                rgba[o + 1] = palRGBA[p + 1];
                rgba[o + 2] = palRGBA[p + 2];
                rgba[o + 3] = (idx == 0 ? 0 : palRGBA[p + 3]);
            }
        }
    }

    // Draw with GL (anchor is top-left)
    auto tex = engine_core::texture::create_texture_rgba(rw, rh, rgba.data());
    engine_core::render2d::draw_textured_quad(tex, static_cast<float>(rx), static_cast<float>(ry), static_cast<float>(rw), static_cast<float>(rh));
    engine_core::texture::destroy_texture(tex);
}

void ShowRLC(int x, int y, void* picPtr) {
    // Default legacy path uses the current screen palette
    draw_pic_core(x, y, picPtr, /*paletteId*/ 0);
}

void ShowRLCi(int x, int y, void* picPtr) {
    // Mirror horizontally: draw shifted by width to emulate i-variant
    const auto* hdr = reinterpret_cast<const RLCHeader*>(picPtr);
    const int w = static_cast<int>(static_cast<uint16_t>(hdr->SizeX));
    draw_pic_core(x - w + 1, y, picPtr, /*paletteId*/ 0);
}

void ShowRLCpal(int x, int y, void* picPtr, byte* /*pal*/) {
    // Map PALx.dat to paletteId 1..7 via resource_io::palettes; 0 for base palette
    // Here use base palette (0) — caller-selectable variants handled by ShowRLCItem(nt)
    draw_pic_core(x, y, picPtr, /*paletteId*/ 0);
}

void ShowRLCipal(int x, int y, void* picPtr, byte* /*pal*/) {
    const auto* hdr = reinterpret_cast<const RLCHeader*>(picPtr);
    const int w = static_cast<int>(static_cast<uint16_t>(hdr->SizeX));
    draw_pic_core(x - w + 1, y, picPtr, /*paletteId*/ 0);
}

void ShowRLCItem(int x, int y, lpRLCTable lprt, int n, byte nt) {
    // Mirror original dispatch: GP first when GPID < 4096
    const std::uintptr_t tag = reinterpret_cast<std::uintptr_t>(*lprt);
    if (tag < 4096u) {
        legacy::gp::GPS.ShowGP(x, y, static_cast<int>(tag), n, nt);
        return;
    }
    const bool inv = (n >= 4096);
    const int idx = inv ? (n - 4096) : n;
    const std::uint8_t* base = reinterpret_cast<const std::uint8_t*>(*lprt);
    void* picPtr = const_cast<void*>(reinterpret_cast<const void*>(base + 4 + (*lprt)->OfsTable[idx]));
    // nt: 0=normal, 1..7 remap using pal1..pal7
    if (nt == 0) {
        inv ? ShowRLCi(x, y, picPtr) : ShowRLC(x, y, picPtr);
        return;
    }
    // nation palettes 1..7 map directly to paletteId 1..7
    const int paletteId = static_cast<int>(nt);
    // Use core with palette
    if (inv) {
        const int w = GetRLCWidth(*lprt, static_cast<byte>(idx));
        draw_pic_core(x - w + 1, y, picPtr, paletteId);
    } else {
        draw_pic_core(x, y, picPtr, paletteId);
    }
}

void ShowRLCItemPal(int x, int y, lpRLCTable lprt, int n, byte* /*Pal*/) {
    // Base palette mapping (screen palette)
    const bool inv = (n >= 4096);
    const int idx = inv ? (n - 4096) : n;
    const std::uint8_t* base = reinterpret_cast<const std::uint8_t*>(*lprt);
    void* picPtr = const_cast<void*>(reinterpret_cast<const void*>(base + 4 + (*lprt)->OfsTable[idx]));
    if (inv) {
        const int w = GetRLCWidth(*lprt, static_cast<byte>(idx));
        draw_pic_core(x - w + 1, y, picPtr, /*paletteId*/ 0);
    } else {
        draw_pic_core(x, y, picPtr, /*paletteId*/ 0);
    }
}

void ShowRLCItemTrans8(int x, int y, lpRLCTable lprt, int n) {
    // Approximate with base palette (could implement trans8 via shader LUT later)
    ShowRLCItem(x, y, lprt, n, /*nt*/ 0);
}

void ShowRLCItemDarkN(int x, int y, lpRLCTable lprt, int n, int Ints) {
    // Map intensity 1..7 to paletteId 1..7, clamp
    int palId = Ints;
    if (palId < 1) palId = 1; if (palId > 7) palId = 7;
    ShowRLCItem(x, y, lprt, n, static_cast<byte>(palId));
}

void ShowRLCItemRedN(int x, int y, lpRLCTable lprt, int n, int Ints) {
    int palId = Ints;
    if (palId < 1) palId = 1; if (palId > 7) palId = 7;
    ShowRLCItem(x, y, lprt, n, static_cast<byte>(palId));
}

void ShowRLCItemDark(int x, int y, lpRLCTable lprt, int n) {
    ShowRLCItemDarkN(x, y, lprt, n, /*Ints*/ 1);
}

void ShowRLCItemBlue(int x, int y, lpRLCTable lprt, int n) {
    ShowRLCItemDarkN(x, y, lprt, n, /*Ints*/ 1);
}

void ShowRLCItemFired(int x, int y, lpRLCTable lprt, int n) {
    ShowRLCItemRedN(x, y, lprt, n, /*Ints*/ 1);
}

int GetRLCWidth(RLCTable lpr, byte n) {
    const std::uintptr_t tag = reinterpret_cast<std::uintptr_t>(lpr);
    if (tag < 4096u) {
        const int GPID = static_cast<int>(tag);
        if (n == 32) return legacy::gp::GPS.GetGPWidth(GPID, 'c');
        return legacy::gp::GPS.GetGPWidth(GPID, n);
    }
    const xRLCTable* t = lpr;
    if (n >= static_cast<byte>(t->SCount)) return 0;
    const auto* hdr = reinterpret_cast<const RLCHeader*>(
        reinterpret_cast<const std::uint8_t*>(t) + 4 + t->OfsTable[n]);
    return static_cast<int>(static_cast<uint16_t>(hdr->SizeX));
}

int GetRLCHeight(RLCTable lpr, byte n) {
    const std::uintptr_t tag = reinterpret_cast<std::uintptr_t>(lpr);
    if (tag < 4096u) return legacy::gp::GPS.GetGPHeight(static_cast<int>(tag), n);
    const xRLCTable* t = lpr;
    if (n >= static_cast<byte>(t->SCount)) return 0;
    const auto* hdr = reinterpret_cast<const RLCHeader*>(
        reinterpret_cast<const std::uint8_t*>(t) + 4 + t->OfsTable[n]);
    return static_cast<int>(static_cast<uint16_t>(hdr->SizeY));
}

void LoadPalettes() {
    // Ensure palette tables are initialized
    (void)resource_io::palettes::get_table(0);
}

} } // namespace legacy::rlc_compat


