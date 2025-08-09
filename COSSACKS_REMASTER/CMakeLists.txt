cmake_minimum_required(VERSION 3.20)

project(COSSACKS_REMASTER VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Disallow in-source builds
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not supported. Use a separate build directory.")
endif()

option(ENABLE_WARNINGS "Enable compiler warnings" ON)

add_subdirectory(engine_core)
add_subdirectory(resource_io)
add_subdirectory(game_logic)
add_subdirectory(legacy_adapter)
add_subdirectory(audio_core)

add_executable(cossacks_remaster src/main.cpp)

# Put the runtime (exe) into BUILD folder so resources can be colocated
set_target_properties(cossacks_remaster PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/BUILD"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/BUILD"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/BUILD"
)

# Set VS debugger working directory to BUILD
set_property(TARGET cossacks_remaster PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/BUILD")


target_link_libraries(cossacks_remaster PRIVATE engine_core resource_io game_logic legacy_adapter audio_core)

if(MSVC)
  target_compile_options(cossacks_remaster PRIVATE /permissive- /W4 /Zc:__cplusplus)
else()
  target_compile_options(cossacks_remaster PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Set startup project for Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT cossacks_remaster)
