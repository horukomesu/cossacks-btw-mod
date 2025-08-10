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
    # Debug lib may not exist if single-config was installed; guard it
    if(NOT EXISTS ${GLAD_LIB_DEBUG})
        set(GLAD_LIB_DEBUG ${GLAD_LIB_RELEASE})
    endif()
    target_include_directories(engine_core PUBLIC ${GLAD_INCLUDE_DIR})
else()
    message(FATAL_ERROR "Non-Windows builds currently require a platform-appropriate GLAD integration. Provide GLAD include/lib paths or add a platform section here.")
endif()

if (WIN32)
    target_link_libraries(engine_core PRIVATE
        glfw
        $<$<CONFIG:Debug>:${GLAD_LIB_DEBUG}>
        $<$<CONFIG:Release>:${GLAD_LIB_RELEASE}>
        $<$<CONFIG:RelWithDebInfo>:${GLAD_LIB_RELEASE}>
        $<$<CONFIG:MinSizeRel>:${GLAD_LIB_RELEASE}>
        opengl32)
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(engine_core PRIVATE glfw OpenGL::GL)
endif()

if(MSVC)
  target_compile_options(engine_core PRIVATE /W4 /Zc:__cplusplus)
else()
  target_compile_options(engine_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_compile_features(engine_core PUBLIC cxx_std_20)
