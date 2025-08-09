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
    legacy::time_compat::initialize();
    // Try to load a default color palette used by UI (fallback to grayscale if missing)
    (void)resource_io::color_palette::load_from_path("2\\agew_1.pal");
    game_logic::initialize();

    // Setup main menu and drive it each frame
    legacy::ui::setup_main_menu();
    // Hide system cursor so GP-cursor is visible in windowed/fullscreen
    engine_core::set_system_cursor_visible(false);
    engine_core::set_frame_callback([](int /*fbw*/, int /*fbh*/){
        if (legacy::ui::is_main_menu_active()) {
            legacy::ui::frame_main_menu();
        }
    });

    engine_core::run_main_loop();
    engine_core::shutdown();

    std::cout << "Initialization complete." << std::endl;
    return 0;
}
