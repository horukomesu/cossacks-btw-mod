#include "legacy/Water_compat.hpp"
#include "legacy/MapState_compat.hpp"
#include "legacy/Path_compat.hpp"

#include <algorithm>
#include <cmath>

namespace legacy { namespace water_compat {

// Helper: compute 4 downsampled depths around cell center (mirrors RealWater::GetZ usage)
static inline int blend4(int z1, int z2, int z3, int z4) {
    // Same weighted sum as original GetZ(): (3*(3*z1+z2+z3)+z4)/16 simplified here
    return (3 * (3 * z1 + z2 + z3) + z4) >> 4;
}

void rebuild_water_locking_region(int x0, int y0, int x1, int y1) {
    using namespace legacy::path_compat;
    auto* map = legacy::map_state::get_current_map();
    if (!map) return;
    const int maxwx = MaxWX;
    const int maxwy = MaxWY;
    if (map->waterDeep.empty()) return;
    x0 = std::max(0, std::min(x0, maxwx - 1));
    y0 = std::max(0, std::min(y0, maxwy - 1));
    x1 = std::max(0, std::min(x1, maxwx));
    y1 = std::max(0, std::min(y1, maxwy));

    // Update MFIELDS[1] based on depth threshold ~130 as in original CreateWaterLocking
    for (int y = y0; y < y1; ++y) {
        for (int x = x0; x < x1; ++x) {
            const int ofst = x + y * maxwx;
            const int z1 = map->waterDeep[ofst];
            const int z2 = (x + 1 < maxwx) ? map->waterDeep[ofst + 1] : z1;
            const int z3 = (y + 1 < maxwy) ? map->waterDeep[ofst + maxwx] : z1;
            const int z4 = (x + 1 < maxwx && y + 1 < maxwy) ? map->waterDeep[ofst + maxwx + 1] : z1;
            const int D1 = blend4(z1, z2, z3, z4);
            const int D2 = blend4(z2, z4, z1, z3);
            const int D3 = blend4(z3, z4, z1, z2);
            const int D4 = blend4(z4, z3, z2, z1);
            const int xx = x + x;
            const int yy = y + y;
            if (D1 > 130) MFIELDS[1].BClrPt(xx, yy);     else MFIELDS[1].BSetPt(xx, yy);
            if (D2 > 130) MFIELDS[1].BClrPt(xx + 1, yy); else MFIELDS[1].BSetPt(xx + 1, yy);
            if (D3 > 130) MFIELDS[1].BClrPt(xx, yy + 1); else MFIELDS[1].BSetPt(xx, yy + 1);
            if (D4 > 130) MFIELDS[1].BClrPt(xx + 1, yy + 1); else MFIELDS[1].BSetPt(xx + 1, yy + 1);
        }
    }
}

void rebuild_water_locking_full() {
    rebuild_water_locking_region(0, 0, legacy::path_compat::MaxWX, legacy::path_compat::MaxWY);
}

// -------- Ported editor-like water operations --------

// Increase or decrease brightness around a spot (adapted from RealWater::SetBrightSpot)
void SetBrightSpot(int x, int y, int Brightness, bool increase) {
    auto* map = legacy::map_state::get_current_map();
    if (!map) return;
    if (map->waterBright.empty()) return;
    const int maxwx = legacy::path_compat::MaxWX;
    const int maxwy = legacy::path_compat::MaxWY;
    const int xx = x >> 5;      // legacy downscale
    const int yy = y >> 4;      // note different shift per original
    const int BRI16 = Brightness << 4;
    if (increase) {
        for (int dy = -Brightness; dy <= Brightness; ++dy) {
            for (int dx = -Brightness; dx <= Brightness; ++dx) {
                const int rr = static_cast<int>(std::sqrt((dx * dx + dy * dy) << 8));
                const int r = rr >> 4;
                const int x1 = xx + dx;
                const int y1 = yy + dy;
                if (r <= Brightness && x1 >= 0 && y1 >= 0 && x1 < maxwx && y1 < maxwy) {
                    const int bri = BRI16 - rr;
                    const int ofst = x1 + y1 * maxwx;
                    if (map->waterBright[ofst] < bri) map->waterBright[ofst] = static_cast<uint8_t>(std::clamp(bri, 0, 255));
                }
            }
        }
    } else {
        const int MBright = 16 + Brightness;
        for (int dy = -MBright; dy <= MBright; ++dy) {
            for (int dx = -MBright; dx <= MBright; ++dx) {
                const int rr = static_cast<int>(std::sqrt((dx * dx + dy * dy) << 8));
                const int r = rr >> 4;
                const int x1 = xx + dx;
                const int y1 = yy + dy;
                if (r <= MBright && x1 >= 0 && y1 >= 0 && x1 < maxwx && y1 < maxwy) {
                    const int ofst = x1 + y1 * maxwx;
                    if (r < Brightness) map->waterBright[ofst] = 0;
                    else {
                        const int bri = rr - BRI16;
                        if (map->waterBright[ofst] > bri) map->waterBright[ofst] = static_cast<uint8_t>(std::clamp(bri, 0, 255));
                    }
                }
            }
        }
    }
}

// Helpers from RealWater: radial depth increase/decrease shapes
static void SetDeepSpot(resource_io::map::MapData* map, int x, int y, int r, int dr, int dh) {
    if (!map) return;
    const int maxwx = legacy::path_compat::MaxWX;
    const int maxwy = legacy::path_compat::MaxWY;
    const int xx = x >> 5;
    const int yy = y >> 4;
    const int rr = (r + dr) >> 5;
    int x0 = xx - rr - 3;
    int y0 = yy - rr - 3;
    int x1 = xx + rr + 3;
    int y1 = yy + rr + 3;
    x0 = std::clamp(x0, 0, maxwx - 1);
    y0 = std::clamp(y0, 0, maxwy - 1);
    x1 = std::clamp(x1, 0, maxwx - 1);
    y1 = std::clamp(y1, 0, maxwy - 1);
    y <<= 1;
    bool change = false;
    for (int xa = x0; xa <= x1; ++xa) {
        for (int ya = y0; ya < y1; ++ya) {
            const int dx = x - (xa << 5);
            const int dy = y - (ya << 5);
            const int ra = static_cast<int>(std::sqrt(double(dx * dx + dy * dy)));
            int dhi = 0;
            if (ra < r) dhi = dh; else if (ra >= r + dr) dhi = 0; else dhi = (dh * (r + dr - ra)) / dr;
            const int pos = xa + ya * maxwx;
            int v = int(map->waterDeep[pos]) + dhi;
            if (v > 255) v = 255;
            if (v != map->waterDeep[pos]) { map->waterDeep[pos] = static_cast<uint8_t>(v); change = true; }
        }
    }
    if (change) {
        rebuild_water_locking_region(x0, y0, x1 + 1, y1 + 1);
    }
}

static void ClearDeepSpot(resource_io::map::MapData* map, int x, int y, int r, int dr, int dh) {
    if (!map) return;
    const int maxwx = legacy::path_compat::MaxWX;
    const int maxwy = legacy::path_compat::MaxWY;
    const int xx = x >> 5;
    const int yy = y >> 4;
    const int rr = (r + dr) >> 5;
    int x0 = xx - rr - 3;
    int y0 = yy - rr - 3;
    int x1 = xx + rr + 3;
    int y1 = yy + rr + 3;
    x0 = std::clamp(x0, 0, maxwx - 1);
    y0 = std::clamp(y0, 0, maxwy - 1);
    x1 = std::clamp(x1, 0, maxwx - 1);
    y1 = std::clamp(y1, 0, maxwy - 1);
    y <<= 1;
    bool change = false;
    for (int xa = x0; xa <= x1; ++xa) {
        for (int ya = y0; ya < y1; ++ya) {
            const int dx = x - (xa << 5);
            const int dy = y - (ya << 5);
            const int ra = static_cast<int>(std::sqrt(double(dx * dx + dy * dy)));
            int dhi = 0;
            if (ra < r) dhi = dh; else if (ra >= r + dr) dhi = 0; else dhi = (dh * (r + dr - ra)) / dr;
            const int pos = xa + ya * maxwx;
            dhi = 255 - std::clamp(dhi, 0, 255);
            if (map->waterDeep[pos] > dhi) { map->waterDeep[pos] = static_cast<uint8_t>(dhi); change = true; }
        }
    }
    if (change) {
        rebuild_water_locking_region(x0, y0, x1 + 1, y1 + 1);
    }
}

// Good spot variants (ported arithmetic from RealWater)
static void SetGoodDeepSpot(int x, int y, int r, int dr, int dh) {
    int dr1 = (dr * (dh - 128)) / (143 - 128);
    if (dr1 >= r) { dh = 128 + ((dh - 128) * dr1) / r; dr = r; }
    const int r0 = r - dr1;
    const int r1 = r + ((dr1 << 7) / (dh - 128));
    SetDeepSpot(legacy::map_state::get_current_map(), x, y, r0, r1 - r0 + 1, dh);
}

static void ClearGoodDeepSpot(int x, int y, int r, int dr, int dh) {
    if (r <= 0) return;
    int dr1 = (dr * (dh - 128)) / (143 - 128);
    if (dr1 >= r) { dh = 128 + ((dh - 128) * dr1) / r; dr = r; }
    const int r0 = r - dr1;
    const int r1 = r + ((dr1 << 7) / (dh - 128));
    ClearDeepSpot(legacy::map_state::get_current_map(), x, y, r0, r1 - r0 + 1, dh);
}

void SetWaterSpot(int x, int y, int r) {
    if (r <= 0) r = 1;
    const int dr = 16 << 4; // CostThickness << 4 with CostThickness =~16
    SetGoodDeepSpot(x, y, r << 5, dr, 170);
}

void EraseWaterSpot(int x, int y, int r) {
    const int dr = 16 << 4;
    ClearGoodDeepSpot(x, y, r << 5, dr, 170);
}

} } // namespace legacy::water_compat


