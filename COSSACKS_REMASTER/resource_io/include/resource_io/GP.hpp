#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace resource_io::gp {

struct GPHeaderMeta {
    int16_t dx{0};
    int16_t dy{0};
    int16_t lx{0};
    int16_t ly{0};
    uint8_t options{0};
    int32_t nextPict{-1};
};

struct GPFile {
    std::vector<uint8_t> bytes;
    int numPictures{0};
};

// Load baseName + ".gp" into GPFile; returns false if not found or invalid
bool load_gp(const std::string& baseName, GPFile& out);

// Retrieve per-picture meta (dx,dy,lx,ly, options, next) for index [0..numPictures)
bool get_frame_meta(const GPFile& file, int index, GPHeaderMeta& out);

// Load baseName + ".gpi" per-frame arrays (ItDX/ItLX). Returns false if not found.
// Output vectors sized to file.numPictures if possible.
bool load_gpi(const std::string& baseName, int expectedCount, std::vector<int8_t>& outItDX, std::vector<int8_t>& outItLX);

} // namespace resource_io::gp


