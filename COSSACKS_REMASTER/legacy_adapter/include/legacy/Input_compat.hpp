#pragma once

#include <cstdint>

namespace legacy { namespace input_compat {

struct MouseStack {
    int32_t x{0};
    int32_t y{0};
    bool Lpressed{false};
    bool Rpressed{false};
    bool rLpressed{false};
    bool rRpressed{false};
};

// Legacy-style globals
extern int32_t mouseX;
extern int32_t mouseY;
extern bool Lpressed;
extern bool Rpressed;

// Shims mirroring original signatures
void SetMPtr(int32_t x, int32_t y);
void AddMouseEvent(int32_t x, int32_t y, bool L, bool R);
MouseStack* ReadMEvent();

// Key shims (optional)
void SetKeyState(int32_t key, bool pressed);
bool IsKeyPressed(int32_t key);

// Proxy to platform message pump (glfwPollEvents)
bool ProcessMessages();

} } // namespace legacy::input_compat


