///////////////////////////////////////////////////////////
// RAYLIB_WINDOW.H -- Pure Raylib Window System
// 
// This replaces all Windows API window management with raylib.
// No Windows compatibility layer - pure raylib implementation.
///////////////////////////////////////////////////////////

#ifndef __RAYLIB_WINDOW_H
#define __RAYLIB_WINDOW_H

#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// WINDOW SYSTEM
// =============================================================================

// Window configuration
typedef struct {
    int width;
    int height;
    const char* title;
    bool fullscreen;
    bool vsync;
    bool resizable;
    int target_fps;
} RaylibWindowConfig;

// Global window state
extern int g_window_width;
extern int g_window_height;
extern bool g_window_fullscreen;
extern bool g_window_active;
extern bool g_window_should_close;

// =============================================================================
// CORE WINDOW FUNCTIONS
// =============================================================================

// Initialize raylib window system
bool InitRaylibWindow(const RaylibWindowConfig* config);

// Shutdown raylib window system
void ShutdownRaylibWindow(void);

// Begin frame rendering
void BeginRaylibFrame(void);

// End frame rendering
void EndRaylibFrame(void);

// Check if window should close
bool ShouldWindowClose(void);

// Toggle fullscreen
void ToggleWindowFullscreen(void);

// Resize window
void ResizeWindow(int width, int height);

// Get window dimensions
void GetWindowSize(int* width, int* height);

// Set window title
void SetWindowTitle(const char* title);

// =============================================================================
// INPUT SYSTEM
// =============================================================================

// Input state
typedef struct {
    int mouse_x;
    int mouse_y;
    int mouse_delta_x;
    int mouse_delta_y;
    float mouse_wheel;
    bool left_button_pressed;
    bool right_button_pressed;
    bool left_button_down;
    bool right_button_down;
    bool left_button_released;
    bool right_button_released;
    
    // Keyboard state
    bool keys_pressed[512];
    bool keys_down[512];
    bool keys_released[512];
    int last_key_pressed;
    char last_char_pressed;
    
    // Text input
    char text_input[256];
    int text_length;
} RaylibInputState;

extern RaylibInputState g_input_state;

// Update input state (call every frame)
void UpdateRaylibInput(void);

// Get current input state
const RaylibInputState* GetRaylibInputState();

// =============================================================================
// GAME-SPECIFIC GLOBALS (replacing Windows globals)
// =============================================================================

// Screen dimensions (replaces RealLx, RealLy)
extern int RealLx;
extern int RealLy;

// Mouse position (replaces global mouseX, mouseY)
extern int mouseX;
extern int mouseY;

// Mouse button state (replaces global Lpressed, Rpressed)
extern bool Lpressed;
extern bool Rpressed;
extern bool realLpressed;
extern bool realRpressed;

// Keyboard state (replaces global KeyPressed, LastKey)
extern bool KeyPressed;
extern int LastKey;
extern int LastAsciiKey;

// Window focus state (replaces global bActive)
extern bool bActive;

// Mouse wheel (replaces global WheelDelta)
extern short WheelDelta;

// =============================================================================
// UTILITY FUNCTIONS
// =============================================================================

// Get time in milliseconds (replaces GetTickCount)
unsigned long GetRaylibTickCount(void);

// Get real time (replaces GetRealTime)
unsigned long GetRealTime(void);

// Save screenshot
void SaveScreen(void);

// Set cursor position
void SetCursorPosition(int x, int y);

// Show/hide cursor
int ShowRaylibCursor(int show);

// Get system metrics equivalent
int GetSystemMetrics(int metric);

// System metrics constants
#define SM_CXSCREEN 0
#define SM_CYSCREEN 1

#ifdef __cplusplus
}
#endif

#endif // __RAYLIB_WINDOW_H 