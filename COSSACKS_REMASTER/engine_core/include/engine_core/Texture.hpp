#pragma once

#include <cstdint>

namespace engine_core::texture {

using TextureHandle = unsigned int; // GL texture name

// Create a GL texture from RGBA8 pixels (row-major, tightly packed)
TextureHandle create_texture_rgba(int width, int height, const uint8_t* pixels);

void destroy_texture(TextureHandle tex);

} // namespace engine_core::texture
