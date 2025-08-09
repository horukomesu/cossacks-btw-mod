#include "resource_io/RLC.hpp"

#include "resource_io/ResourceIO.hpp"

#include <cstring>

namespace resource_io::rlc {

#pragma pack(push, 1)
struct RLCHeader {
    int16_t SizeX;
    int16_t SizeY;
};
struct xRLCTable {
    int Size;
    int sign;
    int SCount;
    int OfsTable[1]; // variable-sized
};
#pragma pack(pop)

static inline const xRLCTable* asTable(const RLCTableData& d) {
    return reinterpret_cast<const xRLCTable*>(d.bytes.data());
}

bool load_rlc(const std::string& path, RLCTableData& out) {
    std::vector<unsigned char> data;
    if (!read_file_anywhere(path, data)) return false;
    out.bytes.assign(data.begin(), data.end());
    const xRLCTable* t = asTable(out);
    out.subImageCount = (t->SCount & 0xFFFF);
    return true;
}

bool get_subimage_size(const RLCTableData& tbl, int index, int& outW, int& outH) {
    const xRLCTable* t = asTable(tbl);
    if (index < 0 || index >= (t->SCount & 0xFFFF)) return false;
    const uint8_t* base = tbl.bytes.data();
    const int offs = t->OfsTable[index];
    const auto* hdr = reinterpret_cast<const RLCHeader*>(base + static_cast<size_t>(offs));
    outW = static_cast<int>(static_cast<uint16_t>(hdr->SizeX));
    outH = static_cast<int>(static_cast<uint16_t>(hdr->SizeY));
    return true;
}

bool get_subimage_anchor(const RLCTableData& tbl, int index, int& outDx, int& outDy) {
    // Original RLC header does not store dx/dy; anchors are part of GP format.
    // Return zeros to indicate top-left.
    (void)tbl; (void)index;
    outDx = 0; outDy = 0;
    return true;
}

// TODO: implement true colorization and palette/national-mask support.
// Current implementation emits white RGB with alpha on non-zero indices.
bool decode_subimage_rgba(const RLCTableData& tbl, int index, std::vector<uint8_t>& outRGBA) {
    const xRLCTable* t = asTable(tbl);
    if (index < 0 || index >= (t->SCount & 0xFFFF)) return false;
    const uint8_t* base = tbl.bytes.data();
    const int offs = t->OfsTable[index];
    const uint8_t* ptr = base + static_cast<size_t>(offs);
    const RLCHeader* hdr = reinterpret_cast<const RLCHeader*>(ptr);
    const int width = static_cast<int>(static_cast<uint16_t>(hdr->SizeX));
    const int height = static_cast<int>(static_cast<uint16_t>(hdr->SizeY));
    ptr += sizeof(RLCHeader);

    outRGBA.assign(static_cast<size_t>(width * height) * 4, 0);

    for (int y = 0; y < height; ++y) {
        if (ptr >= base + tbl.bytes.size()) break;
        uint8_t numSegments = *ptr++;
        int x = 0;
        for (uint8_t s = 0; s < numSegments; ++s) {
            if (ptr + 2 > base + tbl.bytes.size()) break;
            uint8_t skip = *ptr++;
            x += skip;
            uint8_t run = *ptr++;
            for (int i = 0; i < run; ++i) {
                if (ptr >= base + tbl.bytes.size()) break;
                uint8_t v = *ptr++;
                const int px = x + i;
                if (px >= 0 && px < width) {
                    const size_t idx = static_cast<size_t>(y * width + px) * 4;
                    outRGBA[idx + 0] = 255;
                    outRGBA[idx + 1] = 255;
                    outRGBA[idx + 2] = 255;
                    outRGBA[idx + 3] = (v != 0) ? 255 : 0;
                }
            }
            x += run;
        }
    }
    return true;
}

bool decode_subimage_indexed(const RLCTableData& tbl, int index, std::vector<uint8_t>& outIndices) {
    const xRLCTable* t = asTable(tbl);
    if (index < 0 || index >= (t->SCount & 0xFFFF)) return false;
    const uint8_t* base = tbl.bytes.data();
    const int offs = t->OfsTable[index];
    const uint8_t* ptr = base + static_cast<size_t>(offs);
    const RLCHeader* hdr = reinterpret_cast<const RLCHeader*>(ptr);
    const int width = static_cast<int>(static_cast<uint16_t>(hdr->SizeX));
    const int height = static_cast<int>(static_cast<uint16_t>(hdr->SizeY));
    ptr += sizeof(RLCHeader);

    outIndices.assign(static_cast<size_t>(width * height), 0);

    for (int y = 0; y < height; ++y) {
        if (ptr >= base + tbl.bytes.size()) break;
        uint8_t numSegments = *ptr++;
        int x = 0;
        for (uint8_t s = 0; s < numSegments; ++s) {
            if (ptr + 2 > base + tbl.bytes.size()) break;
            uint8_t skip = *ptr++;
            x += skip;
            uint8_t run = *ptr++;
            for (int i = 0; i < run; ++i) {
                if (ptr >= base + tbl.bytes.size()) break;
                uint8_t v = *ptr++;
                const int px = x + i;
                if (px >= 0 && px < width) {
                    outIndices[static_cast<size_t>(y * width + px)] = v;
                }
            }
            x += run;
        }
    }
    return true;
}

bool decode_subimage_rgba_with_palette(const RLCTableData& tbl, int index, const uint8_t* paletteRGBA256x4, std::vector<uint8_t>& outRGBA) {
    std::vector<uint8_t> idx;
    if (!decode_subimage_indexed(tbl, index, idx)) return false;
    int w = 0, h = 0;
    if (!get_subimage_size(tbl, index, w, h)) return false;
    outRGBA.resize(static_cast<size_t>(w * h) * 4);
    for (int i = 0; i < w * h; ++i) {
        const uint8_t indexVal = idx[static_cast<size_t>(i)];
        const size_t p = static_cast<size_t>(indexVal) * 4;
        const size_t o = static_cast<size_t>(i) * 4;
        outRGBA[o + 0] = paletteRGBA256x4[p + 0];
        outRGBA[o + 1] = paletteRGBA256x4[p + 1];
        outRGBA[o + 2] = paletteRGBA256x4[p + 2];
        outRGBA[o + 3] = (indexVal == 0) ? 0 : paletteRGBA256x4[p + 3];
    }
    return true;
}

} // namespace resource_io::rlc
