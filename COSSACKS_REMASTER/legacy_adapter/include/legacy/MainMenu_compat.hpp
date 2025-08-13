#pragma once

namespace legacy { namespace ui {

enum class MainMenuChoice {
    None = 0,
    Single,
    Multi,
    Load,
    Options,
    Video,
    Exit
};

// Prepare resources and UI for the main menu; safe to call once at startup
void setup_main_menu();

// Per-frame menu update & draw; call from engine frame callback
void frame_main_menu();

// Query menu state/choice
bool is_main_menu_active();
MainMenuChoice get_main_menu_choice();

// Enable/disable FPS overlay in top-right corner (rendered via RLCFont)
void set_fps_overlay_enabled(bool enabled);

// Options state accessors for persistence/binding
void set_sfx_volume_percent(int vol01to100);
int  get_sfx_volume_percent();
void set_music_volume_percent(int vol01to100);
int  get_music_volume_percent();
void set_scroll_speed(int val2to10);
int  get_scroll_speed();
void set_game_speed_mode(int mode0slow1fast);
int  get_game_speed_mode();
void set_music_play_mode(int mode0off1nation2random);
int  get_music_play_mode();
void set_recording_enabled(bool enabled);
bool get_recording_enabled();
void set_recording_path(const char* path);
const char* get_recording_path();
void set_resolution(int width, int height);
void get_resolution(int& outW, int& outH);

// Temporary hook: load a map (original .m3d) for terrain rendering tests.
void debug_load_map(const char* path);

// Test hook for gameplay start from Single Player → Random Map
bool consume_start_random_requested();

} } // namespace legacy::ui


