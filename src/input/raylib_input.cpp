#include "raylib_input.h"
#include <iostream>
#include <algorithm>
#include <cstring>

// Global input instance
RaylibInput* g_input = nullptr;

RaylibInput::RaylibInput()
    : input_initialized(false)
    , mouse_position({0, 0})
    , mouse_delta({0, 0})
    , mouse_wheel_delta(0.0f)
    , text_input_active(false)
    , recording_input(false)
    , playing_input(false)
    , playback_index(0)
{
    // Initialize arrays
    memset(mouse_buttons, false, sizeof(mouse_buttons));
    memset(prev_mouse_buttons, false, sizeof(prev_mouse_buttons));
    memset(keys, false, sizeof(keys));
    memset(prev_keys, false, sizeof(prev_keys));
    memset(gamepad_available, false, sizeof(gamepad_available));
    memset(gamepad_buttons, false, sizeof(gamepad_buttons));
    memset(prev_gamepad_buttons, false, sizeof(prev_gamepad_buttons));
    memset(gamepad_axes, 0, sizeof(gamepad_axes));
}

RaylibInput::~RaylibInput()
{
    Shutdown();
}

bool RaylibInput::Initialize()
{
    try
    {
        input_initialized = true;
        last_error = "";
        return true;
    }
    catch (const std::exception& e)
    {
        LogError("Exception during input initialization: " + std::string(e.what()));
        return false;
    }
}

void RaylibInput::Shutdown()
{
    if (!input_initialized)
        return;
    
    input_callbacks.clear();
    recorded_events.clear();
    text_input.clear();
    
    input_initialized = false;
}

void RaylibInput::Update()
{
    if (!input_initialized)
        return;
    
    UpdateKeyboard();
    UpdateMouse();
    UpdateGamepads();
    ProcessInputEvents();
}

bool RaylibInput::IsKeyDown(int key)
{
    return ::IsKeyDown(key);
}

bool RaylibInput::IsKeyPressed(int key)
{
    return ::IsKeyPressed(key);
}

bool RaylibInput::IsKeyReleased(int key)
{
    return ::IsKeyReleased(key);
}

bool RaylibInput::IsKeyUp(int key)
{
    return ::IsKeyUp(key);
}

bool RaylibInput::IsMouseButtonDown(int button)
{
    return ::IsMouseButtonDown(button);
}

bool RaylibInput::IsMouseButtonPressed(int button)
{
    return ::IsMouseButtonPressed(button);
}

bool RaylibInput::IsMouseButtonReleased(int button)
{
    return ::IsMouseButtonReleased(button);
}

bool RaylibInput::IsMouseButtonUp(int button)
{
    return ::IsMouseButtonUp(button);
}

Vector2 RaylibInput::GetMousePosition()
{
    return ::GetMousePosition();
}

Vector2 RaylibInput::GetMouseDelta()
{
    return ::GetMouseDelta();
}

float RaylibInput::GetMouseWheelMove()
{
    return ::GetMouseWheelMove();
}

void RaylibInput::SetMousePosition(int x, int y)
{
    ::SetMousePosition(x, y);
}

void RaylibInput::SetMouseCursor(int cursor_type)
{
    ::SetMouseCursor(cursor_type);
}

void RaylibInput::ShowCursor()
{
    ::ShowCursor();
}

void RaylibInput::HideCursor()
{
    ::HideCursor();
}

bool RaylibInput::IsCursorHidden()
{
    return ::IsCursorHidden();
}

bool RaylibInput::IsGamepadAvailable(int gamepad_id)
{
    return ::IsGamepadAvailable(gamepad_id);
}

bool RaylibInput::IsGamepadButtonDown(int gamepad_id, int button)
{
    return ::IsGamepadButtonDown(gamepad_id, button);
}

bool RaylibInput::IsGamepadButtonPressed(int gamepad_id, int button)
{
    return ::IsGamepadButtonPressed(gamepad_id, button);
}

