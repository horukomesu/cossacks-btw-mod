#pragma once

#include <raylib.h>
#include <string>
#include <vector>
#include <functional>

// Input event types
enum RaylibInputEventType
{
    RAYLIB_INPUT_EVENT_KEY_DOWN,
    RAYLIB_INPUT_EVENT_KEY_UP,
    RAYLIB_INPUT_EVENT_MOUSE_DOWN,
    RAYLIB_INPUT_EVENT_MOUSE_UP,
    RAYLIB_INPUT_EVENT_MOUSE_MOVE,
    RAYLIB_INPUT_EVENT_MOUSE_WHEEL,
    RAYLIB_INPUT_EVENT_GAMEPAD_BUTTON_DOWN,
    RAYLIB_INPUT_EVENT_GAMEPAD_BUTTON_UP,
    RAYLIB_INPUT_EVENT_GAMEPAD_AXIS
};

// Input event structure
struct RaylibInputEvent
{
    RaylibInputEventType type;
    int key_code;
    int mouse_button;
    Vector2 mouse_position;
    float mouse_wheel;
    int gamepad_id;
    int gamepad_button;
    int gamepad_axis;
    float gamepad_axis_value;
    
    RaylibInputEvent() : type(RAYLIB_INPUT_EVENT_KEY_DOWN), key_code(0), mouse_button(0), 
                         mouse_position({0, 0}), mouse_wheel(0.0f), gamepad_id(0), 
                         gamepad_button(0), gamepad_axis(0), gamepad_axis_value(0.0f) {}
};

// Input callback function type
using RaylibInputCallback = std::function<void(const RaylibInputEvent&)>;

// Raylib Input System
class RaylibInput
{
private:
    // Input state
    bool input_initialized;
    
    // Mouse state
    Vector2 mouse_position;
    Vector2 mouse_delta;
    float mouse_wheel_delta;
    bool mouse_buttons[8];
    bool prev_mouse_buttons[8];
    
    // Keyboard state
    bool keys[512];
    bool prev_keys[512];
    
    // Gamepad state
    bool gamepad_available[4];
    bool gamepad_buttons[4][16];
    bool prev_gamepad_buttons[4][16];
    float gamepad_axes[4][8];
    
    // Input callbacks
    std::vector<RaylibInputCallback> input_callbacks;
    
    // Text input
    std::string text_input;
    bool text_input_active;
    
public:
    RaylibInput();
    ~RaylibInput();
    
    // Initialization
    bool Initialize();
    void Shutdown();
    void Update(); // Call every frame
    
    // Keyboard input
    bool IsKeyDown(int key);
    bool IsKeyPressed(int key);
    bool IsKeyReleased(int key);
    bool IsKeyUp(int key);
    
    // Mouse input
    bool IsMouseButtonDown(int button);
    bool IsMouseButtonPressed(int button);
    bool IsMouseButtonReleased(int button);
    bool IsMouseButtonUp(int button);
    Vector2 GetMousePosition();
    Vector2 GetMouseDelta();
    float GetMouseWheelMove();
    void SetMousePosition(int x, int y);
    void SetMouseCursor(int cursor_type);
    void ShowCursor();
    void HideCursor();
    bool IsCursorHidden();
    
    // Gamepad input
    bool IsGamepadAvailable(int gamepad_id);
    bool IsGamepadButtonDown(int gamepad_id, int button);
    bool IsGamepadButtonPressed(int gamepad_id, int button);
    bool IsGamepadButtonReleased(int gamepad_id, int button);
    bool IsGamepadButtonUp(int gamepad_id, int button);
    float GetGamepadAxisMovement(int gamepad_id, int axis);
    int GetGamepadButtonPressed();
    std::string GetGamepadName(int gamepad_id);
    
    // Text input
    void StartTextInput();
    void StopTextInput();
    bool IsTextInputActive();
    std::string GetTextInput();
    void ClearTextInput();
    int GetCharPressed();
    
    // Input callbacks
    void RegisterInputCallback(RaylibInputCallback callback);
    void UnregisterInputCallback(RaylibInputCallback callback);
    void ClearInputCallbacks();
    
