#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace engine_core { namespace texture { using TextureHandle = unsigned int; } }

namespace legacy {

// Minimal compatible SQPicture used by legacy DialogsSystem.
// Stores 8-bit indices buffer with dimensions. Provides Draw and DrawTransparent using modern renderer.
class SQPictureCompat {
public:
    SQPictureCompat() = default;
    explicit SQPictureCompat(const char* path) { LoadPicture(path); }

    // Load BMP(8-bit) or BPX (2-byte w, 2-byte h, followed by indices)
    void LoadPicture(const char* path);

    // Immediate draw at x,y using current color palette (true-color). Index 0 is transparent.
    void Draw(int x, int y) const;             // opaque blit (non-zero overwrites)
    void DrawTransparent(int x, int y) const;  // same as Draw, kept for API parity

    int GetLx() const { return width; }
    int GetLy() const { return height; }

    // Expose raw pointer layout [w,h,indices...] (word*) like original.
    const uint16_t* getLegacyPicPtr() const;

private:
    int width{0};
    int height{0};
    std::vector<uint8_t> compatBuffer; // 4-byte header (w,h) + indices (w*h)
    mutable engine_core::texture::TextureHandle texture{0};
    mutable bool textureDirty{true};
};

} // namespace legacy


