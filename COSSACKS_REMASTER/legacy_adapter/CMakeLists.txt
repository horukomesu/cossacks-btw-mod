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
    src/Sound_compat.cpp
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

# Integrate GLAD directly (no find_package)
set(GLAD_ROOT "${CMAKE_SOURCE_DIR}/extern/vcpkg/packages/glad_x64-windows")
set(GLAD_INCLUDE_DIR "${GLAD_ROOT}/include")
set(GLAD_LIB_DEBUG "${GLAD_ROOT}/debug/lib/glad.lib")
set(GLAD_LIB_RELEASE "${GLAD_ROOT}/lib/glad.lib")

if(WIN32)
    if(NOT EXISTS ${GLAD_INCLUDE_DIR}/glad/glad.h)
        message(FATAL_ERROR "GLAD header not found at ${GLAD_INCLUDE_DIR}. Please ensure GLAD is available at extern/vcpkg/packages/glad_x64-windows")
    endif()
    if(NOT EXISTS ${GLAD_LIB_RELEASE})
        message(FATAL_ERROR "GLAD release library not found at ${GLAD_LIB_RELEASE}")
    endif()
    if(NOT EXISTS ${GLAD_LIB_DEBUG})
        set(GLAD_LIB_DEBUG ${GLAD_LIB_RELEASE})
    endif()
    target_include_directories(legacy_adapter PUBLIC ${GLAD_INCLUDE_DIR})
else()
    message(FATAL_ERROR "Non-Windows builds currently require a platform-appropriate GLAD integration. Provide GLAD include/lib paths or add a platform section here.")
endif()

# Link against engine_core (for window accessor) and OpenGL deps for headers
if (WIN32)
    target_link_libraries(legacy_adapter PRIVATE
        engine_core audio_core glfw
        $<$<CONFIG:Debug>:${GLAD_LIB_DEBUG}>
        $<$<CONFIG:Release>:${GLAD_LIB_RELEASE}>
        $<$<CONFIG:RelWithDebInfo>:${GLAD_LIB_RELEASE}>
        $<$<CONFIG:MinSizeRel>:${GLAD_LIB_RELEASE}>
        opengl32)
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(legacy_adapter PRIVATE engine_core audio_core glfw OpenGL::GL)
endif()

if(MSVC)
  target_compile_options(legacy_adapter PRIVATE /W4 /Zc:__cplusplus)
  target_compile_definitions(legacy_adapter PRIVATE NOMINMAX)
else()
  target_compile_options(legacy_adapter PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_compile_features(legacy_adapter PUBLIC cxx_std_20)
