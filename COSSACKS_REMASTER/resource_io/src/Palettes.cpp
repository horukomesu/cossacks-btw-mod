#include "resource_io/Palettes.hpp"

#include "resource_io/ResourceIO.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <vector>

namespace resource_io::palettes {

namespace {
std::array<std::array<uint8_t, 256>, 8> g_tables{};
bool g_inited = false;
// NatPal from original: 16 base indices, each nation uses 4-color strip
std::array<uint8_t, 16> g_uiNatPal{ {
    0xD0, 0xD4, 0xD8, 0xDC, 0xE0, 0xE4, 0xE8, 0xEC,
    // Remaining colors could be custom; mirror original order
    0xD0, 0xD4, 0xD8, 0xDC, 0xE0, 0xE4, 0xE8, 0xEC
} };

void load_or_identity(int slot, const char* fileName) {
    std::vector<unsigned char> data;
    if (resource_io::read_file_anywhere(fileName, data) && data.size() >= 256) {
        for (size_t i = 0; i < 256; ++i) g_tables[slot][i] = static_cast<uint8_t>(data[i]);
    } else {
        for (size_t i = 0; i < 256; ++i) g_tables[slot][i] = static_cast<uint8_t>(i);
    }
}
} // namespace

void initialize() {
    if (g_inited) return;
    // Identity at 0
    for (size_t i = 0; i < 256; ++i) g_tables[0][i] = static_cast<uint8_t>(i);
    // pal1..pal7 at 1..7
    load_or_identity(1, "pal1.dat");
    load_or_identity(2, "pal2.dat");
    load_or_identity(3, "pal3.dat");
    load_or_identity(4, "pal4.dat");
    load_or_identity(5, "pal5.dat");
    load_or_identity(6, "pal6.dat");
    load_or_identity(7, "pal7.dat");
    g_inited = true;
}

const uint8_t* get_table(int idx) {
    if (!g_inited) initialize();
    if (idx < 0 || idx >= static_cast<int>(g_tables.size())) idx = 0;
    return g_tables[static_cast<size_t>(idx)].data();
}

const uint8_t* get_ui_nation_palette16() {
    if (!g_inited) initialize();
    return g_uiNatPal.data();
}

} // namespace resource_io::palettes


