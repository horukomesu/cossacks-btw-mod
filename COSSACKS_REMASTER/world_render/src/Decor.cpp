#include "world_render/Decor.hpp"
#include "world_render/WorldRender.hpp"
#include "legacy/Globals_compat.hpp"
#include "legacy/SpriteCache.hpp"

namespace world_render { namespace decor {

namespace {
const resource_io::map::MapSceneData* gScene = nullptr;
}

void set_from_scene(const resource_io::map::MapSceneData* scene) {
    gScene = scene;
}

void submit_frame() {
    if (!gScene) return;
    for (const auto& ds : gScene->decorSprites) {
        // Map EERT signs to stems per original: 'GA' -> trees, 'TS' -> stones, 'OH' -> holes, 'OC' -> complex
        const uint16_t sign = ds.sign;
        const char* stem = nullptr;
        if (sign == 'GA') stem = "treelist";
        else if (sign == 'TS') stem = "stonlist";
        else if (sign == 'OH') stem = "holelist";
        else if (sign == 'OC') stem = "complex";
        if (!stem) continue;
        // World coords in original are pixel-like; convert to tile coords (divide by 32 horizontally, 16 vertically in iso metric)
        const int worldX = ds.x >> 5; // approximate tile X
        const int worldY = ds.y >> 5; // approximate tile Y
        const int frameIndex = static_cast<int>(ds.sgIndex);
        world_render::submit_sprite_gp(stem, frameIndex, worldX, worldY, /*elevHalfPixels*/0, /*zBias*/0.0f,
                                       /*paletteId*/0, Layer::TerrainDecor, 1.0f,1.0f,1.0f,1.0f);
    }
}

} } // namespace world_render::decor


