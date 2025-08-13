add_library(resource_io STATIC
    src/ResourceIO.cpp
    src/ResFile.cpp
    src/Arc.cpp
    src/RLC.cpp
    src/Palettes.cpp
    src/ColorPalette.cpp
    src/GP.cpp
    src/BPXImage.cpp
    src/Map.cpp
    src/TexturesMeta.cpp
)

target_include_directories(resource_io
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(resource_io PUBLIC cxx_std_20)

if(MSVC)
  target_compile_options(resource_io PRIVATE /W4 /Zc:__cplusplus)
else()
  target_compile_options(resource_io PRIVATE -Wall -Wextra -Wpedantic)
endif()
