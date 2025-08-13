#pragma once

#include <cstdint>
#include <string>

namespace resource_io::terrain_meta {

// Flags copied conceptually from original textures.lst semantics
enum TexFlag : uint16_t {
    TEX_NONE                 = 0,
    TEX_ALWAYS_WATER_UNLOCK  = 1 << 0,
    TEX_ALWAYS_LAND_LOCK     = 1 << 1,
    TEX_ALWAYS_LAND_UNLOCK   = 1 << 2,
    TEX_PLAIN                = 1 << 3,
    TEX_NORMALPUT            = 1 << 4,
    TEX_HARD                 = 1 << 5,
    TEX_HARDLIGHT            = 1 << 6,
    TEX_NOLIGHT              = 1 << 7,
};

// Global tables mirroring original: provide simple accessors
const uint8_t* get_tile_map_16x16();           // 256 entries of digits 0..9
const uint16_t* get_tex_flags_256();           // 256 entries of TexFlag
const uint8_t* get_tex_media_256();            // 256 entries of media id
const uint16_t* get_road_tex_256();            // 256 remap indices
const uint8_t (*get_ext_tex_256x4())[4];       // 256 rows of 4 indices

// Load metadata from default files (Tiling.txt, textures.lst, fract_set.txt)
// Returns true on success; partially true if some files missing (tables kept at defaults like in original)
bool load_from_files(const std::string& tilingPath = "Tiling.txt",
                     const std::string& texturesListPath = "textures.lst",
                     const std::string& fractSetPath = "fract_set.txt");

} // namespace resource_io::terrain_meta


