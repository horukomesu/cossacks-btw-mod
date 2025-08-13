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
#include "legacy/RLCFont_compat.hpp"
#include "legacy/RLC_compat.hpp"
#include "engine_core/EngineCore.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <cstdio>
#include "audio_core/Music.hpp"
#include "legacy/Sound_compat.hpp"
#include "legacy/FontsRegistry_compat.hpp"
#include "resource_io/ColorPalette.hpp"
#include "resource_io/Map.hpp"
#include "game_logic/GameLogic.hpp"
#include "world_render/Terrain.hpp"
#include "legacy/MapState_compat.hpp"
#include "legacy/Path_compat.hpp"
#include "legacy/Water_compat.hpp"
#include "legacy/ScenePostInit_compat.hpp"
#include "resource_io/Map.hpp"

namespace legacy { namespace ui {

namespace {
bool g_active = false;
MainMenuChoice g_choice = MainMenuChoice::None;
enum class MenuState { Main, Single, Options, Load };
static MenuState g_state = MenuState::Main;
static MenuState g_builtState = MenuState::Main;
static bool g_startRandomRequested = false; // request to start gameplay (random map) on next frame

// Menu resources
legacy::SQPictureCompat g_background;
legacy::ui::DialogsSystem* g_menu = nullptr;

// FPS overlay state
static bool g_showFps = true;
// RLC-based font cache for FPS overlay (legacy-compatible rendering)
static legacy::rlc_compat::RLCTable g_fpsRlcFont = nullptr;
static bool g_fpsRlcFontLoaded = false;
static uint32_t g_lastFpsTime = 0;
static int g_frameCounter = 0;
static int g_lastFpsValue = 0;

// Draw ASCII text using legacy RLC rendering (indices start at baseChar=32)
static int rlc_text_width(const char* s) {
    if (!s) return 0;
    if (!g_fpsRlcFontLoaded) {
        g_fpsRlcFontLoaded = legacy::rlc_compat::LoadRLC("RF32W.RLC", &g_fpsRlcFont);
        // Match original RLCFont behavior: first symbol offset = 0
    }
    if (!g_fpsRlcFontLoaded || !g_fpsRlcFont) return 0;
    int w = 0;
    const unsigned char* p = reinterpret_cast<const unsigned char*>(s);
    while (*p) {
        unsigned char ch = *p++;
        if (ch < 32) ch = 32;
        const int idx = static_cast<int>(ch);
        int adv = legacy::rlc_compat::GetRLCWidth(g_fpsRlcFont, static_cast<legacy::rlc_compat::byte>(idx));
        if (adv <= 0) adv = 16; // fallback for fonts with missing width/space
        w += adv;
    }
    return w;
}

static void rlc_draw_text(int x, int y, const char* s) {
    if (!s) return;
    if (!g_fpsRlcFontLoaded) {
        g_fpsRlcFontLoaded = legacy::rlc_compat::LoadRLC("RF32W.RLC", &g_fpsRlcFont);
    }
    if (!g_fpsRlcFontLoaded || !g_fpsRlcFont) return;
    const unsigned char* p = reinterpret_cast<const unsigned char*>(s);
    int cx = x;
    while (*p) {
        unsigned char ch = *p++;
        if (ch < 32) ch = 32;
        const int idx = static_cast<int>(ch);
        legacy::rlc_compat::ShowRLCItem(cx, y, &g_fpsRlcFont, idx, /*nt*/ 0);
        int adv = legacy::rlc_compat::GetRLCWidth(g_fpsRlcFont, static_cast<legacy::rlc_compat::byte>(idx));
        if (adv <= 0) adv = 16;
        cx += adv;
    }
}

static int rlc_line_height() {
    if (!g_fpsRlcFontLoaded) {
        g_fpsRlcFontLoaded = legacy::rlc_compat::LoadRLC("RF32W.RLC", &g_fpsRlcFont);
    }
    if (!g_fpsRlcFontLoaded || !g_fpsRlcFont) return 32;
    int h = legacy::rlc_compat::GetRLCHeight(g_fpsRlcFont, static_cast<legacy::rlc_compat::byte>('A'));
    if (h <= 0) h = legacy::rlc_compat::GetRLCHeight(g_fpsRlcFont, static_cast<legacy::rlc_compat::byte>('0'));
    if (h <= 0) h = 32;
    return h;
}

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
// Debug: load map and pipe into terrain system
static resource_io::map::MapData g_loadedMap; // persistent storage for map data
static void load_map_for_debug(const char* path) {
    resource_io::map::MapData mdTemp;
    if (resource_io::map::load_m3d(path, mdTemp)) {
        g_loadedMap = std::move(mdTemp);
        std::cout << "[debug] map loaded: " << path << " addShift=" << g_loadedMap.addShift
                  << " tiles=" << g_loadedMap.mapTilesX << "x" << g_loadedMap.mapTilesY << std::endl;
        // Perform post-load initialization (compat passmaps, water locking, renderer binding)
        legacy::scene_post_init::after_map_loaded(g_loadedMap);
    } else {
        std::cout << "[debug] map load FAILED: " << path << std::endl;
    }
}

// Helpers
static void close_window() {
    // Request window close (Exit flow replacement for SlideShow in legacy)
    if (auto* win = engine_core::get_active_window()) {
        glfwSetWindowShouldClose(win, GLFW_TRUE);
    }
}

// Simple settings storage for Options screen
static int s_warSound = 50;
static int s_midiSound = 50;
static int s_scrollSpeed = 5; // 2..10
static int s_gameSpeedMode = 0; // 0 slow, 1 fast
static int s_musicPlayMode = 2; // 0 off, 1 nation, 2 random
static bool s_recordEnabled = false;
static std::string s_recordPath;
static int s_resW = 1024, s_resH = 768;

// Pointers to Options UI elements
static legacy::ui::VScrollBar* g_opt_music = nullptr;
static legacy::ui::VScrollBar* g_opt_sfx = nullptr;
static legacy::ui::VScrollBar* g_opt_scroll = nullptr;
static legacy::ui::ComboBox* g_opt_vmode = nullptr;
static legacy::ui::ComboBox* g_opt_fmode = nullptr;
static legacy::ui::ComboBox* g_opt_musmode = nullptr;
static legacy::ui::GP_Button* g_opt_rec_btn = nullptr;
static legacy::ui::InputBox* g_opt_rec_path = nullptr;
static legacy::ui::GPPicture* g_opt_hint1 = nullptr;
static legacy::ui::GPPicture* g_opt_hint2 = nullptr;
static legacy::ui::GPPicture* g_opt_hint3 = nullptr;

static void load_settings();
static void save_settings();
static bool on_options_ok(legacy::ui::SimpleDialog* /*sd*/) {
    if (g_opt_music) s_midiSound = std::max(0, std::min(100, g_opt_music->SPos));
    if (g_opt_sfx)   s_warSound  = std::max(0, std::min(100, g_opt_sfx->SPos));
    if (g_opt_scroll) s_scrollSpeed = std::clamp(g_opt_scroll->SPos + 2, 2, 10);
    if (g_opt_fmode) s_gameSpeedMode = std::clamp(g_opt_fmode->CurLine, 0, 1);
    if (g_opt_musmode) s_musicPlayMode = std::clamp(g_opt_musmode->CurLine, 0, 2);
    if (g_opt_rec_path) s_recordPath = g_opt_rec_path->Text;
    // Apply music & SFX
    audio_core::music::set_volume_percent(s_midiSound);
    ::legacy::sound_compat::set_sfx_volume_percent(s_warSound);
    audio_core::music::set_play_mode(s_musicPlayMode);
    if (s_musicPlayMode != 0) audio_core::music::play_random_next(); else audio_core::music::stop();
    // Apply game speed to simulation thread (Slow=30Hz, Fast=60Hz)
    game_logic::set_speed_mode(s_gameSpeedMode);
    // Apply resolution
    if (g_opt_vmode && !g_opt_vmode->Lines.empty()) {
        const std::string& sel = g_opt_vmode->Lines[std::max(0, std::min(static_cast<int>(g_opt_vmode->Lines.size()) - 1, g_opt_vmode->CurLine))];
        int w=0,h=0; if (std::sscanf(sel.c_str(), "%dx%d", &w, &h) == 2) {
            s_resW = w; s_resH = h; engine_core::set_window_size(w, h);
        }
    }
    save_settings();
    g_state = MenuState::Main;
    return true;
}
static void load_settings() {
    std::ifstream in("remaster_settings.ini");
    if (!in) return;
    // war sfx | midi music | scroll | gameSpeed | playMode | recEnabled | resW | resH | recPath
    int w = s_warSound, m = s_midiSound, sc = s_scrollSpeed, gs = s_gameSpeedMode, pm = s_musicPlayMode;
    int re = s_recordEnabled ? 1 : 0; int rw = s_resW, rh = s_resH; std::string rp;
    in >> w >> m >> sc >> gs >> pm >> re >> rw >> rh;
    std::getline(in, rp);
    if (!rp.empty() && rp[0] == ' ') rp.erase(0, 1);
    if (w >= 0 && w <= 100) s_warSound = w;
    if (m >= 0 && m <= 100) s_midiSound = m;
    if (sc >= 2 && sc <= 10) s_scrollSpeed = sc;
    if (gs == 0 || gs == 1) s_gameSpeedMode = gs;
    if (pm >= 0 && pm <= 2) s_musicPlayMode = pm;
    s_recordEnabled = (re != 0);
    if (rw >= 320 && rh >= 240) { s_resW = rw; s_resH = rh; }
    s_recordPath = rp;
}
static void save_settings() {
    std::ofstream out("remaster_settings.ini", std::ios::trunc);
    if (!out) return;
    out << s_warSound << ' ' << s_midiSound << ' ' << s_scrollSpeed << ' ' << s_gameSpeedMode
        << ' ' << s_musicPlayMode << ' ' << (s_recordEnabled ? 1 : 0)
        << ' ' << s_resW << ' ' << s_resH << ' ' << s_recordPath;
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
        // Тестовый запуск геймплея: одиночная игра → рандомная карта
        g_startRandomRequested = true;
        return true;
    };
    bBack->OnClick = [](legacy::ui::SimpleDialog* /*sd*/){ g_state = MenuState::Main; return true; };
    g_builtState = MenuState::Single;
}

static void build_options_screen() {
    // Options submenu (копия логики из Interface.cpp::ProcessMenuOptions, сокращённая до доступных подсистем)
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
    // Hint pictures
    g_opt_hint1 = g_menu->addGPPicture(nullptr, 240, 317 + 10 - 13, BTNS.GPID, 13);
    g_opt_hint2 = g_menu->addGPPicture(nullptr, 240, 343 + 10 - 13, BTNS.GPID, 14);
    g_opt_hint3 = g_menu->addGPPicture(nullptr, 240, 447 + 10 - 13, BTNS.GPID, 15);
    if (g_opt_hint1) g_opt_hint1->Visible = false; if (g_opt_hint2) g_opt_hint2->Visible = false; if (g_opt_hint3) g_opt_hint3->Visible = false;
    // Resolution dropdown
    g_opt_vmode = g_menu->addGP_ComboBox(nullptr, 240, 240 - 16 - 13, BTNS.GPID, 0, 9, 0, nullptr, nullptr, nullptr);
    if (g_opt_vmode) {
        const struct { int w,h; } modes[] = { {1024,768}, {1280,720}, {1280,1024}, {1366,768}, {1600,900}, {1920,1080}, {2560,1440} };
        int cur = 0;
        for (size_t i=0;i<sizeof(modes)/sizeof(modes[0]);++i) {
            char cc[32]; std::snprintf(cc, sizeof(cc), "%dx%d", modes[i].w, modes[i].h);
            g_opt_vmode->AddLine(cc);
            if (modes[i].w==s_resW && modes[i].h==s_resH) cur = static_cast<int>(i);
        }
        g_opt_vmode->CurLine = cur; g_opt_vmode->MaxVisible = 6;
    }
    // Music play mode
    g_opt_musmode = g_menu->addGP_ComboBox(nullptr, 240, 240 - 16 + 80 - 3 - 13, BTNS.GPID, 0, 9, 0, nullptr, nullptr, nullptr);
    if (g_opt_musmode) { g_opt_musmode->AddLine("NOCD"); g_opt_musmode->AddLine("NATMUS"); g_opt_musmode->AddLine("RNDMUS"); g_opt_musmode->CurLine = s_musicPlayMode; g_opt_musmode->MaxVisible = 5; }
    // Game speed
    g_opt_fmode = g_menu->addGP_ComboBox(nullptr, 240, 421 + 10 - 13, BTNS.GPID, 0, 9, 0, nullptr, nullptr, nullptr);
    if (g_opt_fmode) { g_opt_fmode->AddLine("Slow"); g_opt_fmode->AddLine("Fast"); g_opt_fmode->CurLine = s_gameSpeedMode; g_opt_fmode->MaxVisible = 5; }
    // Sliders (MVL, SVL, SSP)
    g_opt_music = g_menu->addGP_ScrollBarL(nullptr, 244-2, 320 + 10 - 13, 100, s_midiSound, BTNS.GPID, 12, 484 - 244 + 20, 12, 0, 0);
    if (g_opt_music) {
        g_opt_music->OnMouseOver = [](legacy::ui::SimpleDialog* sd){
            if (g_opt_music) {
                int v = std::max(0, std::min(100, g_opt_music->SPos));
                audio_core::music::set_volume_percent(v);
            }
            if (g_opt_hint1) g_opt_hint1->Visible = sd->MouseOver;
            return false;
        };
    }
    g_opt_sfx   = g_menu->addGP_ScrollBarL(nullptr, 244-2, 346 + 10 - 13, 100, s_warSound,  BTNS.GPID, 12, 484 - 244 + 20, 12, 0, 0);
    if (g_opt_sfx) {
        g_opt_sfx->OnMouseOver = [](legacy::ui::SimpleDialog* sd){
            if (g_opt_sfx) {
                int v = std::max(0, std::min(100, g_opt_sfx->SPos));
                ::legacy::sound_compat::set_sfx_volume_percent(v);
            }
            if (g_opt_hint2) g_opt_hint2->Visible = sd->MouseOver;
            return false;
        };
    }
    // Scroll speed control
    g_opt_scroll = g_menu->addGP_ScrollBarL(nullptr, 244-2, 450 + 10 - 13, 8, std::max(0, s_scrollSpeed - 2), BTNS.GPID, 12, 484 - 244 + 20, 12, 0, 0);
    if (g_opt_scroll) g_opt_scroll->OnMouseOver = [](legacy::ui::SimpleDialog* sd){ if (g_opt_hint3) g_opt_hint3->Visible = sd->MouseOver; return false; };
    // Recording checkbox + path
    g_opt_rec_btn = g_menu->addGP_Button(nullptr, 240, 496, BTNS.GPID, 20, 19);
    if (g_opt_rec_btn) {
        g_opt_rec_btn->OnUserClick = [](legacy::ui::SimpleDialog* sd){
            s_recordEnabled = !s_recordEnabled;
            auto* b = static_cast<legacy::ui::GP_Button*>(sd);
            b->ActiveFrame = s_recordEnabled ? 22 : 20;
            b->PassiveFrame = s_recordEnabled ? 21 : 19;
            if (g_opt_rec_path) g_opt_rec_path->Active = s_recordEnabled;
            return true;
        };
        g_opt_rec_btn->ActiveFrame = s_recordEnabled ? 22 : 20;
        g_opt_rec_btn->PassiveFrame = s_recordEnabled ? 21 : 19;
    }
    g_opt_rec_path = g_menu->addInputBox(nullptr, 275, 496, s_recordPath.c_str(), 120, 240, 26, nullptr, nullptr);
    if (g_opt_rec_path) g_opt_rec_path->Active = s_recordEnabled;
    // OK / Exit buttons
    auto* bOk   = g_menu->addGP_Button(nullptr, 113, 534 + 10 + 13, BTNS.GPID, 8, 9);
    auto* bExit = g_menu->addGP_Button(nullptr, 333, 534 + 10 + 13, BTNS.GPID, 10, 11);
    // Remember previous for cancel
    static int prev_music_vol = s_midiSound; static int prev_play_mode = s_musicPlayMode;
    prev_music_vol = s_midiSound; prev_play_mode = s_musicPlayMode;
    bOk->OnClick   = &on_options_ok;
    bExit->OnClick = [](legacy::ui::SimpleDialog* /*sd*/){
        audio_core::music::set_volume_percent(prev_music_vol);
        audio_core::music::set_play_mode(prev_play_mode);
        if (prev_play_mode != 0) audio_core::music::play_random_next(); else audio_core::music::stop();
        g_state = MenuState::Main; return true; };
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
    // Ensure legacy RLC window covers full framebuffer for clipping-based draws
    legacy::gp::SetRLCWindow(0, 0, legacy::globals::RealLx, legacy::globals::RealLy, legacy::globals::SCRSizeX);
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
    // Тест: старт геймплея из Single Player → Random Map
    if (g_startRandomRequested) {
        // Запускаем тестовую сцену: загрузка карты test.m3d и выход из меню
        // Make sure resources/palettes are available for terrain colors
        (void)resource_io::color_palette::load_from_path("2\\agew_1.pal");
        load_map_for_debug("test.m3d");
        std::cout << "[debug] terrain ready=" << (world_render::terrain::is_ready()?1:0) << std::endl;
        // One-time summary of loaded map and tile usage
        resource_io::map::log_map_summary(g_loadedMap);
        g_active = false;
        g_startRandomRequested = false;
        return;
    }
    if (g_choice == MainMenuChoice::Video) {
        // Legacy enters editor after ShowLoading/PrepareEditMedia; we close menu for now
        g_active = false;
        // Debug: try to load default user map if present
        load_map_for_debug("USERMISSPATH.m3d");
        return;
    }
    g_menu->ProcessDialogs();
    // Our renderer clears every frame, so always redraw full UI
    g_menu->MarkToDraw();
    g_menu->RefreshView();
    // FPS overlay using legacy RLC rendering (top-right)
    if (g_showFps) {
        ++g_frameCounter;
        const uint32_t now = static_cast<uint32_t>(legacy::time_compat::GetTickCount());
        if (!g_fpsRlcFontLoaded) {
            g_lastFpsTime = now;
            g_frameCounter = 0;
            g_lastFpsValue = 0;
        }
        if (now - g_lastFpsTime >= 1000u) {
            g_lastFpsValue = g_frameCounter;
            g_frameCounter = 0;
            g_lastFpsTime = now;
        }
        char buf[32];
        std::snprintf(buf, sizeof(buf), "%d FPS", g_lastFpsValue);
        const int mx = g_menu ? g_menu->BaseX : legacy::globals::menu_x_off;
        const int my = g_menu ? g_menu->BaseY : legacy::globals::menu_y_off;
        const int mw = 1024; // legacy menu logical width
        // Ensure color palette for fonts is loaded from resources/1W.pal (RGB 256*3 or RGBA 256*4)
        (void)resource_io::color_palette::load_from_path("resources/1W.pal");
        const int x = mx + mw - rlc_text_width(buf) - 8;
        rlc_draw_text(x, my + 8, buf);
        // Additional test string just below FPS
        // No test string; only FPS counter displayed
    }
    // Draw cursor last, over UI
    legacy::cursor_compat::draw_cursor();
}
void debug_load_map(const char* path) {
    load_map_for_debug(path);
}

bool is_main_menu_active() { return g_active; }
MainMenuChoice get_main_menu_choice() { return g_choice; }

bool consume_start_random_requested() {
    if (!g_startRandomRequested) return false;
    g_startRandomRequested = false;
    return true;
}

void set_fps_overlay_enabled(bool enabled) { g_showFps = enabled; }

} } // namespace legacy::ui


