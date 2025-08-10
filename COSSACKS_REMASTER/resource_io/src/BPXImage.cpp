#include "../include/resource_io/BPXImage.hpp"

#include "../include/resource_io/ResourceIO.hpp"

#include <cctype>
#include <cstring>
#include <cstdint>
#include <string>
#include <vector>

namespace resource_io { namespace bpximg {

static bool ends_with_ci(const std::string& s, const char* suff) {
    const size_t n = s.size();
    const size_t m = std::strlen(suff);
    if (m > n) return false;
    for (size_t i = 0; i < m; ++i) {
        char a = static_cast<char>(std::tolower(static_cast<unsigned char>(s[n - m + i])));
        char b = static_cast<char>(std::tolower(static_cast<unsigned char>(suff[i])));
        if (a != b) return false;
    }
    return true;
}

// Minimal BMP header structs (packed)
#pragma pack(push, 1)
struct BMPHeader {
    uint16_t bfType;      // 'BM'
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;   // offset to pixel array
    uint32_t biSize;      // DIB header size (expected 40)
    int32_t  biWidth;
    int32_t  biHeight;    // positive = bottom-up
    uint16_t biPlanes;    // 1
    uint16_t biBitCount;  // 8 expected
    uint32_t biCompression; // 0 = BI_RGB
    uint32_t biSizeImage; // can be 0 for BI_RGB
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;   // 0 or 256
    uint32_t biClrImportant;
};
#pragma pack(pop)

bool load_bpx_or_bmp_indexed(const std::string& path, int& outW, int& outH, std::vector<uint8_t>& outIndices) {
    outW = 0; outH = 0; outIndices.clear();

    std::string resolved;
    // Try exact first; otherwise search by suffix in archives
    std::vector<unsigned char> file;
    if (!resource_io::read_file_anywhere(path, file)) {
        // Attempt case-insensitive suffix search for legacy assets
        if (!resource_io::try_find_entry_by_suffix(path, resolved)) {
            return false;
        }
        if (!resource_io::read_file_anywhere(resolved, file)) return false;
    }

    // Detect format by extension or content
    bool isBmp = ends_with_ci(path, ".bmp");
    bool isBpx = ends_with_ci(path, ".bpx");
    if (!isBmp && !isBpx) {
        // Try to infer by content: BMP begins with 'BM' (0x42,0x4D) in little endian 'MB' in some legacy code
        if (file.size() >= 2 && file[0] == 'B' && file[1] == 'M') isBmp = true;
        else isBpx = true;
    }

    if (isBpx) {
        if (file.size() < 4) return false;
        const uint16_t w = static_cast<uint16_t>(file[0] | (static_cast<uint16_t>(file[1]) << 8));
        const uint16_t h = static_cast<uint16_t>(file[2] | (static_cast<uint16_t>(file[3]) << 8));
        const size_t need = static_cast<size_t>(w) * static_cast<size_t>(h);
        if (file.size() < 4 + need) return false;
        outW = w; outH = h;
        outIndices.resize(need);
        std::memcpy(outIndices.data(), file.data() + 4, need);
        return true;
    }

    // BMP 8-bit paletted
    if (file.size() < sizeof(BMPHeader)) return false;
    const BMPHeader* hdr = reinterpret_cast<const BMPHeader*>(file.data());
    if (!(hdr->bfType == 0x4D42 /*'BM'*/)) return false;
    if (hdr->biBitCount != 8) return false; // Only 8-bit paletted supported per legacy usage
    const int width = hdr->biWidth;
    const int height = hdr->biHeight;
    if (width <= 0 || height == 0) return false;
    const bool bottomUp = (height > 0);
    const int absHeight = bottomUp ? height : -height;
    const size_t offBits = hdr->bfOffBits;
    // Row is padded to 4-byte boundary
    const size_t rowSize = static_cast<size_t>((width + 3) & ~3u);
    const size_t need = static_cast<size_t>(absHeight) * rowSize;
    if (file.size() < offBits + need) return false;
    outW = width;
    outH = absHeight;
    outIndices.resize(static_cast<size_t>(width) * static_cast<size_t>(absHeight));
    for (int y = 0; y < absHeight; ++y) {
        const int srcY = bottomUp ? (absHeight - 1 - y) : y;
        const size_t srcOff = offBits + static_cast<size_t>(srcY) * rowSize;
        std::memcpy(outIndices.data() + static_cast<size_t>(y) * static_cast<size_t>(width), file.data() + srcOff, static_cast<size_t>(width));
    }
    return true;
}

void expand_indices_to_rgba(const uint8_t* indices, int w, int h, const uint8_t* rgba256x4, std::vector<uint8_t>& outRGBA) {
    outRGBA.resize(static_cast<size_t>(w) * static_cast<size_t>(h) * 4);
    uint8_t* dst = outRGBA.data();
    for (int i = 0, n = w * h; i < n; ++i) {
        const uint8_t idx = indices[i];
        const size_t p = static_cast<size_t>(idx) * 4;
        dst[i * 4 + 0] = rgba256x4[p + 0];
        dst[i * 4 + 1] = rgba256x4[p + 1];
        dst[i * 4 + 2] = rgba256x4[p + 2];
        dst[i * 4 + 3] = (idx == 0) ? 0 : rgba256x4[p + 3];
    }
}

void expand_indices_to_rgba_opaque(const uint8_t* indices, int w, int h, const uint8_t* rgba256x4, std::vector<uint8_t>& outRGBA) {
    outRGBA.resize(static_cast<size_t>(w) * static_cast<size_t>(h) * 4);
    uint8_t* dst = outRGBA.data();
    for (int i = 0, n = w * h; i < n; ++i) {
        const uint8_t idx = indices[i];
        const size_t p = static_cast<size_t>(idx) * 4;
        dst[i * 4 + 0] = rgba256x4[p + 0];
        dst[i * 4 + 1] = rgba256x4[p + 1];
        dst[i * 4 + 2] = rgba256x4[p + 2];
        dst[i * 4 + 3] = 255; // force opaque
    }
}

}} // namespace resource_io::bpximg


