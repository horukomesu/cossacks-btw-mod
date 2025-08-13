#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include "resource_io/Map.hpp"

namespace world_render::decor {

// Provide static decor sprites (EERT chunk) from a loaded scene.
void set_from_scene(const resource_io::map::MapSceneData* scene);

// Submit decor for the current frame. Call between begin_frame() and flush().
void submit_frame();

} // namespace world_render::decor


