add_library(audio_core STATIC
    src/AudioCore.cpp
    src/Music.cpp
)

target_include_directories(audio_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_SOURCE_DIR}/resource_io/include
)

find_package(OpenAL CONFIG REQUIRED)

target_link_libraries(audio_core PRIVATE OpenAL::OpenAL resource_io)

if(MSVC)
  target_compile_options(audio_core PRIVATE /W4 /Zc:__cplusplus)
else()
  target_compile_options(audio_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_compile_features(audio_core PUBLIC cxx_std_20)

# minimp3 header
target_include_directories(audio_core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
