#include <iostream>
#include <vector>
#include <string>
#include "engine_core/EngineCore.hpp"
#include "resource_io/ResourceIO.hpp"
#include "resource_io/ColorPalette.hpp"
#include "game_logic/GameLogic.hpp"
#include "legacy/SpriteCache.hpp"
#include "legacy/Time_compat.hpp"
#include "legacy/MainMenu_compat.hpp"
#include "legacy/Sound_compat.hpp"
#include "audio_core/AudioCore.hpp"
#include "audio_core/Music.hpp"
#include "world_render/WorldRender.hpp"
#include "world_render/Terrain.hpp"
#include "world_render/Decor.hpp"
#include "legacy/Text_compat.hpp"
#if defined(_WIN32)
#include <windows.h>
#endif

int main() {
    std::cout << "Cossacks Remaster skeleton" << std::endl;

    if (!engine_core::initialize_window(1280, 720, "Cossacks Remaster")) {
        std::cerr << "Failed to initialize engine window" << std::endl;
        return 1;
    }

    resource_io::initialize();
    // Set data root to the directory of the executable (game root)
#if defined(_WIN32)
    {
        char modulePath[260] = {0};
        if (GetModuleFileNameA(nullptr, modulePath, sizeof(modulePath) - 1) > 0) {
            std::string exePath(modulePath);
            size_t pos = exePath.find_last_of("\\/");
            std::string exeDir = (pos != std::string::npos) ? exePath.substr(0, pos) : std::string(".");
            resource_io::set_data_root(exeDir);
        } else {
            resource_io::set_data_root(".");
        }
    }
#else
    resource_io::set_data_root(".");
#endif
    resource_io::open_default_archives();
    resource_io::initialize_subsystems();
    // Delay terrain GPU initialization until a map is actually loaded
    legacy::sound_compat::initialize();
    legacy::time_compat::initialize();
    audio_core::initialize();
    audio_core::set_max_live_sources(64);
    // Music: scan BUILD\Music and start random playback like legacy CD logic
    audio_core::music::initialize();
    audio_core::music::set_track_directory("Music");
    audio_core::music::reload_tracks();
    audio_core::music::set_play_mode(2); // default: random
    audio_core::music::set_volume_percent(50);
    audio_core::music::play_random_next();
    // Try to load a default color palette used by UI (fallback to grayscale if missing)
    (void)resource_io::color_palette::load_from_path("2\\agew_1.pal");
    game_logic::initialize();

    // Setup main menu and drive it each frame
    legacy::ui::setup_main_menu();
    // Hide system cursor so GP-cursor is visible in windowed/fullscreen
    engine_core::set_system_cursor_visible(false);
    static double lastTimeSec = 0.0;
    static int frameCount = 0;
    static float fps = 0.0f;
    engine_core::set_frame_callback([&](int fbw, int fbh){
        world_render::set_framebuffer_size(fbw, fbh);
        world_render::begin_frame();
        // Process menu first; it may load a map and deactivate menu
        if (legacy::ui::is_main_menu_active()) {
            legacy::ui::frame_main_menu();
        }
        // Render world only if terrain has valid arrays (map loaded)
        if (world_render::terrain::is_ready()) {
            world_render::terrain::submit_frame();
            world_render::decor::submit_frame();
        }
        // FPS counter (always render; same style as menu text adapter)
        {
            using legacy::ui::ShowString;
            // Update once per 0.25s for stability
            double now = engine_core::get_time_seconds();
            frameCount++;
            if (now - lastTimeSec >= 0.25) {
                fps = static_cast<float>(frameCount / (now - lastTimeSec));
                frameCount = 0; lastTimeSec = now;
            }
            char buf[64];
            snprintf(buf, sizeof(buf), "FPS: %.1f", fps);
            ShowString(8, 8, buf, 0xFFFFFFFFu);
        }
        world_render::flush();
        audio_core::update();
        audio_core::music::update();
    });

    engine_core::run_main_loop();
    // Stop simulation before tearing down subsystems
    game_logic::shutdown();
    audio_core::music::shutdown();
    audio_core::shutdown();
    engine_core::shutdown();

    std::cout << "Initialization complete." << std::endl;
    return 0;
}
