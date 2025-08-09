#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace legacy::gp {

// Minimal compatible GP system wrapper providing GPS.ShowGP and size queries
class GPSCompat {
public:
    // Draw sprite by GPID and sub-index (Nation ignored for now)
    void ShowGP(int x, int y, int gpid, int sprIndex, uint8_t nation);
    void ShowGPLayers(int x, int y, int gpid, int sprIndex, uint8_t nation, int mask);

    // Return sprite dimensions; returns 0 if invalid
    int GetGPWidth(int gpid, int sprIndex);
    int GetGPHeight(int gpid, int sprIndex);
    int GetGPShift(int gpid, int sprIndex);

    // Set anchor overrides from GP metadata (dx,dy arrays) for a GPID
    void SetAnchors(int gpid, const std::vector<int8_t>& itdx, const std::vector<int8_t>& itlx);

    // Draw sprite with palette remap table (256 bytes). Table maps source index->new index.
    void ShowGPPal(int x, int y, int gpid, int sprIndex, uint8_t nation, const uint8_t* indexMap256);

    // Resolve and register resource name to a GPID (e.g., "Interface\\Main_Menu")
    // Returns >=0 GPID on success, -1 on failure
    int LocalGP_Load(const std::string& name);

    // Resolve registered resource path for given GPID; returns false if not found
    bool ResolveGPIDPath(int gpid, std::string& outPath);
};

extern GPSCompat GPS;

// Helper to mirror original LocalGP convenience type
struct LocalGP {
    int GPID{-1};
    explicit LocalGP(const char* name) : GPID(GPS.LocalGP_Load(name)) {}
};

// Free-function shims for legacy code that calls ShowGP directly
void ShowGP(int x, int y, int fileIndex, int sprIndex, uint8_t nation);
void FlipPages();
void SetRLCWindow(int x, int y, int lx, int ly, int slx);
// Solid fill rectangle with palette index color
void CBar(int x, int y, int Lx, int Ly, uint8_t c);
// Legacy blit; in our GL path acts as no-op (rendering is immediate)
void CopyToScreen(int x, int y, int Lx, int Ly);

} // namespace legacy::gp
