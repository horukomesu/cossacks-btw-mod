#include "resource_io/ColorPalette.hpp"

#include "resource_io/ResourceIO.hpp"

#include <array>
#include <cstring>
#include <vector>

namespace resource_io::color_palette {

namespace {
std::array<uint8_t, 256 * 4> g_palette{};
bool g_loaded = false;

void make_identity_grayscale() {
    for (int i = 0; i < 256; ++i) {
        g_palette[i * 4 + 0] = static_cast<uint8_t>(i);
        g_palette[i * 4 + 1] = static_cast<uint8_t>(i);
        g_palette[i * 4 + 2] = static_cast<uint8_t>(i);
        g_palette[i * 4 + 3] = 255;
    }
}
}

void initialize() {
    make_identity_grayscale();
    g_loaded = false;
}

bool load_from_path(const std::string& path) {
    std::vector<unsigned char> data;
    if (!resource_io::read_file_anywhere(path, data)) return false;
    if (data.size() >= 1024) {
        // RGBA 256*4
        std::memcpy(g_palette.data(), data.data(), 1024);
        g_loaded = true;
        return true;
    } else if (data.size() >= 768) {
        // RGB 256*3
        for (int i = 0; i < 256; ++i) {
            g_palette[i * 4 + 0] = data[static_cast<size_t>(i) * 3 + 0];
            g_palette[i * 4 + 1] = data[static_cast<size_t>(i) * 3 + 1];
            g_palette[i * 4 + 2] = data[static_cast<size_t>(i) * 3 + 2];
            g_palette[i * 4 + 3] = 255;
        }
        g_loaded = true;
        return true;
    }
    return false;
}

const uint8_t* get_rgba256x4() {
    return g_palette.data();
}

bool set_from_rgba(const uint8_t* rgba256x4) {
    if (!rgba256x4) return false;
    std::memcpy(g_palette.data(), rgba256x4, 256 * 4);
    g_loaded = true;
    return true;
}

} // namespace resource_io::color_palette


