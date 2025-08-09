#include "legacy/MainMenu_compat.hpp"

#include "legacy/DialogsSystem_compat.hpp"
#include "legacy/GP_Draw_compat.hpp"
#include "legacy/SQPicture_compat.hpp"
#include "legacy/Time_compat.hpp"
#include "legacy/Globals_compat.hpp"
#include "legacy/Input_compat.hpp"
#include "legacy/MouseCursor_compat.hpp"

namespace legacy { namespace ui {

namespace {
bool g_active = false;
MainMenuChoice g_choice = MainMenuChoice::None;

// Menu resources
legacy::SQPictureCompat g_background;
legacy::ui::DialogsSystem* g_menu = nullptr;

bool on_click_choice(legacy::ui::SimpleDialog* sd, MainMenuChoice c) {
    g_choice = c;
    g_active = false;
    return true;
}
}

void setup_main_menu() {
    if (g_active) return;
    // Load background BMP/BPX
    g_background.LoadPicture("Interface\\Background_Main_Menu.bmp");

    // Build UI
    const int baseX = legacy::globals::menu_x_off;
    const int baseY = legacy::globals::menu_y_off;
    delete g_menu;
    g_menu = new legacy::ui::DialogsSystem(baseX, baseY);

    // Set legacy draw window to full framebuffer
    legacy::gp::SetRLCWindow(0, 0,
        legacy::globals::RealLx,
        legacy::globals::RSCRSizeY,
        legacy::globals::SCRSizeX);

    // Background
    auto* pic = g_menu->addPicture(nullptr, 0, 0, &g_background, &g_background, &g_background);
    (void)pic;

    // Load buttons GP now (avoid static init before ResourceIO)
    legacy::gp::LocalGP BTNS("Interface\\Main_Menu");
    // Buttons layout mirrors original (x=76,y starts at 140 with 82 step)
    auto* bSingle  = g_menu->addGP_Button(nullptr, 76, 140, BTNS.GPID, 0, 1);
    auto* bMulti   = g_menu->addGP_Button(nullptr, 76, 140 + 82, BTNS.GPID, 2, 3);
    auto* bLoad    = g_menu->addGP_Button(nullptr, 76, 140 + 82*2, BTNS.GPID, 4, 5);
    auto* bOptions = g_menu->addGP_Button(nullptr, 76, 140 + 82*3, BTNS.GPID, 6, 7);
    auto* bVideo   = g_menu->addGP_Button(nullptr, 76, 140 + 82*4, BTNS.GPID, 8, 9);
    auto* bExit    = g_menu->addGP_Button(nullptr, 76, 140 + 82*5, BTNS.GPID, 10, 11);

    bSingle->OnClick  = [](legacy::ui::SimpleDialog* sd){ return on_click_choice(sd, MainMenuChoice::Single); };
    bMulti->OnClick   = [](legacy::ui::SimpleDialog* sd){ return on_click_choice(sd, MainMenuChoice::Multi); };
    bLoad->OnClick    = [](legacy::ui::SimpleDialog* sd){ return on_click_choice(sd, MainMenuChoice::Load); };
    bOptions->OnClick = [](legacy::ui::SimpleDialog* sd){ return on_click_choice(sd, MainMenuChoice::Options); };
    bVideo->OnClick   = [](legacy::ui::SimpleDialog* sd){ return on_click_choice(sd, MainMenuChoice::Video); };
    bExit->OnClick    = [](legacy::ui::SimpleDialog* sd){ return on_click_choice(sd, MainMenuChoice::Exit); };

    g_active = true;
    g_choice = MainMenuChoice::None;

    // Setup cursor pack used in menus; original uses CurrentCursorGP set via fog.cpp logic
    // Original sets CurrentCursorGP via LoadFog(set): set+5 maps to Cursor_0{set} packs; for menu set=2 → Cursor_02
    // Cursor packs are at archive root (e.g., CURSOR_02.GP)
    legacy::cursor_compat::set_cursor_pack("CURSOR_02");
    // If not found, try lower/upper variants on next frames via log; user can report actual name
    legacy::cursor_compat::set_cursor_frame(0);
}

void frame_main_menu() {
    if (!g_active || !g_menu) return;
    // Process platform messages first as in legacy ProcessMessages()
    (void)legacy::input_compat::ProcessMessages();
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


