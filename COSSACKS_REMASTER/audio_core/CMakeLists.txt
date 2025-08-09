add_library(audio_core STATIC
    src/AudioCore.cpp
)

target_include_directories(audio_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

find_package(OpenAL CONFIG REQUIRED)

target_link_libraries(audio_core PRIVATE OpenAL::OpenAL)

if(MSVC)
  target_compile_options(audio_core PRIVATE /W4 /Zc:__cplusplus)
else()
  target_compile_options(audio_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_compile_features(audio_core PUBLIC cxx_std_20)