bool RaylibInput::IsGamepadButtonReleased(int gamepad_id, int button)
{
    return ::IsGamepadButtonReleased(gamepad_id, button);
}

bool RaylibInput::IsGamepadButtonUp(int gamepad_id, int button)
{
    return ::IsGamepadButtonUp(gamepad_id, button);
}

float RaylibInput::GetGamepadAxisMovement(int gamepad_id, int axis)
{
    return ::GetGamepadAxisMovement(gamepad_id, axis);
}

int RaylibInput::GetGamepadButtonPressed()
{
    return ::GetGamepadButtonPressed();
}

std::string RaylibInput::GetGamepadName(int gamepad_id)
{
    const char* name = ::GetGamepadName(gamepad_id);
    return name ? std::string(name) : "";
}

void RaylibInput::StartTextInput()
{
    text_input_active = true;
    text_input.clear();
}

void RaylibInput::StopTextInput()
{
    text_input_active = false;
    text_input.clear();
}

bool RaylibInput::IsTextInputActive()
{
    return text_input_active;
}

std::string RaylibInput::GetTextInput()
{
    return text_input;
}

void RaylibInput::ClearTextInput()
{
    text_input.clear();
}

int RaylibInput::GetCharPressed()
{
    return ::GetCharPressed();
}

void RaylibInput::RegisterInputCallback(RaylibInputCallback callback)
{
    input_callbacks.push_back(callback);
}

void RaylibInput::UnregisterInputCallback(RaylibInputCallback callback)
{
    // Note: This is a simplified implementation
    // In a real implementation, you'd need to compare function objects properly
    LogError("UnregisterInputCallback not fully implemented");
}

void RaylibInput::ClearInputCallbacks()
{
    input_callbacks.clear();
}

Vector2 RaylibInput::GetWorldMousePosition(Camera2D camera)
{
    return ::GetScreenToWorld2D(GetMousePosition(), camera);
}

bool RaylibInput::IsMouseInRectangle(Rectangle rect)
{
    return ::CheckCollisionPointRec(GetMousePosition(), rect);
}

int RaylibInput::MapVirtualKey(int virtual_key)
{
    // TODO: Implement virtual key mapping for Windows compatibility
    return virtual_key;
}

int RaylibInput::MapScanCode(int scan_code)
{
    // TODO: Implement scan code mapping for Windows compatibility
    return scan_code;
}

void RaylibInput::StartInputRecording()
{
    recording_input = true;
    recorded_events.clear();
}

void RaylibInput::StopInputRecording()
{
    recording_input = false;
}

void RaylibInput::StartInputPlayback()
{
    playing_input = true;
    playback_index = 0;
}

void RaylibInput::StopInputPlayback()
{
    playing_input = false;
    playback_index = 0;
}

bool RaylibInput::IsInputRecording()
{
    return recording_input;
}

bool RaylibInput::IsInputPlaying()
{
    return playing_input;
}

std::string RaylibInput::GetLastError() const
{
    return last_error;
}

void RaylibInput::LogError(const std::string& error)
{
    last_error = error;
    std::cerr << "RaylibInput Error: " << error << std::endl;
}

void RaylibInput::UpdateKeyboard()
{
    // Update keyboard state
    for (int i = 0; i < 512; i++)
    {
        prev_keys[i] = keys[i];
        keys[i] = IsKeyDown(i);
    }
    
    // Handle text input
    if (text_input_active)
    {
        int key = GetCharPressed();
        while (key > 0)
        {
            if (key >= 32 && key <= 125)
            {
                text_input += static_cast<char>(key);
            }
            key = GetCharPressed();
        }
        
        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE) && !text_input.empty())
        {
            text_input.pop_back();
        }
    }
}

