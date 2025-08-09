#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace resource_io { namespace bpximg {

// Load either a legacy .bpx file (2-byte width, 2-byte height, then width*height indices)
// or an 8-bit paletted .bmp file, returning width, height and raw 8-bit index pixels (row-major, top-left origin).
// Returns false if file not found or unsupported format.
bool load_bpx_or_bmp_indexed(const std::string& path, int& outW, int& outH, std::vector<uint8_t>& outIndices);

// Utility: expand 8-bit indices to RGBA using a provided 256x4 RGBA palette; index 0 -> alpha 0.
void expand_indices_to_rgba(const uint8_t* indices, int w, int h, const uint8_t* rgba256x4, std::vector<uint8_t>& outRGBA);

} } // namespace resource_io::bpximg


