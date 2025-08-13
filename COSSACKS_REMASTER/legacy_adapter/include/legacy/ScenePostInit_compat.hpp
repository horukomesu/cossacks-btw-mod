#pragma once

#include "resource_io/Map.hpp"
#include "resource_io/ResourceIO.hpp"
#include "world_render/Decor.hpp"

namespace legacy { namespace scene_post_init {

// Perform post-initialization sequence after loading a map, mirroring
// the original flow in @Main executable/SaveNewMap.cpp after Load3DMap():
// - Set current map for compat modules
// - Initialize global grid params and motion fields
// - Build initial water passability
// - Bind terrain data to renderer
// - Prepare minimap/triangulation hooks (where applicable)
//
// This function is production code and will be extended as more legacy
// subsystems are ported (topologies, islands, cost places, etc.).
void after_map_loaded(resource_io::map::MapData& map);
// Overload for full scene data to enable decor sprites, etc.
void after_scene_loaded(const resource_io::map::MapSceneData& scene);

} } // namespace legacy::scene_post_init


