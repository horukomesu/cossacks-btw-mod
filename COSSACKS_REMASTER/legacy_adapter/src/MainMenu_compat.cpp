#include "legacy/MainMenu_compat.hpp"

#include "legacy/DialogsSystem_compat.hpp"
#include "legacy/GP_Draw_compat.hpp"
#include "legacy/SQPicture_compat.hpp"
#include "legacy/Time_compat.hpp"
#include "legacy/Globals_compat.hpp"
#include "legacy/Input_compat.hpp"
#include "legacy/MouseCursor_compat.hpp"
#include "legacy/Palette_compat.hpp"
#include "legacy/Fog_compat.hpp"
#include "engine_core/EngineCore.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include "audio_core/Music.hpp"

namespace legacy { namespace ui {

namespace {
bool g_active = false;
MainMenuChoice g_choice = MainMenuChoice::None;
enum class MenuState { Main, Single, Options, Load };
static MenuState g_state = MenuState::Main;
static MenuState g_builtState = MenuState::Main;

// Menu resources
legacy::SQPictureCompat g_background;
legacy::ui::DialogsSystem* g_menu = nullptr;

bool on_click_choice(legacy::ui::SimpleDialog* /*sd*/, MainMenuChoice c) {
    g_choice = c;
    switch (c) {
        case MainMenuChoice::Single:  g_state = MenuState::Single;  break;
        case MainMenuChoice::Options: g_state = MenuState::Options; break;
        case MainMenuChoice::Load:    g_state = MenuState::Load;    break;
        case MainMenuChoice::Exit:    g_active = false;             break;
        default: break;
    }
    return true;
}

static void destroy_menu() {
    delete g_menu;
    g_menu = nullptr;
}

static void build_main_screen();
static void build_single_screen();
static void build_options_screen();
static void build_load_screen();

// Helpers
static void close_window() {
    // Request window close (Exit flow replacement for SlideShow in legacy)
    if (auto* win = engine_core::get_active_window()) {
        glfwSetWindowShouldClose(win, GLFW_TRUE);
    }
}

// Simple settings storage for Options screen (compat to legacy WarSound/MidiSound)
static int s_warSound = 50;
static int s_midiSound = 50;
static void load_settings();
static void save_settings();
static legacy::ui::VScrollBar* g_opt_music = nullptr;
static legacy::ui::VScrollBar* g_opt_sfx = nullptr;
static bool on_options_ok(legacy::ui::SimpleDialog* /*sd*/) {
    if (g_opt_music) s_midiSound = std::max(0, std::min(100, g_opt_music->SPos));
    if (g_opt_sfx)   s_warSound  = std::max(0, std::min(100, g_opt_sfx->SPos));
    audio_core::music::set_volume_percent(s_midiSound);
    save_settings();
    g_state = MenuState::Main;
    return true;
}
static void load_settings() {
    std::ifstream in("remaster_settings.ini");
    if (!in) return;
    int w = s_warSound, m = s_midiSound;
    in >> w >> m;
    if (w >= 0 && w <= 100) s_warSound = w;
    if (m >= 0 && m <= 100) s_midiSound = m;
}
static void save_settings() {
    std::ofstream out("remaster_settings.ini", std::ios::trunc);
    if (!out) return;
    out << s_warSound << " " << s_midiSound;
}

static void build_main_screen() {
    const int baseX = legacy::globals::menu_x_off;
    const int baseY = legacy::globals::menu_y_off;
    destroy_menu();
    g_menu = new legacy::ui::DialogsSystem(baseX, baseY);
    g_menu->HintX = 18 + baseX;
    g_menu->HintY = 701 + baseY;
    g_background.LoadPicture("Interface\\Background_Main_Menu.bmp");
    (void)g_menu->addPicture(nullptr, 0, 0, &g_background, &g_background, &g_background);
    legacy::gp::LocalGP BTNS("INTERFACE\\MAIN_MENU");
    auto* bSingle  = g_menu->addGP_Button(nullptr, 76, 140, BTNS.GPID, 0, 1);
    auto* bMulti   = g_menu->addGP_Button(nullptr, 76, 140 + 82, BTNS.GPID, 2, 3);
    auto* bLoad    = g_menu->addGP_Button(nullptr, 76, 140 + 82*2, BTNS.GPID, 4, 5);
    auto* bOptions = g_menu->addGP_Button(nullptr, 76, 140 + 82*3, BTNS.GPID, 6, 7);
    auto* bVideo   = g_menu->addGP_Button(nullptr, 76, 140 + 82*4, BTNS.GPID, 8, 9);
    auto* bExit    = g_menu->addGP_Button(nullptr, 76, 140 + 82*5, BTNS.GPID, 10, 11);
    bSingle->OnClick  = [](legacy::ui::SimpleDialog* /*sd*/){ on_click_choice(nullptr, MainMenuChoice::Single);  return true; };
    bMulti->OnClick   = [](legacy::ui::SimpleDialog* /*sd*/){ on_click_choice(nullptr, MainMenuChoice::Multi);   return true; };
    bLoad->OnClick    = [](legacy::ui::SimpleDialog* /*sd*/){ on_click_choice(nullptr, MainMenuChoice::Load);    return true; };
    bOptions->OnClick = [](legacy::ui::SimpleDialog* /*sd*/){ on_click_choice(nullptr, MainMenuChoice::Options); return true; };
    bVideo->OnClick   = [](legacy::ui::SimpleDialog* /*sd*/){ on_click_choice(nullptr, MainMenuChoice::Video);   return true; };
    bExit->OnClick    = [](legacy::ui::SimpleDialog* /*sd*/){ on_click_choice(nullptr, MainMenuChoice::Exit);    return true; };
    // Main menu should use parchment scroll sounds
    bSingle->AssignSound("SCROLL", MOUSE_SOUND);
    bMulti->AssignSound("SCROLL", MOUSE_SOUND);
    bLoad->AssignSound("SCROLL", MOUSE_SOUND);
    bOptions->AssignSound("SCROLL", MOUSE_SOUND);
    bVideo->AssignSound("SCROLL", MOUSE_SOUND);
    bExit->AssignSound("SCROLL", MOUSE_SOUND);
    // Assign click mapping: generic buttons -> Svitok2, back -> SvitKlik
    bSingle->AssignSound("SCROLL2", CLICK_SOUND);
    bMulti->AssignSound("SCROLL2", CLICK_SOUND);
    bLoad->AssignSound("SCROLL2", CLICK_SOUND);
    bOptions->AssignSound("SCROLL2", CLICK_SOUND);
    bVideo->AssignSound("SCROLL2", CLICK_SOUND);
    bExit->AssignSound("SCROLL2", CLICK_SOUND);
    g_builtState = MenuState::Main;
}

static void build_single_screen() {
    // Single Player submenu: Campaign, Mission, Random Map, Back
    legacy::palette_compat::SlowLoadPalette("2\\agew_1.pal");
    legacy::fog_compat::LoadFog(2);
    const int baseX = legacy::globals::menu_x_off;
    const int baseY = legacy::globals::menu_y_off;
    destroy_menu();
    g_menu = new legacy::ui::DialogsSystem(baseX, baseY);
    g_menu->HintX = 18 + baseX;
    g_menu->HintY = 701 + baseY;
    g_background.LoadPicture("Interface\\Background_Single_Player.bmp");
    (void)g_menu->addPicture(nullptr, 0, 0, &g_background, &g_background, &g_background);
    legacy::gp::LocalGP BTNS("INTERFACE\\SINGLE_PLAYER");
    const int Dy = 27;
    auto* bCampaign = g_menu->addGP_Button(nullptr, 76, 140 + 35 + Dy, BTNS.GPID, 0, 1);
    auto* bMission  = g_menu->addGP_Button(nullptr, 76, 140 + 82 + 35 + Dy, BTNS.GPID, 2, 3);
    auto* bRandom   = g_menu->addGP_Button(nullptr, 76, 140 + 35 + 82*2 + Dy, BTNS.GPID, 4, 5);
    auto* bBack     = g_menu->addGP_Button(nullptr, 76, 140 + 35 + 82*3 + Dy, BTNS.GPID, 6, 7);
    bCampaign->OnClick = [](legacy::ui::SimpleDialog* /*sd*/){
        // Placeholder for campaign entry point wiring; return to main menu for now
        g_choice = MainMenuChoice::Single;
        g_state = MenuState::Main;
        return true;
    };
    // Submenu: hover is scroll; clicks are button-style
    bCampaign->AssignSound("SCROLL", MOUSE_SOUND);
    bMission->AssignSound("SCROLL", MOUSE_SOUND);
    bRandom->AssignSound("SCROLL", MOUSE_SOUND);
    bBack->AssignSound("SCROLL", MOUSE_SOUND);
    bCampaign->AssignSound("SCROLL2", CLICK_SOUND);
    bMission->AssignSound("SCROLL2", CLICK_SOUND);
    bRandom->AssignSound("SCROLL2", CLICK_SOUND);
    bBack->AssignSound("SCROLLCLICK", CLICK_SOUND); // back -> svitklik
    bMission->OnClick = [](legacy::ui::SimpleDialog* /*sd*/){
        g_choice = MainMenuChoice::Single;
        g_state = MenuState::Main;
        return true;
    };
    bRandom->OnClick = [](legacy::ui::SimpleDialog* /*sd*/){
        g_choice = MainMenuChoice::Single;
        g_state = MenuState::Main;
        return true;
    };
    bBack->OnClick = [](legacy::ui::SimpleDialog* /*sd*/){ g_state = MenuState::Main; return true; };
    g_builtState = MenuState::Single;
}

static void build_options_screen() {
    // Options submenu: simple sliders for Music/SFX, speed mode list, OK/Back
    legacy::palette_compat::SlowLoadPalette("2\\agew_1.pal");
    legacy::fog_compat::LoadFog(2);
    load_settings();
    const int baseX = legacy::globals::menu_x_off;
    const int baseY = legacy::globals::menu_y_off;
    destroy_menu();
    g_menu = new legacy::ui::DialogsSystem(baseX, baseY);
    g_menu->HintX = 18 + baseX;
    g_menu->HintY = 701 + baseY;
    g_background.LoadPicture("Interface\\Background_options.bmp");
    (void)g_menu->addPicture(nullptr, 0, 0, &g_background, &g_background, &g_background);
    legacy::gp::LocalGP BTNS("INTERFACE\\OPTIONS");
    // Sliders (visual only for now)
    g_opt_music = g_menu->addGP_ScrollBar(nullptr, 244-2, 320 + 10 - 13, 100, s_midiSound, BTNS.GPID, 12, 12, 0, 0); // Music
    if (g_opt_music) {
        // live-apply while dragging/adjusting
        g_opt_music->OnMouseOver = [](legacy::ui::SimpleDialog* /*sd*/){
            if (g_opt_music) {
                int v = std::max(0, std::min(100, g_opt_music->SPos));
                audio_core::music::set_volume_percent(v);
            }
            return false;
        };
    }
    g_opt_sfx   = g_menu->addGP_ScrollBar(nullptr, 244-2, 346 + 10 - 13, 100, s_warSound,  BTNS.GPID, 12, 12, 0, 0); // SFX
    // OK / Exit buttons
    auto* bOk   = g_menu->addGP_Button(nullptr, 113, 534 + 10 + 13, BTNS.GPID, 8, 9);
    auto* bExit = g_menu->addGP_Button(nullptr, 333, 534 + 10 + 13, BTNS.GPID, 10, 11);
    bOk->OnClick   = &on_options_ok;
    bExit->OnClick = [](legacy::ui::SimpleDialog* /*sd*/){ g_state = MenuState::Main; return true; };
    bOk->AssignSound("SCROLL", MOUSE_SOUND);
    bExit->AssignSound("SCROLL", MOUSE_SOUND);
    bOk->AssignSound("SCROLL2", CLICK_SOUND);
    bExit->AssignSound("SCROLL2", CLICK_SOUND);
    g_builtState = MenuState::Options;
}

static void build_load_screen() {
    // Load Game submenu: list .sav/.rec files with Ok/Cancel
    legacy::palette_compat::SlowLoadPalette("2\\agew_1.pal");
    legacy::fog_compat::LoadFog(2);
    const int baseX = legacy::globals::menu_x_off;
    const int baseY = legacy::globals::menu_y_off;
    destroy_menu();
    g_menu = new legacy::ui::DialogsSystem(baseX, baseY);
    g_menu->HintX = 18 + baseX;
    g_menu->HintY = 701 + baseY;
    g_background.LoadPicture("Interface\\Background_Load_Game.bmp");
    (void)g_menu->addPicture(nullptr, 0, 0, &g_background, &g_background, &g_background);
    legacy::gp::LocalGP BTNS("INTERFACE\\LOAD_GAME");
    // List and scrollbar
    auto* vs = g_menu->addGP_ScrollBar(nullptr, 500, 166, 403, 0, BTNS.GPID, 7, 7, 0, 14);
    auto* lb = g_menu->addListBox(nullptr, 92, 172, 380, 380, 15, nullptr, nullptr, vs);
    // Populate from current directory and Autorecord
    auto add_files = [&](const std::filesystem::path& dir){
        std::error_code ec;
        if (!std::filesystem::exists(dir, ec)) return;
        for (const auto& de : std::filesystem::directory_iterator(dir, ec)) {
            if (!de.is_regular_file()) continue;
            const auto ext = de.path().extension().string();
            if (ext == ".sav" || ext == ".rec" || ext == ".SAV" || ext == ".REC") {
                const auto name = de.path().filename().string();
                lb->AddItem(name.c_str());
            }
        }
    };
    add_files(std::filesystem::path("."));
    add_files(std::filesystem::path("Autorecord"));
    // Ok / Cancel
    auto* bOk  = g_menu->addGP_Button(nullptr, 113, 594, BTNS.GPID, 0, 1);
    auto* bCan = g_menu->addGP_Button(nullptr, 333, 594, BTNS.GPID, 2, 3);
    bOk->OnClick  = [](legacy::ui::SimpleDialog* /*sd*/){ g_choice = MainMenuChoice::Load; g_state = MenuState::Main; return true; };
    bCan->OnClick = [](legacy::ui::SimpleDialog* /*sd*/){ g_state = MenuState::Main; return true; };
    bOk->AssignSound("SCROLL", MOUSE_SOUND);
    bCan->AssignSound("SCROLL", MOUSE_SOUND);
    bOk->AssignSound("SCROLL2", CLICK_SOUND);
    bCan->AssignSound("SCROLLCLICK", CLICK_SOUND);
    g_builtState = MenuState::Load;
}
}

void setup_main_menu() {
    if (g_active) return;
    // Match legacy: darken then fade in palette and load fog tables for menu (set 2)
    legacy::palette_compat::SetDarkPalette();
    legacy::palette_compat::SlowLoadPalette("2\\agew_1.pal");
    legacy::fog_compat::LoadFog(2);
    std::cout << "[menu] setup_main_menu: palette and fog loaded, building main screen" << std::endl;
    g_active = true;
    g_choice = MainMenuChoice::None;
    g_state = MenuState::Main;
    legacy::cursor_compat::set_cursor_pack("Cursor_00");
    legacy::cursor_compat::set_cursor_frame(0);
    // Build main screen layout like the original
    build_main_screen();
}

void frame_main_menu() {
    if (!g_active || !g_menu) return;
    // Process platform messages first as in legacy ProcessMessages()
    (void)legacy::input_compat::ProcessMessages();
    // Rebuild screen if state changed (like processMainMenu switching pages)
    if (g_builtState != g_state) {
        std::cout << "[menu] state change: rebuilding from " << static_cast<int>(g_builtState)
                  << " to " << static_cast<int>(g_state) << std::endl;
        switch (g_state) {
            case MenuState::Main:    build_main_screen();    break;
            case MenuState::Single:  build_single_screen();  break;
            case MenuState::Options: build_options_screen(); break;
            case MenuState::Load:    build_load_screen();    break;
        }
    }
    // Handle immediate actions
    if (g_choice == MainMenuChoice::Exit) {
        // Legacy calls SlideShow(); here we just close the window
        close_window();
        g_active = false;
        g_choice = MainMenuChoice::None;
        return;
    }
    if (g_choice == MainMenuChoice::Video) {
        // Legacy enters editor after ShowLoading/PrepareEditMedia; we close menu for now
        g_active = false;
        return;
    }
    g_menu->ProcessDialogs();
    // Our renderer clears every frame, so always redraw full UI
    g_menu->MarkToDraw();
    g_menu->RefreshView();
    // Draw cursor last, over UI
    legacy::cursor_compat::draw_cursor();
}

bool is_main_menu_active() { return g_active; }
MainMenuChoice get_main_menu_choice() { return g_choice; }

} } // namespace legacy::ui


