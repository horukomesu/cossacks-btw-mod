#pragma once

#include <cstdint>
#include <vector>

// Compatibility layer for original MotionField/MFIELDS/UnitsField and barrier checks.
// Mirrors data layout and semantics from @Main executable/path.cpp without assembly,
// preserving memory indexing (vertical bitfield) and API contracts.

namespace legacy { namespace path_compat {

struct MotionField {
    // MapV is a vertical bitfield with layout identical to original:
    // byte index = (x << MAPSHF) + (y >> 3), bit = 1 << (y & 7)
    std::vector<uint8_t> MapV;

    void allocate(int mapsx, int mapsy, int mapshf);
    void free_alloc();
    void clear_maps();

    // Point operations (pixel grid unit in legacy logic: 1 point = 16x16 world pix at ADDSH=1)
    void BSetPt(int x, int y);
    void BClrPt(int x, int y);

    // Square/rectangle set/clear
    void BSetBar(int x, int y, int Lx);
    void BClrBar(int x, int y, int Lx);
    void BSetSQ(int x, int y, int Lx, int Ly);
    void BClrSQ(int x, int y, int Lx, int Ly);

    // Checks
    int  CheckPt(int x, int y) const;          // returns 0 if free, non-zero if blocked
    int  CheckHLine(int x, int y, int Lx) const;
    int  CheckVLine(int x, int y, int Lx) const;
    bool CheckBar(int x, int y, int Lx, int Ly) const;

private:
    // Cached params from initialize_from_map
    int m_mapsx{0};
    int m_mapsy{0};
    int m_mapshf{0};
    int m_bmsx{0};
    inline bool in_bounds(int x, int y) const { return x >= 0 && x < m_mapsx && y >= 0 && y < m_mapsy; }
    inline size_t byte_index(int x, int y) const { return static_cast<size_t>((x << m_mapshf) + (y >> 3)); }
    inline uint8_t bit_mask(int y) const { return static_cast<uint8_t>(1u << (y & 7)); }
};

// Global motion fields (0-land, 1-water), and per-unit dynamic field (UnitsField), as in original.
extern MotionField MFIELDS[2];
extern MotionField UnitsField;

// Global grid parameters copied from original map setup (see MapDiscr.h notes)
extern int ADDSH;   // 1..3
extern int MAPSX;   // 240 << ADDSH
extern int MAPSY;   // = MAPSX
extern int MAPSHF;  // 6 + ADDSH
extern int BMSX;    // 64 << ADDSH
extern int MaxWX;   // MAPSX >> 1
extern int MaxWY;   // MAPSY >> 1

// Initialize motion fields and globals from loaded map parameters
struct InitParams {
    int addShift;     // map.addShift
    int mapTilesX;    // map.mapTilesX
    int mapTilesY;    // map.mapTilesY
};
void initialize_from_params(const InitParams& p);

// Convenience wrappers matching legacy free functions
void BSetBar(int x, int y, int Lx);
void BClrBar(int x, int y, int Lx);
bool CheckBar(int x, int y, int Lx, int Ly);

} } // namespace legacy::path_compat


