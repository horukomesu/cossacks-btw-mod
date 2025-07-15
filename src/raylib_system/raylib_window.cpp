///////////////////////////////////////////////////////////
// RAYLIB_WINDOW.CPP -- Pure Raylib Window System Implementation
// 
// This replaces all Windows API window management with raylib.
// No Windows compatibility layer - pure raylib implementation.
///////////////////////////////////////////////////////////

#include "raylib_window.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

// =============================================================================
// GLOBAL STATE
// =============================================================================

// Window state
int g_window_width = 1024;
int g_window_height = 768;
bool g_window_fullscreen = false;
bool g_window_active = true;
bool g_window_should_close = false;

// Game-specific globals (replacing Windows globals)
int RealLx = 1024;
int RealLy = 768;
int mouseX = 0;
int mouseY = 0;
bool Lpressed = false;
bool Rpressed = false;
bool realLpressed = false;
bool realRpressed = false;
bool KeyPressed = false;
int LastKey = 0;
int LastAsciiKey = 0;
bool bActive = true;
short WheelDelta = 0;

// Input state
RaylibInputState g_input_state = {0};

// Time tracking
static double g_start_time = 0.0;

// =============================================================================
// CORE WINDOW FUNCTIONS
// =============================================================================

bool InitRaylibWindow(const RaylibWindowConfig* config) {
    // Store window configuration
    g_window_width = config->width;
    g_window_height = config->height;
    g_window_fullscreen = config->fullscreen;
    
    // Set raylib configuration flags
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    if (config->vsync) {
        SetConfigFlags(FLAG_VSYNC_HINT);
    }
    
    // Initialize raylib window
    InitWindow(config->width, config->height, config->title);
    
    if (!IsWindowReady()) {
        return false;
    }
    
    // Set target FPS
    SetTargetFPS(config->target_fps);
    
    // Initialize audio system
    InitAudioDevice();
    
    // Set fullscreen if requested
    if (config->fullscreen) {
        ToggleFullscreen();
    }
    
    // Update global variables
    RealLx = config->width;
    RealLy = config->height;
    
    // Initialize timing
    g_start_time = GetTime();
    
    // Initialize input state
    memset(&g_input_state, 0, sizeof(RaylibInputState));
    
    return true;
}

void ShutdownRaylibWindow(void) {
    // Shutdown audio system
    CloseAudioDevice();
    
    // Close window
    CloseWindow();
}

void BeginRaylibFrame(void) {
    // Update input state
    UpdateRaylibInput();
    
    // Begin drawing
    BeginDrawing();
}

void EndRaylibFrame(void) {
    // End drawing
    EndDrawing();
}

bool ShouldWindowClose(void) {
    return WindowShouldClose() || g_window_should_close;
}

void ToggleWindowFullscreen(void) {
    ToggleFullscreen();
    g_window_fullscreen = !g_window_fullscreen;
    
    if (g_window_fullscreen) {
        int monitor = GetCurrentMonitor();
        g_window_width = GetMonitorWidth(monitor);
        g_window_height = GetMonitorHeight(monitor);
    } else {
        g_window_width = 1024;
        g_window_height = 768;
    }
    
    RealLx = g_window_width;
    RealLy = g_window_height;
}

void ResizeWindow(int width, int height) {
    SetWindowSize(width, height);
    g_window_width = width;
    g_window_height = height;
    RealLx = width;
    RealLy = height;
}

void GetWindowSize(int* width, int* height) {
    *width = GetScreenWidth();
    *height = GetScreenHeight();
}

void SetWindowTitle(const char* title) {
    SetWindowTitle(title);
}

// =============================================================================
// INPUT SYSTEM
// =============================================================================