    // Utility functions
    Vector2 GetWorldMousePosition(Camera2D camera);
    bool IsMouseInRectangle(Rectangle rect);
    
    // Key mapping (for compatibility with original game)
    int MapVirtualKey(int virtual_key);
    int MapScanCode(int scan_code);
    
    // Input recording/playback (for testing)
    void StartInputRecording();
    void StopInputRecording();
    void StartInputPlayback();
    void StopInputPlayback();
    bool IsInputRecording();
    bool IsInputPlaying();
    
    // Error handling
    std::string GetLastError() const;
    
private:
    std::string last_error;
    
    // Helper functions
    void LogError(const std::string& error);
    void UpdateKeyboard();
    void UpdateMouse();
    void UpdateGamepads();
    void ProcessInputEvents();
    void TriggerInputCallback(const RaylibInputEvent& event);
    
    // Input recording
    bool recording_input;
    bool playing_input;
    std::vector<RaylibInputEvent> recorded_events;
    size_t playback_index;
};

// Global input instance
extern RaylibInput* g_input;

// C-style interface for compatibility with original code
extern "C" {
    // Initialization
    void InitRaylibInput();
    void CleanupRaylibInput();
    void UpdateRaylibInput();
    
    // Keyboard
    bool IsRaylibKeyDown(int key);
    bool IsRaylibKeyPressed(int key);
    bool IsRaylibKeyReleased(int key);
    bool IsRaylibKeyUp(int key);
    
    // Mouse
    bool IsRaylibMouseButtonDown(int button);
    bool IsRaylibMouseButtonPressed(int button);
    bool IsRaylibMouseButtonReleased(int button);
    bool IsRaylibMouseButtonUp(int button);
    void GetRaylibMousePosition(int* x, int* y);
    void GetRaylibMouseDelta(int* dx, int* dy);
    float GetRaylibMouseWheelMove();
    void SetRaylibMousePosition(int x, int y);
    void SetRaylibMouseCursor(int cursor_type);
    void ShowRaylibCursor();
    void HideRaylibCursor();
    bool IsRaylibCursorHidden();
    
    // Gamepad
    bool IsRaylibGamepadAvailable(int gamepad_id);
    bool IsRaylibGamepadButtonDown(int gamepad_id, int button);
    bool IsRaylibGamepadButtonPressed(int gamepad_id, int button);
    bool IsRaylibGamepadButtonReleased(int gamepad_id, int button);
    bool IsRaylibGamepadButtonUp(int gamepad_id, int button);
    float GetRaylibGamepadAxisMovement(int gamepad_id, int axis);
    int GetRaylibGamepadButtonPressed();
    const char* GetRaylibGamepadName(int gamepad_id);
    
    // Text input
    void StartRaylibTextInput();
    void StopRaylibTextInput();
    bool IsRaylibTextInputActive();
    const char* GetRaylibTextInput();
    void ClearRaylibTextInput();
    int GetRaylibCharPressed();
    
    // Utility
    void GetRaylibWorldMousePosition(float camera_x, float camera_y, float camera_zoom, int* world_x, int* world_y);
    bool IsRaylibMouseInRectangle(int x, int y, int width, int height);
}

