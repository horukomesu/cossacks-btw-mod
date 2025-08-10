#include "resource_io/GP.hpp"

#include "resource_io/ResourceIO.hpp"

#include <cstring>
#include <algorithm>

namespace resource_io::gp {

#pragma pack(push, 1)
struct GP_GlobalHeaderBin {
    int32_t Sign;
    int16_t NPictures;
    int16_t Reserved;
    int32_t VocOffset;
    int16_t VocLength;
    int32_t LGPH[256];
};
// On-disk GP header (packed, 23 bytes). IMPORTANT: 'Pack' is a 32-bit value in file layout,
// not a native pointer size. Do NOT use a pointer-sized field here.
struct GP_HeaderFileBin {
    int32_t NextPict;
    int16_t dx;
    int16_t dy;
    int16_t Lx;
    int16_t Ly;
    uint32_t Pack; // pointer value in original runtime; unused here
    uint8_t Options;
    uint32_t CData;
    int16_t NLines;
};
#pragma pack(pop)

static inline const GP_GlobalHeaderBin* as_global(const GPFile& f) {
    return reinterpret_cast<const GP_GlobalHeaderBin*>(f.bytes.data());
}

bool load_gp(const std::string& baseName, GPFile& out) {
    std::vector<unsigned char> data;
    if (!resource_io::read_file_anywhere(baseName + ".gp", data)) {
        // Try uppercase suffix lookup inside archives
        std::string r;
        if (resource_io::try_find_entry_by_suffix(baseName + ".GP", r)) {
            if (!resource_io::read_file_anywhere(r, data)) return false;
        } else {
            return false;
        }
    }
    out.bytes.assign(data.begin(), data.end());
    const auto* gh = as_global(out);
    out.numPictures = gh->NPictures;
    return true;
}

bool get_frame_meta(const GPFile& file, int index, GPHeaderMeta& out) {
    const auto* gh = as_global(file);
    if (index < 0 || index >= gh->NPictures) return false;
    const uint8_t* base = file.bytes.data();
    const int32_t offs = gh->LGPH[index];
    const auto* hdr = reinterpret_cast<const GP_HeaderFileBin*>(base + static_cast<size_t>(offs));
    out.dx = hdr->dx;
    out.dy = hdr->dy;
    out.lx = hdr->Lx;
    out.ly = hdr->Ly;
    out.options = hdr->Options;
    out.nextPict = hdr->NextPict;
    return true;
}

bool load_gpi(const std::string& baseName, int expectedCount, std::vector<int8_t>& outItDX, std::vector<int8_t>& outItLX) {
    std::vector<unsigned char> data;
    if (!resource_io::read_file_anywhere(baseName + ".gpi", data)) return false;
    if (static_cast<int>(data.size()) < expectedCount * 2) return false;
    outItDX.resize(expectedCount);
    outItLX.resize(expectedCount);
    std::memcpy(outItDX.data(), data.data(), static_cast<size_t>(expectedCount));
    std::memcpy(outItLX.data(), data.data() + static_cast<size_t>(expectedCount), static_cast<size_t>(expectedCount));
    return true;
}

static inline uint32_t u32_le(const uint8_t* p) {
    return p[0] | (uint32_t(p[1]) << 8) | (uint32_t(p[2]) << 16) | (uint32_t(p[3]) << 24);
}

static inline uint16_t u16_le(const uint8_t* p) {
    return p[0] | (uint16_t(p[1]) << 8);
}

static void decode_std_unpack(const uint8_t* src, size_t srcLen, size_t destLen, const uint8_t* voc, size_t vocLen, std::vector<uint8_t>& out) {
    out.assign(destLen, 0);
    size_t si = 0;
    size_t di = 0;
    while (di < destLen && si < srcLen) {
        uint8_t flags = src[si++];
        for (int bit = 0; bit < 8 && di < destLen; ++bit) {
            if (flags & 0x80) {
                if (si + 1 >= srcLen) return;
                uint16_t ctrl = uint16_t(src[si]) | (uint16_t(src[si + 1]) << 8);
                si += 2;
                size_t length = ((ctrl >> 12) & 0xF) + 3;
                size_t ofs = (ctrl & 0x0FFF);
                if (vocLen == 0) {
                    // Nothing to copy, advance
                    size_t toCopy = std::min(length, destLen - di);
                    for (size_t k = 0; k < toCopy; ++k) out[di++] = 0;
                } else {
                    if (ofs >= vocLen) ofs %= std::max<size_t>(1, vocLen);
                    size_t toCopy = std::min(length, destLen - di);
                    size_t available = std::min(toCopy, vocLen - ofs);
                    std::memcpy(out.data() + di, voc + ofs, available);
                    di += available;
                    // If requested more than available, wrap or pad zeros
                    while (available < toCopy) {
                        size_t more = std::min(vocLen, toCopy - available);
                        if (more == 0) break;
                        std::memcpy(out.data() + di, voc, more);
                        di += more;
                        available += more;
                    }
                }
            } else {
                if (si >= srcLen || di >= destLen) return;
                out[di++] = src[si++];
            }
            flags <<= 1;
        }
    }
}

static void decode_lz_unpack(const uint8_t* src, size_t srcLen, size_t destLen, std::vector<uint8_t>& out) {
    out.assign(destLen, 0);
    size_t si = 0;
    size_t di = 0;
    while (di < destLen && si < srcLen) {
        uint8_t flags = src[si++];
        for (int bit = 0; bit < 8 && di < destLen; ++bit) {
            if (flags & 1) {
                if (si + 1 >= srcLen) return;
                uint16_t ctrl = uint16_t(src[si]) | (uint16_t(src[si + 1]) << 8);
                si += 2;
                size_t ofs = (ctrl & 0x1FFF);
                size_t length = (ctrl >> 13) + 3;
                // Copy from already written output
                size_t srcIndex = (di > ofs + 1) ? (di - ofs - 1) : 0;
                for (size_t k = 0; k < length && di < destLen; ++k) {
                    uint8_t v = (srcIndex < di) ? out[srcIndex] : 0;
                    out[di++] = v;
                    ++srcIndex;
                }
            } else {
                if (si >= srcLen || di >= destLen) return;
                out[di++] = src[si++];
            }
            flags >>= 1;
        }
    }
}

static void decode_nat_unpack(const uint8_t* src, size_t srcLen, size_t destLen, std::vector<uint8_t>& out) {
    out.assign(destLen, 0);
    size_t si = 0;
    size_t di = 0;
    size_t need = destLen / 4;
    for (size_t t = 0; t < need && si < srcLen; ++t) {
        uint8_t a = src[si++];
        uint8_t b0 = (a & 0x03);
        uint8_t b1 = (a >> 2) & 0x03;
        uint8_t b2 = (a >> 4) & 0x03;
        uint8_t b3 = (a >> 6) & 0x03;
        if (di + 4 <= destLen) {
            out[di + 0] = b0;
            out[di + 1] = b1;
            out[di + 2] = b2;
            out[di + 3] = b3;
        }
        di += 4;
    }
}

static void decode_grey_unpack(const uint8_t* src, size_t srcLen, size_t destLen, std::vector<uint8_t>& out) {
    out.assign(destLen, 0);
    size_t si = 0;
    size_t di = 0;
    size_t need = destLen / 2;
    for (size_t t = 0; t < need && si < srcLen; ++t) {
        uint8_t a = src[si++];
        uint8_t low = uint8_t((a & 0x0F) << 1);
        uint8_t high = uint8_t(((a >> 4) & 0x0F) >> 3);
        if (di + 2 <= destLen) {
            out[di] = low;
            out[di + 1] = high;
        }
        di += 2;
    }
}

static bool compute_bounds(const GPFile& file, int index, int& outMinX, int& outMinY, int& outMaxX, int& outMaxY) {
    const auto* gh = as_global(file);
    if (index < 0 || index >= gh->NPictures) return false;
    const uint8_t* base = file.bytes.data();
    const uint32_t frameOff = gh->LGPH[index];
    if (frameOff >= file.bytes.size()) return false;
    int minx = 1'000'000;
    int miny = 1'000'000;
    int maxx = -1'000'000;
    int maxy = -1'000'000;
    uint32_t pos = frameOff;
    for (;;) {
        if (pos + sizeof(GP_HeaderFileBin) > file.bytes.size()) break;
        const auto* hdr = reinterpret_cast<const GP_HeaderFileBin*>(base + pos);
        minx = std::min<int>(minx, hdr->dx);
        miny = std::min<int>(miny, hdr->dy);
        maxx = std::max<int>(maxx, hdr->dx + hdr->Lx);
        maxy = std::max<int>(maxy, hdr->dy + hdr->Ly);
        if (hdr->NextPict == -1) break;
        pos = frameOff + static_cast<uint32_t>(hdr->NextPict);
    }
    if (minx > maxx || miny > maxy) return false;
    outMinX = minx;
    outMinY = miny;
    outMaxX = maxx;
    outMaxY = maxy;
    return true;
}

bool compute_frame_bounds(const GPFile& file, int index, int& outMinX, int& outMinY, int& outMaxX, int& outMaxY) {
    return compute_bounds(file, index, outMinX, outMinY, outMaxX, outMaxY);
}

bool decode_frame_indices(const GPFile& file, int index, GPFrameIndices& out) {
    const auto* gh = as_global(file);
    if (index < 0 || index >= gh->NPictures) return false;
    const uint8_t* base = file.bytes.data();
    const uint32_t frameOff = gh->LGPH[index];
    if (frameOff >= file.bytes.size()) return false;
    int minx = 0, miny = 0, maxx = 0, maxy = 0;
    if (!compute_bounds(file, index, minx, miny, maxx, maxy)) return false;
    const int width = std::max(1, maxx - minx);
    const int height = std::max(1, maxy - miny);
    std::vector<uint8_t> canvas(static_cast<size_t>(width) * static_cast<size_t>(height), 0);
    // VOC block if present
    const auto* ghb = as_global(file);
    const uint8_t* voc = (ghb->VocLength > 0) ? (base + static_cast<size_t>(ghb->VocOffset)) : nullptr;
    const size_t vocLen = (ghb->VocLength > 0) ? static_cast<size_t>(ghb->VocLength) : 0;

    uint32_t pos = frameOff;
    for (;;) {
        if (pos + sizeof(GP_HeaderFileBin) > file.bytes.size()) break;
        const auto* hdr = reinterpret_cast<const GP_HeaderFileBin*>(base + pos);
        const int16_t dx = hdr->dx;
        const int16_t dy = hdr->dy;
        const int16_t Lx = hdr->Lx;
        const int16_t Ly = hdr->Ly;
        const uint8_t opt = hdr->Options;
        uint32_t cdata = hdr->CData;
        int nlines = hdr->NLines;
        // UnpackLen in bits [31:14] across CData plus opt extensions
        size_t unpackLen = (cdata >> 14) & 0x3FFFF;
        size_t cdoffs = cdata & 0x3FFF;
        if (opt & 64) cdoffs += 16384; // add 2^14
        if (opt & 128) cdoffs += 32768; // add 2^15
        const uint8_t optCode = (opt & 63);
        if (optCode == 43) unpackLen += 262144; // +2^18
        else if (optCode == 44) unpackLen += 524288; // +2^19
        const size_t srcPtr = static_cast<size_t>(pos) + static_cast<size_t>(cdoffs);
        const uint8_t* src = (srcPtr < file.bytes.size()) ? (base + srcPtr) : nullptr;
        const size_t srcLen = (srcPtr < file.bytes.size()) ? (file.bytes.size() - srcPtr) : 0;
        std::vector<uint8_t> cdataBytes;
        switch (optCode) {
            case 0: // std unpack with VOC
                if (src && srcLen) decode_std_unpack(src, srcLen, unpackLen, voc, vocLen, cdataBytes);
                else cdataBytes.assign(unpackLen, 0);
                break;
            case 42: // LZ unpack
            case 43:
            case 44:
                if (src && srcLen) decode_lz_unpack(src, srcLen, unpackLen, cdataBytes);
                else cdataBytes.assign(unpackLen, 0);
                break;
            case 41: // raw copy of given unpackLen bytes
                if (src && srcLen) cdataBytes.assign(src, src + std::min(srcLen, unpackLen));
                else cdataBytes.assign(unpackLen, 0);
                break;
            case 1: // nat unpack 2-bit -> 8-bit indices
                if (src && srcLen) decode_nat_unpack(src, srcLen, unpackLen, cdataBytes);
                else cdataBytes.assign(unpackLen, 0);
                break;
            case 38: // grey unpack -> pairs
                if (src && srcLen) decode_grey_unpack(src, srcLen, unpackLen, cdataBytes);
                else cdataBytes.assign(unpackLen, 0);
                break;
            default:
                // Fallback: best-effort copy of declared length
                if (src && srcLen) cdataBytes.assign(src, src + std::min(srcLen, unpackLen));
                else cdataBytes.assign(unpackLen, 0);
                break;
        }
        // Now decode scanlines: line descriptors start at pos+23 (sizeof on-disk header)
        size_t linePtr = pos + sizeof(GP_HeaderFileBin);
        size_t cdpos = 0;
        for (int line = 0; line < nlines; ++line) {
            if (linePtr >= file.bytes.size()) break;
            uint8_t head = base[linePtr++];
            int x = 0;
            if (head & 0x80) {
                // Complex line
                int segs = (head & 31);
                int extBit = (head & 32) ? 16 : 0; // adds 16 to run length nibble
                for (int s = 0; s < segs; ++s) {
                    if (linePtr >= file.bytes.size()) break;
                    uint8_t b = base[linePtr++];
                    // Lower 4 bits: X delta, Upper 4 bits plus extBit: run length-1 per original; but the
                    // assembly uses (b>>4)|ext directly as count. We mirror that.
                    int xdelta = (b & 0x0F);
                    int ln = ((b >> 4) & 0x0F) | extBit;
                    int gx = x + xdelta;
                    int gy = (dy - miny) + line;
                    if (gy >= 0 && gy < height) {
                        for (int i = 0; i < ln; ++i) {
                            if (cdpos >= cdataBytes.size()) break;
                            int px = gx + i + (dx - minx);
                            if (px >= 0 && px < width) {
                                uint8_t v = cdataBytes[cdpos];
                                if (v) {
                                    canvas[static_cast<size_t>(gy) * static_cast<size_t>(width) + static_cast<size_t>(px)] = v;
                                }
                            }
                            ++cdpos;
                        }
                    } else {
                        cdpos = std::min(cdpos + static_cast<size_t>(std::max(0, ln)), cdataBytes.size());
                    }
                    x = gx + ln;
                }
            } else {
                // Simple line
                int segs = head;
                for (int s = 0; s < segs; ++s) {
                    if (linePtr + 1 >= file.bytes.size()) break;
                    int xdelta = base[linePtr + 0];
                    int ln = base[linePtr + 1];
                    linePtr += 2;
                    int gx = x + xdelta;
                    int gy = (dy - miny) + line;
                    if (gy >= 0 && gy < height) {
                        for (int i = 0; i < ln; ++i) {
                            if (cdpos >= cdataBytes.size()) break;
                            int px = gx + i + (dx - minx);
                            if (px >= 0 && px < width) {
                                uint8_t v = cdataBytes[cdpos];
                                if (v) {
                                    canvas[static_cast<size_t>(gy) * static_cast<size_t>(width) + static_cast<size_t>(px)] = v;
                                }
                            }
                            ++cdpos;
                        }
                    } else {
                        cdpos = std::min(cdpos + static_cast<size_t>(std::max(0, ln)), cdataBytes.size());
                    }
                    x = gx + ln;
                }
            }
        }
        if (hdr->NextPict == -1) break;
        pos = frameOff + static_cast<uint32_t>(hdr->NextPict);
    }

    out.width = width;
    out.height = height;
    out.minx = minx;
    out.miny = miny;
    out.indices = std::move(canvas);
    return true;
}

} // namespace resource_io::gp


