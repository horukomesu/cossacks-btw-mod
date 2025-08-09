#include "legacy/SQPicture_compat.hpp"

#include "engine_core/Render2D.hpp"
#include "engine_core/Texture.hpp"
#include "resource_io/BPXImage.hpp"
#include "resource_io/ColorPalette.hpp"

#include <vector>

namespace legacy {

void SQPictureCompat::LoadPicture(const char* path) {
    width = 0;
    height = 0;
    compatBuffer.clear();
    if (texture) {
        engine_core::texture::destroy_texture(texture);
        texture = 0;
    }

    int w = 0, h = 0;
    std::vector<uint8_t> idx;
    if (!resource_io::bpximg::load_bpx_or_bmp_indexed(std::string(path), w, h, idx)) {
        return;
    }
    width = w;
    height = h;
    compatBuffer.resize(static_cast<size_t>(w) * static_cast<size_t>(h) + 4u);
    // header in little endian shorts
    compatBuffer[0] = static_cast<uint8_t>(w & 0xFF);
    compatBuffer[1] = static_cast<uint8_t>((w >> 8) & 0xFF);
    compatBuffer[2] = static_cast<uint8_t>(h & 0xFF);
    compatBuffer[3] = static_cast<uint8_t>((h >> 8) & 0xFF);
    std::memcpy(compatBuffer.data() + 4, idx.data(), static_cast<size_t>(w) * static_cast<size_t>(h));
    textureDirty = true;
}

void SQPictureCompat::Draw(int x, int y) const {
    if (width <= 0 || height <= 0 || compatBuffer.size() < 4u) return;
    if (texture == 0 || textureDirty) {
        const uint8_t* baseRGBA = resource_io::color_palette::get_rgba256x4();
        std::vector<uint8_t> rgba;
        resource_io::bpximg::expand_indices_to_rgba(compatBuffer.data() + 4, width, height, baseRGBA, rgba);
        if (texture) engine_core::texture::destroy_texture(texture);
        texture = engine_core::texture::create_texture_rgba(width, height, rgba.data());
        textureDirty = false;
    }
    engine_core::render2d::draw_textured_quad(texture, static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height));
}

void SQPictureCompat::DrawTransparent(int x, int y) const {
    // Our index 0 already maps to alpha=0 in Expand, so this is identical to Draw.
    Draw(x, y);
}

const uint16_t* SQPictureCompat::getLegacyPicPtr() const {
    if (width <= 0 || height <= 0 || compatBuffer.size() < 4u) return nullptr;
    // Return pointer to [w,h] short header located at start of compatBuffer
    return reinterpret_cast<const uint16_t*>(compatBuffer.data());
}

} // namespace legacy


