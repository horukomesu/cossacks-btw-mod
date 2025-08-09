add_library(engine_core STATIC
    src/EngineCore.cpp
    src/InputAdapter.cpp
    src/Render2D.cpp
    src/Texture.cpp
)

target_include_directories(engine_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/legacy_adapter/include
)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

if (WIN32)
    target_link_libraries(engine_core PRIVATE glfw glad::glad opengl32)
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(engine_core PRIVATE glfw glad::glad OpenGL::GL)
endif()

if(MSVC)
  target_compile_options(engine_core PRIVATE /W4 /Zc:__cplusplus)
else()
  target_compile_options(engine_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_compile_features(engine_core PUBLIC cxx_std_20)
