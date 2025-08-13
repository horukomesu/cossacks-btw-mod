#include "legacy/ScenePostInit_compat.hpp"

#include "legacy/MapState_compat.hpp"
#include "legacy/Path_compat.hpp"
#include "legacy/Water_compat.hpp"
#include "world_render/Terrain.hpp"
#include "world_render/Decor.hpp"
#include <iostream>

namespace legacy { namespace scene_post_init {

void after_map_loaded(resource_io::map::MapData& map) {
    // Set active map for compat layers to access WaterDeep/WaterBright
    legacy::map_state::set_current_map(&map);

    // Initialize motion fields and grid params based on loaded map
    legacy::path_compat::initialize_from_params({ map.addShift, map.mapTilesX, map.mapTilesY });

    // Build initial water passability for whole map
    legacy::water_compat::rebuild_water_locking_full();

    std::cout << "[scene] after_map_loaded: VIL=" << map.vertInLine << " MTH=" << map.maxTH << std::endl;
    // Bind terrain arrays to renderer first
    world_render::terrain::set_map_data(map.heights.data(), map.texIndices.data(), map.vertInLine, map.maxTH);
    std::cout << "[scene] after_map_loaded: terrain ready (after set_map_data)=" << (world_render::terrain::is_ready()?1:0) << std::endl;
    // Ensure terrain GPU resources (tile atlas, palette, masks) are initialized
    (void)world_render::terrain::initialize_tiles();

    // Hooks for future parity with original post-load chain (no-ops/covered elsewhere):
    // - ClearRender/CreateMiniMap: renderer-based approach; minimap generation may be implemented later
    // - ClearTrianglesSystem/CreateTrianglesSystem: terrain renderer pulls directly from arrays
    // - CreateWTopMap/GTOP wiring/ResearchCurrentIsland: to be implemented when topology is ported
    // - CreateCostPlaces: to be implemented under game_logic when AI/economy is ported
}

void after_scene_loaded(const resource_io::map::MapSceneData& scene) {
    // Use minimal MapData view to wire path/water/terrain; decor goes via scene
    resource_io::map::MapData md;
    md.addShift = scene.addShift;
    md.mapTilesX = scene.mapTilesX;
    md.mapTilesY = scene.mapTilesY;
    md.maxTH = scene.maxTH;
    md.vertInLine = scene.vertInLine;
    md.heights = scene.heights;
    md.texIndices = scene.texIndices;
    after_map_loaded(md);
    // Provide SectMap (TCES) for mask selection in terrain renderer
    if (!scene.sectMap.empty()) {
        world_render::terrain::set_sect_map(scene.sectMap.data(), scene.maxSector, scene.maxTH);
    }
    // Decor sprites (static) from EERT
    world_render::decor::set_from_scene(&scene);
}

} } // namespace legacy::scene_post_init


