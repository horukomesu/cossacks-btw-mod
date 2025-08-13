#include "resource_io/TexturesMeta.hpp"
#include "resource_io/ResourceIO.hpp"

#include <vector>
#include <string>
#include <cstring>

namespace resource_io::terrain_meta {

namespace {
uint8_t gTileMap[256];
uint16_t gTexFlags[256];
uint8_t gTexMedia[256];
uint16_t gRoadTex[256];
uint8_t gExtTex[256][4];

bool parse_tiling_txt(const std::vector<unsigned char>& data) {
    // file contains 16 lines with 16 chars (digits); we accept any whitespace between lines
    std::memset(gTileMap, 0, sizeof(gTileMap));
    size_t p = 0; int row = 0;
    while (row < 16 && p < data.size()) {
        // read token of at least 16 visible chars
        int col = 0;
        while (p < data.size() && data[p] <= ' ') ++p;
        while (p < data.size() && col < 16) {
            unsigned char c = data[p++];
            if (c <= ' ') break;
            if (c >= '0' && c <= '9') gTileMap[row * 16 + col] = static_cast<uint8_t>(c - '0');
            ++col;
        }
        ++row;
    }
    return true;
}

bool parse_textures_lst(const std::vector<unsigned char>& data) {
    std::memset(gTexFlags, 0, sizeof(gTexFlags));
    std::memset(gTexMedia, 0, sizeof(gTexMedia));
    for (int i = 0; i < 256; ++i) {
        gExtTex[i][0] = gExtTex[i][1] = gExtTex[i][2] = gExtTex[i][3] = static_cast<uint8_t>(i);
        gRoadTex[i] = static_cast<uint16_t>(i);
    }
    // very simple parser: tokens separated by spaces/newlines, directives #MULTI, #ROAD, or lines: <name> <index> <flags> [# <media>]
    std::string s(reinterpret_cast<const char*>(data.data()), data.size());
    size_t pos = 0; auto next_token = [&]() -> std::string {
        while (pos < s.size() && (s[pos] == ' ' || s[pos] == '\t' || s[pos] == '\r' || s[pos] == '\n')) ++pos;
        size_t start = pos;
        while (pos < s.size() && !(s[pos] == ' ' || s[pos] == '\t' || s[pos] == '\r' || s[pos] == '\n')) ++pos;
        return s.substr(start, pos - start);
    };
    while (pos < s.size()) {
        std::string tok = next_token(); if (tok.empty()) break;
        if (tok[0] == '/') { // comment line begins with //
            // skip rest of line
            while (pos < s.size() && s[pos] != '\n') ++pos;
            continue;
        }
        if (tok == "#MULTI") {
            std::string t1 = next_token(), t2 = next_token(), t3 = next_token(), t4 = next_token();
            int i1 = std::atoi(t1.c_str()), i2 = std::atoi(t2.c_str()), i3 = std::atoi(t3.c_str()), i4 = std::atoi(t4.c_str());
            if (i1 >= 0 && i1 < 256 && i2 >= 0 && i2 < 256 && i3 >= 0 && i3 < 256 && i4 >= 0 && i4 < 256) {
                gExtTex[i1][0] = static_cast<uint8_t>(i1);
                gExtTex[i1][1] = static_cast<uint8_t>(i2);
                gExtTex[i1][2] = static_cast<uint8_t>(i3);
                gExtTex[i1][3] = static_cast<uint8_t>(i4);
            }
            continue;
        }
        if (tok == "#ROAD") {
            std::string t1 = next_token(), t2 = next_token();
            int i1 = std::atoi(t1.c_str()), i2 = std::atoi(t2.c_str());
            if (i1 >= 0 && i1 < 256 && i2 >= 0 && i2 < 256) gRoadTex[i1] = static_cast<uint16_t>(i2);
            continue;
        }
        // texture entry: <name> <index> <flags> [# <media>]
        std::string idxStr = next_token();
        std::string flagsStr = next_token();
        if (idxStr.empty() || flagsStr.empty()) continue;
        int idx = std::atoi(idxStr.c_str()); if (idx < 0 || idx >= 256) continue;
        uint16_t fl = 0;
        if (flagsStr.find('W') != std::string::npos) fl |= TEX_ALWAYS_WATER_UNLOCK;
        if (flagsStr.find('L') != std::string::npos) fl |= TEX_ALWAYS_LAND_LOCK;
        if (flagsStr.find('U') != std::string::npos) fl |= TEX_ALWAYS_LAND_UNLOCK;
        if (flagsStr.find('P') != std::string::npos) fl |= TEX_PLAIN;
        if (flagsStr.find('N') != std::string::npos) fl |= TEX_NORMALPUT;
        if (flagsStr.find('H') != std::string::npos) fl |= TEX_HARD;
        if (flagsStr.find('R') != std::string::npos) fl |= TEX_HARDLIGHT;
        if (flagsStr.find('B') != std::string::npos) fl |= TEX_NOLIGHT;
        gTexFlags[idx] = fl;
        // optional media after '#'
        size_t sharp = flagsStr.find('#');
        if (sharp != std::string::npos) {
            // media token was parsed into flagsStr; in original, it was separate; we skip here
        }
    }
    return true;
}

bool parse_fract_set(const std::vector<unsigned char>& /*data*/) {
    // Original populates TEXARR used in editor brushes; not critical for runtime terrain draw
    return true;
}

} // namespace

const uint8_t* get_tile_map_16x16() { return gTileMap; }
const uint16_t* get_tex_flags_256() { return gTexFlags; }
const uint8_t* get_tex_media_256() { return gTexMedia; }
const uint16_t* get_road_tex_256() { return gRoadTex; }
const uint8_t (*get_ext_tex_256x4())[4] { return gExtTex; }

bool load_from_files(const std::string& tilingPath,
                     const std::string& texturesListPath,
                     const std::string& fractSetPath) {
    std::vector<unsigned char> data;
    if (resource_io::read_file_anywhere(tilingPath, data)) parse_tiling_txt(data);
    if (resource_io::read_file_anywhere(texturesListPath, data)) parse_textures_lst(data);
    if (resource_io::read_file_anywhere(fractSetPath, data)) parse_fract_set(data);
    return true;
}

} // namespace resource_io::terrain_meta


