add_library(legacy_adapter STATIC
    src/Fastdraw.cpp
    src/GP_Draw.cpp
    src/Ddini.cpp
    src/Time_compat.cpp
    src/gfile_compat.cpp
    src/SpriteCache.cpp
    src/SQPicture_compat.cpp
    src/DialogsSystem_compat.cpp
    src/Text_compat.cpp
    src/TextDB_compat.cpp
    src/RLCFont_compat.cpp
    src/FontsRegistry_compat.cpp
    src/Input_compat.cpp
    src/Globals_compat.cpp
    src/MainMenu_compat.cpp
    src/MouseCursor_compat.cpp
    src/Palette_compat.cpp
    src/Fog_compat.cpp
)

target_include_directories(legacy_adapter
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_SOURCE_DIR}/resource_io/include
        ${CMAKE_SOURCE_DIR}/engine_core/include
        ${CMAKE_SOURCE_DIR}/audio_core/include
)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

# Link against engine_core (for window accessor) and OpenGL deps for headers
if (WIN32)
    target_link_libraries(legacy_adapter PRIVATE engine_core audio_core glfw glad::glad opengl32)
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(legacy_adapter PRIVATE engine_core audio_core glfw glad::glad OpenGL::GL)
endif()

if(MSVC)
  target_compile_options(legacy_adapter PRIVATE /W4 /Zc:__cplusplus)
  target_compile_definitions(legacy_adapter PRIVATE NOMINMAX)
else()
  target_compile_options(legacy_adapter PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_compile_features(legacy_adapter PUBLIC cxx_std_20)
