cmake_minimum_required(VERSION 3.20)
project(Cossacks_Raylib)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set up raylib paths
set(RAYLIB_PATH "C:/coding/libraries/raylib64")
include_directories(${RAYLIB_PATH}/include)
link_directories(${RAYLIB_PATH}/libs)

# Essential source files that actually exist
set(SOURCES
    src/raylib_main.cpp
    src/graphics/raylib_graphics.cpp
    src/graphics/directdraw_compat.cpp
    src/audio/raylib_audio.cpp
    src/audio/raylib_audio_compat.cpp
    src/raylib_system/raylib_window.cpp
    src/utils/raylib_utils.cpp
    src/cross_platform/platform_compat.cpp
    
    # Stub implementations for singleplayer mode
    "src/Main executable/stubs.cpp"
    
    # Main executable files that actually exist (SINGLEPLAYER ONLY)
    "src/Main executable/Ddex1.cpp"
    "src/Main executable/3DBars.cpp"
    "src/Main executable/3DGraph.cpp"
    "src/Main executable/3DMapEd.cpp"
    "src/Main executable/3DRandMap.cpp"
    "src/Main executable/3DSurf.cpp"
    "src/Main executable/ActiveScenary.cpp"
    "src/Main executable/ActiveZone.cpp"
    "src/Main executable/AntiBug.cpp"
    "src/Main executable/ArchTool.cpp"
    "src/Main executable/bmptool.cpp"
    "src/Main executable/Brigade.cpp"
    "src/Main executable/Build.cpp"
    "src/Main executable/Cdirsnd.cpp"
    "src/Main executable/Chkinet.cpp"
    "src/Main executable/ConstStr.cpp"
    "src/Main executable/Cwave.cpp"
    "src/Main executable/Danger.cpp"
    "src/Main executable/Ddini.cpp"
    "src/Main executable/DeviceCD.cpp"
    "src/Main executable/Dialogs.cpp"
    "src/Main executable/DrawForms.cpp"
    "src/Main executable/Economy.cpp"
    "src/Main executable/EinfoClass.cpp"
    "src/Main executable/ExConst.cpp"
    "src/Main executable/Fastdraw.cpp"
    "src/Main executable/fog.cpp"
    "src/Main executable/Fractal.cpp"
    "src/Main executable/GameSound.cpp"
    "src/Main executable/GP_Draw.cpp"
    "src/Main executable/Graphs.cpp"
    "src/Main executable/Groups.cpp"
    "src/Main executable/gText.cpp"
    "src/Main executable/GVClass.cpp"
    "src/Main executable/Hint.cpp"
    "src/Main executable/Icontool.cpp"
    "src/Main executable/Interface.cpp"
    "src/Main executable/IR_Main.cpp"
    "src/Main executable/Lines.cpp"
    "src/Main executable/LoadSave.cpp"
    "src/Main executable/mapa.cpp"
    "src/Main executable/MapSprites.cpp"
    "src/Main executable/Masks.cpp"
    "src/Main executable/Megapolis.cpp"
    "src/Main executable/menu.cpp"
    "src/Main executable/Mine.cpp"
    "src/Main executable/Motion.cpp"
    "src/Main executable/Mouse_X.cpp"
    "src/Main executable/MousZone.cpp"
    "src/Main executable/Nation.cpp"
    "src/Main executable/Nature.cpp"
    "src/Main executable/Navorots.cpp"
    "src/Main executable/NewAI.cpp"
    "src/Main executable/NewILogin.cpp"
    "src/Main executable/NewMon.cpp"
    "src/Main executable/NewUpgrade.cpp"
    "src/Main executable/Nuclear.cpp"
    "src/Main executable/OnWater.cpp"
    "src/Main executable/path.cpp"
    "src/Main executable/RealWater.cpp"
    "src/Main executable/Recorder.cpp"
    "src/Main executable/Report.cpp"
    "src/Main executable/Resfile.cpp"
    "src/Main executable/River.cpp"
    "src/Main executable/SafeNet.cpp"
    "src/Main executable/SaveNewMap.cpp"
    "src/Main executable/Select.cpp"
    "src/Main executable/SelProp.cpp"
    "src/Main executable/ShipTrace.cpp"
    "src/Main executable/StrategyResearch.cpp"
    "src/Main executable/stRecog.cpp"
    "src/Main executable/Stringshash.cpp"
    "src/Main executable/Timing.cpp"
    "src/Main executable/Tmixer.cpp"
    "src/Main executable/TopoGraf.cpp"
    "src/Main executable/Transport.cpp"
    "src/Main executable/UnRar.cpp"
    "src/Main executable/UnSyncro.cpp"
    "src/Main executable/Upgrade.cpp"
    "src/Main executable/VirtScr.cpp"
    "src/Main executable/walls.cpp"
    "src/Main executable/Waves.cpp"
    "src/Main executable/Weapon.cpp"
    "src/Main executable/ZBuffer.cpp"
)

# Include directories
include_directories(
    src/
    "src/Main executable/"
    src/graphics/
    src/audio/
    src/input/
    src/utils/
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries (cross-platform)
target_link_libraries(${PROJECT_NAME} raylib)

# Add platform-specific libraries only if needed
if(WIN32)
    # Only add Windows-specific libraries if absolutely necessary
    target_link_libraries(${PROJECT_NAME} opengl32 gdi32 ws2_32)
elseif(UNIX AND NOT APPLE)
    # Linux-specific libraries
    target_link_libraries(${PROJECT_NAME} pthread dl)
elseif(APPLE)
    # macOS-specific libraries
    target_link_libraries(${PROJECT_NAME} pthread dl)
endif()

# Compiler definitions (cross-platform)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    _CRT_SECURE_NO_WARNINGS
    PLATFORM_DESKTOP
    SINGLEPLAYER_ONLY
)

# Add platform-specific definitions
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_LINUX)
elseif(APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_MACOS)
endif()