void UpdateRaylibInput(void) {
    // Update mouse position
    Vector2 mouse_pos = GetMousePosition();
    g_input_state.mouse_x = (int)mouse_pos.x;
    g_input_state.mouse_y = (int)mouse_pos.y;
    
    // Update global mouse variables
    mouseX = g_input_state.mouse_x;
    mouseY = g_input_state.mouse_y;
    
    // Update mouse delta
    Vector2 mouse_delta = GetMouseDelta();
    g_input_state.mouse_delta_x = (int)mouse_delta.x;
    g_input_state.mouse_delta_y = (int)mouse_delta.y;
    
    // Update mouse wheel
    float wheel_move = GetMouseWheelMove();
    g_input_state.mouse_wheel = wheel_move;
    WheelDelta = (short)(wheel_move * 120); // Windows uses 120 units per notch
    
    // Update mouse buttons
    g_input_state.left_button_pressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    g_input_state.right_button_pressed = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);
    g_input_state.left_button_down = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    g_input_state.right_button_down = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
    g_input_state.left_button_released = IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
    g_input_state.right_button_released = IsMouseButtonReleased(MOUSE_BUTTON_RIGHT);
    
    // Update global mouse button variables
    if (g_input_state.left_button_pressed) {
        Lpressed = true;
        realLpressed = true;
    }
    if (g_input_state.left_button_released) {
        Lpressed = false;
        realLpressed = false;
    }
    if (g_input_state.right_button_pressed) {
        Rpressed = true;
        realRpressed = true;
    }
    if (g_input_state.right_button_released) {
        Rpressed = false;
        realRpressed = false;
    }
    
    // Update keyboard state
    KeyPressed = false;
    LastKey = 0;
    LastAsciiKey = 0;
    
    // Check all keys
    for (int key = 0; key < 512; key++) {
        g_input_state.keys_pressed[key] = IsKeyPressed(key);
        g_input_state.keys_down[key] = IsKeyDown(key);
        g_input_state.keys_released[key] = IsKeyReleased(key);
        
        if (g_input_state.keys_pressed[key]) {
            KeyPressed = true;
            LastKey = key;
            g_input_state.last_key_pressed = key;
        }
    }
    
    // Handle text input
    int key = GetCharPressed();
    if (key > 0) {
        g_input_state.last_char_pressed = (char)key;
        LastAsciiKey = key;
        
        // Add to text input buffer
        if (g_input_state.text_length < 255) {
            g_input_state.text_input[g_input_state.text_length++] = (char)key;
            g_input_state.text_input[g_input_state.text_length] = '\0';
        }
    }
    
    // Handle special keys
    if (IsKeyPressed(KEY_BACKSPACE) && g_input_state.text_length > 0) {
        g_input_state.text_length--;
        g_input_state.text_input[g_input_state.text_length] = '\0';
    }
    
    if (IsKeyPressed(KEY_ENTER)) {
        g_input_state.text_length = 0;
        g_input_state.text_input[0] = '\0';
    }
    
    // Update window focus state
    bActive = IsWindowFocused();
    g_window_active = bActive;
    
    // Check for window close
    if (WindowShouldClose()) {
        g_window_should_close = true;
    }
}

const RaylibInputState* GetInputState(void) {
    return &g_input_state;
}

// =============================================================================
// UTILITY FUNCTIONS
// =============================================================================

unsigned long GetTickCount(void) {
    return (unsigned long)((GetTime() - g_start_time) * 1000.0);
}

unsigned long GetRealTime(void) {
    return GetTickCount();
}

void SaveScreen(void) {
    // Generate filename with timestamp
    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);
    char filename[256];
    strftime(filename, sizeof(filename), "screenshot_%Y%m%d_%H%M%S.png", timeinfo);
    
    // Take screenshot
    TakeScreenshot(filename);
}

void SetCursorPosition(int x, int y) {
    // Raylib doesn't have a direct equivalent, but we can track it
    // This is mainly used for centering the cursor
    // We'll just store the position for now
}

void ShowCursor(bool show) {
    if (show) {
        ShowCursor();
    } else {
        HideCursor();
    }
}

int GetSystemMetrics(int metric) {
    switch (metric) {
        case SM_CXSCREEN:
            return GetMonitorWidth(GetCurrentMonitor());
        case SM_CYSCREEN:
            return GetMonitorHeight(GetCurrentMonitor());
        default:
            return 0;
    }
}

// =============================================================================
// RAYLIB KEY MAPPING
// =============================================================================

