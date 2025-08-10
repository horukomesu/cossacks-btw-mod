#include "legacy/Palette_compat.hpp"

#include "resource_io/ColorPalette.hpp"
#include "legacy/SpriteCache.hpp"

#include <array>
#include <cstring>

namespace legacy { namespace palette_compat {

static void set_palette_rgba_bytes(const uint8_t* rgba256x4) {
    (void)resource_io::color_palette::set_from_rgba(rgba256x4);
}

void SetDarkPalette() {
    // Build all-black RGBA palette (alpha 255, index 0 alpha 0)
    std::array<uint8_t, 256 * 4> dark{};
    for (int i = 0; i < 256; ++i) {
        dark[i * 4 + 0] = 0;
        dark[i * 4 + 1] = 0;
        dark[i * 4 + 2] = 0;
        dark[i * 4 + 3] = (i == 0) ? 0 : 255;
    }
    set_palette_rgba_bytes(dark.data());
    // Palettized sprite cache depends on palette; flush it
    legacy::sprite_cache::clear();
}

void SlowLoadPalette(const char* lpFileName) {
    (void)lpFileName;
    // Legacy does fade-in; we load instantaneous palette from path to match visuals closely
    // and then clear caches so GP/RLC textures remap using the new palette.
    // Try exact path first; if fails, attempt base file name as well is handled by ResourceIO.
    if (lpFileName && *lpFileName) {
        resource_io::color_palette::load_from_path(lpFileName);
    }
    legacy::sprite_cache::clear();
}

void SlowUnLoadPalette(const char* /*lpFileName*/) {
    // Legacy fades to black. Emulate by switching to dark palette immediately.
    SetDarkPalette();
}

} } // namespace legacy::palette_compat



