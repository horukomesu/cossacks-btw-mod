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
#include <iostream>

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
    const int baseX = legacy::globals::menu_x_off;
    const int baseY = legacy::globals::menu_y_off;
    delete g_menu;
    g_menu = new legacy::ui::DialogsSystem(baseX, baseY);
    g_menu->HintX = 18 + baseX;
    g_menu->HintY = 701 + baseY;
    g_background.LoadPicture("INTERFACE\\Background_Main_Menu.bmp");
    (void)g_menu->addPicture(nullptr, 0, 0, &g_background, &g_background, &g_background);
    legacy::gp::LocalGP BTNS("INTERFACE\\MAIN_MENU");
    std::cout << "[menu] LocalGP MAIN_MENU GPID=" << BTNS.GPID << std::endl;
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
    g_builtState = MenuState::Main;
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
            case MenuState::Main:   setup_main_menu(); break;
            case MenuState::Single: destroy_menu(); /* build single later if needed */ break;
            case MenuState::Options: destroy_menu(); /* build options later */ break;
            case MenuState::Load:   destroy_menu(); /* build load later */ break;
        }
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


