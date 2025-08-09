#include <cstdint>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine_core/EngineCore.hpp"

namespace legacy::ddini {

void FlipPages() {
    if (GLFWwindow* w = engine_core::get_active_window()) {
        glfwSwapBuffers(w);
    }
}

void LockSurface() {
    // No-op in GL path
}

void UnlockSurface() {
    // No-op in GL path
}

} // namespace legacy::ddini
