#include "../include/legacy/Input_compat.hpp"

#include "engine_core/InputAdapter.hpp"
#include "engine_core/EngineCore.hpp"

#include <GLFW/glfw3.h>

namespace legacy { namespace input_compat {

int32_t mouseX = 0;
int32_t mouseY = 0;
bool Lpressed = false;
bool Rpressed = false;

static MouseStack g_lastEvent{};
static bool g_hasEvent = false;

void SetMPtr(int32_t x, int32_t y) {
    mouseX = x;
    mouseY = y;
    engine_core::input::SetMPtr(x, y);
}

void AddMouseEvent(int32_t x, int32_t y, bool L, bool R) {
    engine_core::input::AddMouseEvent(L ? engine_core::input::MouseEventType::LeftDown : engine_core::input::MouseEventType::LeftUp, x, y);
    engine_core::input::AddMouseEvent(R ? engine_core::input::MouseEventType::RightDown : engine_core::input::MouseEventType::RightUp, x, y);
    g_lastEvent = MouseStack{x, y, L, R, L, R};
    g_hasEvent = true;
    Lpressed = L; Rpressed = R;
}

MouseStack* ReadMEvent() {
    static MouseStack ret;
    engine_core::input::MouseEvent ev;
    if (engine_core::input::ReadMEvent(ev)) {
        // Translate one event into legacy snapshot semantics
        ret.x = ev.x; ret.y = ev.y;
        switch (ev.type) {
        case engine_core::input::MouseEventType::LeftDown: Lpressed = true; break;
        case engine_core::input::MouseEventType::LeftUp: Lpressed = false; break;
        case engine_core::input::MouseEventType::RightDown: Rpressed = true; break;
        case engine_core::input::MouseEventType::RightUp: Rpressed = false; break;
        default: break;
        }
        ret.Lpressed = Lpressed;
        ret.Rpressed = Rpressed;
        ret.rLpressed = Lpressed;
        ret.rRpressed = Rpressed;
        mouseX = ev.x; mouseY = ev.y;
        return &ret;
    }
    if (g_hasEvent) { g_hasEvent = false; return &g_lastEvent; }
    return nullptr;
}

void SetKeyState(int32_t key, bool pressed) { engine_core::input::SetKeyState(key, pressed); }
bool IsKeyPressed(int32_t key) { return engine_core::input::IsKeyPressed(key); }

bool ProcessMessages() {
    if (GLFWwindow* w = engine_core::get_active_window()) {
        glfwPollEvents();
        return true;
    }
    return false;
}

} } // namespace legacy::input_compat


