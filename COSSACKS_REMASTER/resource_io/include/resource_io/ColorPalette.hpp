#pragma once

#include <cstdint>
#include <string>

namespace resource_io::color_palette {

// Initialize color palette subsystem and try to load a default palette.
void initialize();

// Try to load palette from path (e.g., "2\\agew_1.pal" or "agew_1.pal").
// Supports 768-byte (RGB) and 1024-byte (RGBA) formats. Returns true on success.
bool load_from_path(const std::string& path);

// Get current RGBA palette (256*4 bytes). If not loaded, returns identity grayscale.
const uint8_t* get_rgba256x4();

// Set current palette from RGBA bytes (256*4). Returns true on success.
bool set_from_rgba(const uint8_t* rgba256x4);

} // namespace resource_io::color_palette


