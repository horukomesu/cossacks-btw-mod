#include "resource_io/GP.hpp"

#include "resource_io/ResourceIO.hpp"

#include <cstring>

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
struct GP_HeaderBin {
    int32_t NextPict;
    int16_t dx;
    int16_t dy;
    int16_t Lx;
    int16_t Ly;
    uint8_t* Pack; // pointer in original; not used here
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
    if (!resource_io::read_file_anywhere(baseName + ".gp", data)) return false;
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
    const auto* hdr = reinterpret_cast<const GP_HeaderBin*>(base + static_cast<size_t>(offs));
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

} // namespace resource_io::gp