void RaylibInput::UpdateMouse()
{
    // Update mouse state
    mouse_position = GetMousePosition();
    mouse_delta = GetMouseDelta();
    mouse_wheel_delta = GetMouseWheelMove();
    
    // Update mouse buttons
    for (int i = 0; i < 8; i++)
    {
        prev_mouse_buttons[i] = mouse_buttons[i];
        mouse_buttons[i] = IsMouseButtonDown(i);
    }
}

void RaylibInput::UpdateGamepads()
{
    // Update gamepad state
    for (int gamepad = 0; gamepad < 4; gamepad++)
    {
        gamepad_available[gamepad] = IsGamepadAvailable(gamepad);
        
        if (gamepad_available[gamepad])
        {
            // Update buttons
            for (int button = 0; button < 16; button++)
            {
                prev_gamepad_buttons[gamepad][button] = gamepad_buttons[gamepad][button];
                gamepad_buttons[gamepad][button] = IsGamepadButtonDown(gamepad, button);
            }
            
            // Update axes
            for (int axis = 0; axis < 8; axis++)
            {
                gamepad_axes[gamepad][axis] = GetGamepadAxisMovement(gamepad, axis);
            }
        }
    }
}

void RaylibInput::ProcessInputEvents()
{
    // Process and trigger input events
    // This is a simplified implementation
    // In a full implementation, you'd generate events for all input changes
    
    // Example: Check for key presses
    for (int i = 0; i < 512; i++)
    {
        if (IsKeyPressed(i))
        {
            RaylibInputEvent event;
            event.type = RAYLIB_INPUT_EVENT_KEY_DOWN;
            event.key_code = i;
            TriggerInputCallback(event);
        }
    }
    
    // Example: Check for mouse button presses
    for (int i = 0; i < 8; i++)
    {
        if (IsMouseButtonPressed(i))
        {
            RaylibInputEvent event;
            event.type = RAYLIB_INPUT_EVENT_MOUSE_DOWN;
            event.mouse_button = i;
            event.mouse_position = GetMousePosition();
            TriggerInputCallback(event);
        }
    }
}

void RaylibInput::TriggerInputCallback(const RaylibInputEvent& event)
{
    for (auto& callback : input_callbacks)
    {
        callback(event);
    }
}

