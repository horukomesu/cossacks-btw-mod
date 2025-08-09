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

} } // namespace legacy::ui


