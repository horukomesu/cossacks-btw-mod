add_library(world_render STATIC
  src/WorldRender.cpp
  src/Terrain.cpp
  src/Decor.cpp
)

target_include_directories(world_render
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

find_package(glfw3 CONFIG REQUIRED)

if (WIN32)
  target_link_libraries(world_render PUBLIC engine_core legacy_adapter resource_io glad_gl_core_33 opengl32 glfw)
else()
  find_package(OpenGL REQUIRED)
  target_link_libraries(world_render PUBLIC engine_core legacy_adapter resource_io glad_gl_core_33 OpenGL::GL glfw)
endif()

target_compile_features(world_render PUBLIC cxx_std_20)


