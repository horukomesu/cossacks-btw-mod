#include "legacy/Path_compat.hpp"

#include <algorithm>
#include <cstring>

namespace legacy { namespace path_compat {

MotionField MFIELDS[2];
MotionField UnitsField;

int ADDSH = 1;
int MAPSX = 240 << 1;
int MAPSY = 240 << 1;
int MAPSHF = 6 + 1;
int BMSX = 64 << 1;
int MaxWX = 0;
int MaxWY = 0;

void MotionField::allocate(int mapsx, int mapsy, int mapshf) {
    m_mapsx = mapsx;
    m_mapsy = mapsy;
    m_mapshf = mapshf;
    m_bmsx = 1 << m_mapshf; // equals 64 << ADDSH like original
    MapV.assign(static_cast<size_t>(m_mapsy) * static_cast<size_t>(m_bmsx), 0);
}

void MotionField::free_alloc() {
    MapV.clear();
    MapV.shrink_to_fit();
    m_mapsx = m_mapsy = m_mapshf = m_bmsx = 0;
}

void MotionField::clear_maps() {
    std::fill(MapV.begin(), MapV.end(), 0);
}

void MotionField::BSetPt(int x, int y) {
    if (!in_bounds(x, y)) return;
    const size_t idx = byte_index(x, y);
    if (idx >= MapV.size()) return;
    MapV[idx] = static_cast<uint8_t>(MapV[idx] | bit_mask(y));
}

void MotionField::BClrPt(int x, int y) {
    if (!in_bounds(x, y)) return;
    const size_t idx = byte_index(x, y);
    if (idx >= MapV.size()) return;
    MapV[idx] = static_cast<uint8_t>(MapV[idx] & ~bit_mask(y));
}

void MotionField::BSetBar(int x, int y, int Lx) {
    for (int ix = x; ix < x + Lx; ++ix) {
        for (int iy = y; iy < y + Lx; ++iy) {
            BSetPt(ix, iy);
        }
    }
}

void MotionField::BClrBar(int x, int y, int Lx) {
    for (int ix = x; ix < x + Lx; ++ix) {
        for (int iy = y; iy < y + Lx; ++iy) {
            BClrPt(ix, iy);
        }
    }
}

void MotionField::BSetSQ(int x, int y, int Lx, int Ly) {
    for (int ix = x; ix < x + Lx; ++ix) {
        for (int iy = y; iy < y + Ly; ++iy) {
            BSetPt(ix, iy);
        }
    }
}

void MotionField::BClrSQ(int x, int y, int Lx, int Ly) {
    for (int ix = x; ix < x + Lx; ++ix) {
        for (int iy = y; iy < y + Ly; ++iy) {
            BClrPt(ix, iy);
        }
    }
}

int MotionField::CheckPt(int x, int y) const {
    if (!in_bounds(x, y)) return 1; // legacy returns non-zero for out-of-bounds
    const size_t idx = byte_index(x, y);
    if (idx >= MapV.size()) return 1;
    const uint8_t bit = bit_mask(y);
    return (MapV[idx] & bit);
}

int MotionField::CheckHLine(int x, int y, int Lx) const {
    for (int i = 0; i < Lx; ++i) {
        if (CheckPt(x + i, y)) return 1;
    }
    return 0;
}

int MotionField::CheckVLine(int x, int y, int Lx) const {
    // Optimize by checking in chunks of up to 24 like original; here simple loop is fine.
    if (Lx <= 0) return 0;
    for (int i = 0; i < Lx; ++i) {
        if (CheckPt(x, y + i)) return 1;
    }
    return 0;
}

bool MotionField::CheckBar(int x, int y, int Lx, int Ly) const {
    for (int ix = 0; ix < Lx; ++ix) {
        if (CheckVLine(x + ix, y, Ly)) return true;
    }
    return false;
}

void initialize_from_params(const InitParams& p) {
    ADDSH = std::clamp(p.addShift, 1, 3);
    MAPSX = 240 << ADDSH;
    MAPSY = MAPSX;
    MAPSHF = 6 + ADDSH;
    BMSX = 64 << ADDSH;
    MaxWX = MAPSX >> 1;
    MaxWY = MAPSY >> 1;

    for (int i = 0; i < 2; ++i) {
        MFIELDS[i].free_alloc();
        MFIELDS[i].allocate(MAPSX, MAPSY, MAPSHF);
        MFIELDS[i].clear_maps();
    }
    UnitsField.free_alloc();
    UnitsField.allocate(MAPSX, MAPSY, MAPSHF);
    UnitsField.clear_maps();
}

void BSetBar(int x, int y, int Lx) { MFIELDS[0].BSetBar(x, y, Lx); }
void BClrBar(int x, int y, int Lx) { MFIELDS[0].BClrBar(x, y, Lx); }
bool CheckBar(int x, int y, int Lx, int Ly) { return MFIELDS[0].CheckBar(x, y, Lx, Ly); }

} } // namespace legacy::path_compat


