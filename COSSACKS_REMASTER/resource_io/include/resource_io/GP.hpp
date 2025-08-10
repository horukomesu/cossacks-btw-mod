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

// Decoded 8-bit index frame (0 is transparent)
struct GPFrameIndices {
    int width{0};
    int height{0};
    int minx{0};
    int miny{0};
    std::vector<uint8_t> indices; // width*height, row-major
};

// Load baseName + ".gp" into GPFile; returns false if not found or invalid
bool load_gp(const std::string& baseName, GPFile& out);

// Retrieve per-picture meta (dx,dy,lx,ly, options, next) for index [0..numPictures)
bool get_frame_meta(const GPFile& file, int index, GPHeaderMeta& out);

// Load baseName + ".gpi" per-frame arrays (ItDX/ItLX). Returns false if not found.
// Output vectors sized to file.numPictures if possible.
bool load_gpi(const std::string& baseName, int expectedCount, std::vector<int8_t>& outItDX, std::vector<int8_t>& outItLX);

// Compute aggregate bounds across all sub-chunks of a frame without decoding pixel data
// Returns false on invalid index or malformed data
bool compute_frame_bounds(const GPFile& file, int index, int& outMinX, int& outMinY, int& outMaxX, int& outMaxY);

// Decode a frame to 8-bit indices (0 transparent). Also returns aggregate bounds
bool decode_frame_indices(const GPFile& file, int index, GPFrameIndices& out);


} // namespace resource_io::gp