// Key code mappings for compatibility
#define RAYLIB_KEY_SPACE          32
#define RAYLIB_KEY_APOSTROPHE     39
#define RAYLIB_KEY_COMMA          44
#define RAYLIB_KEY_MINUS          45
#define RAYLIB_KEY_PERIOD         46
#define RAYLIB_KEY_SLASH          47
#define RAYLIB_KEY_ZERO           48
#define RAYLIB_KEY_ONE            49
#define RAYLIB_KEY_TWO            50
#define RAYLIB_KEY_THREE          51
#define RAYLIB_KEY_FOUR           52
#define RAYLIB_KEY_FIVE           53
#define RAYLIB_KEY_SIX            54
#define RAYLIB_KEY_SEVEN          55
#define RAYLIB_KEY_EIGHT          56
#define RAYLIB_KEY_NINE           57
#define RAYLIB_KEY_SEMICOLON      59
#define RAYLIB_KEY_EQUAL          61
#define RAYLIB_KEY_A              65
#define RAYLIB_KEY_B              66
#define RAYLIB_KEY_C              67
#define RAYLIB_KEY_D              68
#define RAYLIB_KEY_E              69
#define RAYLIB_KEY_F              70
#define RAYLIB_KEY_G              71
#define RAYLIB_KEY_H              72
#define RAYLIB_KEY_I              73
#define RAYLIB_KEY_J              74
#define RAYLIB_KEY_K              75
#define RAYLIB_KEY_L              76
#define RAYLIB_KEY_M              77
#define RAYLIB_KEY_N              78
#define RAYLIB_KEY_O              79
#define RAYLIB_KEY_P              80
#define RAYLIB_KEY_Q              81
#define RAYLIB_KEY_R              82
#define RAYLIB_KEY_S              83
#define RAYLIB_KEY_T              84
#define RAYLIB_KEY_U              85
#define RAYLIB_KEY_V              86
#define RAYLIB_KEY_W              87
#define RAYLIB_KEY_X              88
#define RAYLIB_KEY_Y              89
#define RAYLIB_KEY_Z              90
#define RAYLIB_KEY_LEFT_BRACKET   91
#define RAYLIB_KEY_BACKSLASH      92
#define RAYLIB_KEY_RIGHT_BRACKET  93
#define RAYLIB_KEY_GRAVE          96
#define RAYLIB_KEY_ESCAPE         256
#define RAYLIB_KEY_ENTER          257
#define RAYLIB_KEY_TAB            258
#define RAYLIB_KEY_BACKSPACE      259
#define RAYLIB_KEY_INSERT         260
#define RAYLIB_KEY_DELETE         261
#define RAYLIB_KEY_RIGHT          262
#define RAYLIB_KEY_LEFT           263
#define RAYLIB_KEY_DOWN           264
#define RAYLIB_KEY_UP             265
#define RAYLIB_KEY_PAGE_UP        266
#define RAYLIB_KEY_PAGE_DOWN      267
#define RAYLIB_KEY_HOME           268
#define RAYLIB_KEY_END            269
#define RAYLIB_KEY_CAPS_LOCK      280
#define RAYLIB_KEY_SCROLL_LOCK    281
#define RAYLIB_KEY_NUM_LOCK       282
#define RAYLIB_KEY_PRINT_SCREEN   283
#define RAYLIB_KEY_PAUSE          284
#define RAYLIB_KEY_F1             290
#define RAYLIB_KEY_F2             291
#define RAYLIB_KEY_F3             292
#define RAYLIB_KEY_F4             293
#define RAYLIB_KEY_F5             294
#define RAYLIB_KEY_F6             295
#define RAYLIB_KEY_F7             296
#define RAYLIB_KEY_F8             297
#define RAYLIB_KEY_F9             298
#define RAYLIB_KEY_F10            299
#define RAYLIB_KEY_F11            300
#define RAYLIB_KEY_F12            301
#define RAYLIB_KEY_LEFT_SHIFT     340
#define RAYLIB_KEY_LEFT_CONTROL   341
#define RAYLIB_KEY_LEFT_ALT       342
#define RAYLIB_KEY_LEFT_SUPER     343
#define RAYLIB_KEY_RIGHT_SHIFT    344
#define RAYLIB_KEY_RIGHT_CONTROL  345
#define RAYLIB_KEY_RIGHT_ALT      346
#define RAYLIB_KEY_RIGHT_SUPER    347

// Mouse button mappings
#define RAYLIB_MOUSE_BUTTON_LEFT    0
#define RAYLIB_MOUSE_BUTTON_RIGHT   1
#define RAYLIB_MOUSE_BUTTON_MIDDLE  2
#define RAYLIB_MOUSE_BUTTON_SIDE    3
#define RAYLIB_MOUSE_BUTTON_EXTRA   4
#define RAYLIB_MOUSE_BUTTON_FORWARD 5
#define RAYLIB_MOUSE_BUTTON_BACK    6 