// Map raylib keys to Windows virtual key codes for compatibility
int RaylibKeyToVirtualKey(int raylib_key) {
    switch (raylib_key) {
        case KEY_SPACE: return 32;
        case KEY_APOSTROPHE: return 222;
        case KEY_COMMA: return 188;
        case KEY_MINUS: return 189;
        case KEY_PERIOD: return 190;
        case KEY_SLASH: return 191;
        case KEY_ZERO: return 48;
        case KEY_ONE: return 49;
        case KEY_TWO: return 50;
        case KEY_THREE: return 51;
        case KEY_FOUR: return 52;
        case KEY_FIVE: return 53;
        case KEY_SIX: return 54;
        case KEY_SEVEN: return 55;
        case KEY_EIGHT: return 56;
        case KEY_NINE: return 57;
        case KEY_SEMICOLON: return 186;
        case KEY_EQUAL: return 187;
        case KEY_A: return 65;
        case KEY_B: return 66;
        case KEY_C: return 67;
        case KEY_D: return 68;
        case KEY_E: return 69;
        case KEY_F: return 70;
        case KEY_G: return 71;
        case KEY_H: return 72;
        case KEY_I: return 73;
        case KEY_J: return 74;
        case KEY_K: return 75;
        case KEY_L: return 76;
        case KEY_M: return 77;
        case KEY_N: return 78;
        case KEY_O: return 79;
        case KEY_P: return 80;
        case KEY_Q: return 81;
        case KEY_R: return 82;
        case KEY_S: return 83;
        case KEY_T: return 84;
        case KEY_U: return 85;
        case KEY_V: return 86;
        case KEY_W: return 87;
        case KEY_X: return 88;
        case KEY_Y: return 89;
        case KEY_Z: return 90;
        case KEY_LEFT_BRACKET: return 219;
        case KEY_BACKSLASH: return 220;
        case KEY_RIGHT_BRACKET: return 221;
        case KEY_GRAVE: return 192;
        case KEY_ESCAPE: return 27;
        case KEY_ENTER: return 13;
        case KEY_TAB: return 9;
        case KEY_BACKSPACE: return 8;
        case KEY_INSERT: return 45;
        case KEY_DELETE: return 46;
        case KEY_RIGHT: return 39;
        case KEY_LEFT: return 37;
        case KEY_DOWN: return 40;
        case KEY_UP: return 38;
        case KEY_PAGE_UP: return 33;
        case KEY_PAGE_DOWN: return 34;
        case KEY_HOME: return 36;
        case KEY_END: return 35;
        case KEY_CAPS_LOCK: return 20;
        case KEY_SCROLL_LOCK: return 145;
        case KEY_NUM_LOCK: return 144;
        case KEY_PRINT_SCREEN: return 44;
        case KEY_PAUSE: return 19;
        case KEY_F1: return 112;
        case KEY_F2: return 113;
        case KEY_F3: return 114;
        case KEY_F4: return 115;
        case KEY_F5: return 116;
        case KEY_F6: return 117;
        case KEY_F7: return 118;
        case KEY_F8: return 119;
        case KEY_F9: return 120;
        case KEY_F10: return 121;
        case KEY_F11: return 122;
        case KEY_F12: return 123;
        case KEY_LEFT_SHIFT: return 160;
        case KEY_LEFT_CONTROL: return 162;
        case KEY_LEFT_ALT: return 164;
        case KEY_LEFT_SUPER: return 91;
        case KEY_RIGHT_SHIFT: return 161;
        case KEY_RIGHT_CONTROL: return 163;
        case KEY_RIGHT_ALT: return 165;
        case KEY_RIGHT_SUPER: return 92;
        case KEY_KB_MENU: return 93;
        case KEY_KP_0: return 96;
        case KEY_KP_1: return 97;
        case KEY_KP_2: return 98;
        case KEY_KP_3: return 99;
        case KEY_KP_4: return 100;
        case KEY_KP_5: return 101;
        case KEY_KP_6: return 102;
        case KEY_KP_7: return 103;
        case KEY_KP_8: return 104;
        case KEY_KP_9: return 105;
        case KEY_KP_DECIMAL: return 110;
        case KEY_KP_DIVIDE: return 111;
        case KEY_KP_MULTIPLY: return 106;
        case KEY_KP_SUBTRACT: return 109;
        case KEY_KP_ADD: return 107;
        case KEY_KP_ENTER: return 13;
        case KEY_KP_EQUAL: return 187;
        default: return raylib_key;
    }
} 