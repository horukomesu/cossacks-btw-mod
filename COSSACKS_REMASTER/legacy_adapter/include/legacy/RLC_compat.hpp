// Compatibility layer for legacy RLC loading and rendering API.
// Provides original-like types and functions while rendering via the remaster's GL path.

#pragma once

#include <cstdint>

namespace legacy { namespace rlc_compat {

using byte = std::uint8_t;
using word = std::uint16_t;

#pragma pack(push, 1)
struct RLCHeader {
    int16_t SizeX;
    int16_t SizeY;
};
struct xRLCTable {
    int Size;
    int sign;
    int SCount;
    int OfsTable[1]; // variable-sized tail
};
#pragma pack(pop)

using RLCTable = xRLCTable*;
using lpRLCTable = RLCTable*;

// Load legacy RLC table; mirrors original LoadRLC semantics (alloc + absolute offsets)
bool LoadRLC(const char* fileName, RLCTable* outRlcTable);

// Low-level draw routines (subset):
void ShowRLC(int x, int y, void* picPtr);
void ShowRLCi(int x, int y, void* picPtr);
void ShowRLCpal(int x, int y, void* picPtr, byte* pal /*256 remap table*/);
void ShowRLCipal(int x, int y, void* picPtr, byte* pal /*256 remap table*/);

// High-level dispatch identical to original signature (uses GP first when GPID<4096)
void ShowRLCItem(int x, int y, lpRLCTable lprt, int n, byte nt);

// Convenience variants from original API (map to ShowRLC/ShowRLCpal internally)
void ShowRLCItemPal(int x, int y, lpRLCTable lprt, int n, byte* Pal);
void ShowRLCItemTrans8(int x, int y, lpRLCTable lprt, int n);
void ShowRLCItemDarkN(int x, int y, lpRLCTable lprt, int n, int Ints);
void ShowRLCItemRedN(int x, int y, lpRLCTable lprt, int n, int Ints);
void ShowRLCItemDark(int x, int y, lpRLCTable lprt, int n);
void ShowRLCItemBlue(int x, int y, lpRLCTable lprt, int n);
void ShowRLCItemFired(int x, int y, lpRLCTable lprt, int n);

// Query image dimensions (original-compatible, falling back to GP when GPID<4096)
int GetRLCWidth(RLCTable lpr, byte n);
int GetRLCHeight(RLCTable lpr, byte n);

// Palettes bootstrap (no-op shim; ensures palette tables ready in ResourceIO)
void LoadPalettes();

} } // namespace legacy::rlc_compat


