#pragma once

#include <cstdint>

namespace legacy { namespace palette_compat {

// Sets the current color palette to all black (alpha 255). Used before fades.
void SetDarkPalette();

// Loads a palette file (768-byte RGB or 1024-byte RGBA). In the legacy
// implementation this performs a fade-in; here we load immediately and
// invalidate cached palettized textures so they are recreated with the new palette.
void SlowLoadPalette(const char* lpFileName);

// Performs a fade-out in legacy; here we switch to a black palette and
// invalidate cached palettized textures.
void SlowUnLoadPalette(const char* lpFileName);

} } // namespace legacy::palette_compat



