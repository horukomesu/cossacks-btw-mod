#pragma once

#include <cstdint>

namespace engine_core::input {

// Event types roughly mirroring the legacy system
enum class MouseEventType : int32_t {
    None = 0,
    Move = 1,
    LeftDown = 2,
    LeftUp = 3,
    RightDown = 4,
    RightUp = 5,
    Wheel = 6
};

struct MouseEvent {
    MouseEventType type{MouseEventType::None};
    int32_t x{0};
    int32_t y{0};
    int32_t param{0}; // wheel delta or modifiers
};

// Initializes internal queues; called from engine init
void initialize();

// Legacy-like API
void SetMPtr(int32_t x, int32_t y);
void AddMouseEvent(MouseEventType type, int32_t x, int32_t y, int32_t param = 0);
bool ReadMEvent(MouseEvent& outEvent);

// Key state (very simplified for now)
void SetKeyState(int32_t key, bool pressed);
bool IsKeyPressed(int32_t key);

// Mouse buttons (legacy globals approximations)
void SetLeftPressed(bool pressed);
void SetRightPressed(bool pressed);
bool GetLeftPressed();
bool GetRightPressed();

// Latest pointer position
void GetPointer(int32_t& outX, int32_t& outY);

} // namespace engine_core::input