// C-style interface implementations
extern "C" {

void InitRaylibInput()
{
    if (!g_input)
    {
        g_input = new RaylibInput();
        g_input->Initialize();
    }
}

void CleanupRaylibInput()
{
    if (g_input)
    {
        delete g_input;
        g_input = nullptr;
    }
}

void UpdateRaylibInput()
{
    if (g_input)
    {
        g_input->Update();
    }
}

bool IsRaylibKeyDown(int key)
{
    return g_input ? g_input->IsKeyDown(key) : false;
}

bool IsRaylibKeyPressed(int key)
{
    return g_input ? g_input->IsKeyPressed(key) : false;
}

bool IsRaylibKeyReleased(int key)
{
    return g_input ? g_input->IsKeyReleased(key) : false;
}

bool IsRaylibKeyUp(int key)
{
    return g_input ? g_input->IsKeyUp(key) : false;
}

bool IsRaylibMouseButtonDown(int button)
{
    return g_input ? g_input->IsMouseButtonDown(button) : false;
}

bool IsRaylibMouseButtonPressed(int button)
{
    return g_input ? g_input->IsMouseButtonPressed(button) : false;
}

bool IsRaylibMouseButtonReleased(int button)
{
    return g_input ? g_input->IsMouseButtonReleased(button) : false;
}

bool IsRaylibMouseButtonUp(int button)
{
    return g_input ? g_input->IsMouseButtonUp(button) : false;
}

void GetRaylibMousePosition(int* x, int* y)
{
    if (g_input)
    {
        Vector2 pos = g_input->GetMousePosition();
        *x = (int)pos.x;
        *y = (int)pos.y;
    }
    else
    {
        *x = 0;
        *y = 0;
    }
}

void GetRaylibMouseDelta(int* dx, int* dy)
{
    if (g_input)
    {
        Vector2 delta = g_input->GetMouseDelta();
        *dx = (int)delta.x;
        *dy = (int)delta.y;
    }
    else
    {
        *dx = 0;
        *dy = 0;
    }
}

float GetRaylibMouseWheelMove()
{
    return g_input ? g_input->GetMouseWheelMove() : 0.0f;
}

void SetRaylibMousePosition(int x, int y)
{
    if (g_input)
    {
        g_input->SetMousePosition(x, y);
    }
}

void SetRaylibMouseCursor(int cursor_type)
{
    if (g_input)
    {
        g_input->SetMouseCursor(cursor_type);
    }
}

void ShowRaylibCursor()
{
    if (g_input)
    {
        g_input->ShowCursor();
    }
}

void HideRaylibCursor()
{
    if (g_input)
    {
        g_input->HideCursor();
    }
}

bool IsRaylibCursorHidden()
{
    return g_input ? g_input->IsCursorHidden() : false;
}

bool IsRaylibGamepadAvailable(int gamepad_id)
{
    return g_input ? g_input->IsGamepadAvailable(gamepad_id) : false;
}

bool IsRaylibGamepadButtonDown(int gamepad_id, int button)
{
    return g_input ? g_input->IsGamepadButtonDown(gamepad_id, button) : false;
}

bool IsRaylibGamepadButtonPressed(int gamepad_id, int button)
{
    return g_input ? g_input->IsGamepadButtonPressed(gamepad_id, button) : false;
}

bool IsRaylibGamepadButtonReleased(int gamepad_id, int button)
{
    return g_input ? g_input->IsGamepadButtonReleased(gamepad_id, button) : false;
}

bool IsRaylibGamepadButtonUp(int gamepad_id, int button)
{
    return g_input ? g_input->IsGamepadButtonUp(gamepad_id, button) : false;
}

float GetRaylibGamepadAxisMovement(int gamepad_id, int axis)
{
    return g_input ? g_input->GetGamepadAxisMovement(gamepad_id, axis) : 0.0f;
}

int GetRaylibGamepadButtonPressed()
{
    return g_input ? g_input->GetGamepadButtonPressed() : -1;
}

const char* GetRaylibGamepadName(int gamepad_id)
{
    if (g_input)
    {
        static std::string name = g_input->GetGamepadName(gamepad_id);
        return name.c_str();
    }
    return "";
}

void StartRaylibTextInput()
{
    if (g_input)
    {
        g_input->StartTextInput();
    }
}

void StopRaylibTextInput()
{
    if (g_input)
    {
        g_input->StopTextInput();
    }
}

bool IsRaylibTextInputActive()
{
    return g_input ? g_input->IsTextInputActive() : false;
}

const char* GetRaylibTextInput()
{
    if (g_input)
    {
        static std::string text = g_input->GetTextInput();
        return text.c_str();
    }
    return "";
}

void ClearRaylibTextInput()
{
    if (g_input)
    {
        g_input->ClearTextInput();
    }
}

int GetRaylibCharPressed()
{
    return g_input ? g_input->GetCharPressed() : 0;
}

void GetRaylibWorldMousePosition(float camera_x, float camera_y, float camera_zoom, int* world_x, int* world_y)
{
    if (g_input)
    {
        Camera2D camera = { {camera_x, camera_y}, {0, 0}, 0.0f, camera_zoom };
        Vector2 world_pos = g_input->GetWorldMousePosition(camera);
        *world_x = (int)world_pos.x;
        *world_y = (int)world_pos.y;
    }
    else
    {
        *world_x = 0;
        *world_y = 0;
    }
}

bool IsRaylibMouseInRectangle(int x, int y, int width, int height)
{
    if (g_input)
    {
        Rectangle rect = { (float)x, (float)y, (float)width, (float)height };
        return g_input->IsMouseInRectangle(rect);
    }
    return false;
}

} // extern "C" 