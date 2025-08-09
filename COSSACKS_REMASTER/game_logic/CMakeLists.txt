add_library(game_logic STATIC
    src/GameLogic.cpp
)

target_include_directories(game_logic
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(game_logic PUBLIC cxx_std_20)

if(MSVC)
  target_compile_options(game_logic PRIVATE /W4 /Zc:__cplusplus)
else()
  target_compile_options(game_logic PRIVATE -Wall -Wextra -Wpedantic)
endif()
