#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace resource_io::rlc {

struct RLCTableData {
    // Raw file contents (RLC blob with header and offset table)
    std::vector<uint8_t> bytes;
    // Number of sub-images
    int subImageCount = 0;
    // Offsets to frames (for potential anchor extraction if extended)
};

// Load RLC file from archives/disk via ResourceIO; returns false if not found
bool load_rlc(const std::string& path, RLCTableData& out);

// Get sub-image pixel dimensions; returns false if index invalid
bool get_subimage_size(const RLCTableData& tbl, int index, int& outW, int& outH);

// Optional: get anchor/offsets for the sub-image if encoded (dx, dy). Returns false if not present.
bool get_subimage_anchor(const RLCTableData& tbl, int index, int& outDx, int& outDy);

// Decode sub-image to RGBA8 (opaque white where non-zero index, transparent elsewhere)
bool decode_subimage_rgba(const RLCTableData& tbl, int index, std::vector<uint8_t>& outRGBA);

// Decode sub-image to 8-bit indices (Size: width*height). 0 means transparent in legacy assets.
bool decode_subimage_indexed(const RLCTableData& tbl, int index, std::vector<uint8_t>& outIndices);

// Decode and expand using provided palette (256*4 RGBA entries). Index 0 => alpha 0.
bool decode_subimage_rgba_with_palette(const RLCTableData& tbl, int index, const uint8_t* paletteRGBA256x4, std::vector<uint8_t>& outRGBA);

} // namespace resource_io::rlc
