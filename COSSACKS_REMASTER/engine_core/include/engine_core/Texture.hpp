#pragma once

#include <cstdint>

namespace engine_core::texture {

using TextureHandle = unsigned int; // GL texture name

// Create a GL texture from RGBA8 pixels (row-major, tightly packed)
TextureHandle create_texture_rgba(int width, int height, const uint8_t* pixels);

// Create a GL texture from single-channel R8 pixels (row-major, tightly packed)
TextureHandle create_texture_r8(int width, int height, const uint8_t* pixels);

// Create a GL texture array (2D array) from single-channel R8 pixels.
// The input buffer must contain width*height*layers bytes laid out as layer 0, then layer 1, etc.
TextureHandle create_texture_r8_array(int width, int height, int layers, const uint8_t* pixels);

void destroy_texture(TextureHandle tex);

} // namespace engine_core::texture
