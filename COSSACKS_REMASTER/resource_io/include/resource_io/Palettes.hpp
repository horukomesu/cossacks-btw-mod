#pragma once

#include <array>
#include <cstdint>

namespace resource_io::palettes {

// Initialize and load pal1.dat..pal7.dat from archives/disk if present.
// Always provides identity table at index 0.
void initialize();

// Returns a pointer to a 256-byte remap table (identity if not loaded).
// idx in [0..7]; 0 => identity; 1..7 => pal1..pal7.
const uint8_t* get_table(int idx);

// Returns UI nation palette base indices (NatPal equivalent), array of 16 values.
// Each entry represents a 4-color strip base index in the 256-color palette.
const uint8_t* get_ui_nation_palette16();

} // namespace resource_io::palettes


