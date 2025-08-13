#pragma once

#include <cstdint>

namespace legacy { namespace water_compat {

// Rebuild water locking (MFIELDS[1]) for entire map based on WaterDeep thresholds
void rebuild_water_locking_full();
// Rebuild water locking for a region in water cell coordinates (MaxWX x MaxWY)
void rebuild_water_locking_region(int x0, int y0, int x1, int y1);

// Editor-like water/decor operations from original RealWater.cpp
void SetWaterSpot(int x, int y, int r);
void EraseWaterSpot(int x, int y, int r);
void SetBrightSpot(int x, int y, int Brightness, bool increase);

} } // namespace legacy::water_compat


