#include "engine_core/EngineCore.hpp"

#include <iostream>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#if defined(_WIN32)
#include <windows.h>
#endif

#include "engine_core/InputAdapter.hpp"
#include "engine_core/Render2D.hpp"
#include "legacy/Globals_compat.hpp"

namespace engine_core {

static GLFWwindow* gWindow = nullptr;
static std::function<void(int,int)> gFrameCallback;

GLFWwindow* get_active_window() {
    return gWindow;
}

void set_frame_callback(std::function<void(int,int)> cb) {
    gFrameCallback = std::move(cb);
}

void set_system_cursor_visible(bool visible) {
    if (!gWindow) return;
    glfwSetInputMode(gWindow, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
}

static void glfw_error_callback(int error, const char* description) {
    std::cerr << "[engine_core] GLFW error " << error << ": " << description << std::endl;
}

static void glfw_cursor_pos_callback(GLFWwindow*, double x, double y) {
    input::SetMPtr(static_cast<int>(x), static_cast<int>(y));
    input::AddMouseEvent(input::MouseEventType::Move, static_cast<int>(x), static_cast<int>(y));
}

static void glfw_mouse_button_callback(GLFWwindow*, int button, int action, int /*mods*/) {
    double xd = 0.0, yd = 0.0;
    glfwGetCursorPos(gWindow, &xd, &yd);
    const int x = static_cast<int>(xd);
    const int y = static_cast<int>(yd);

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        const bool pressed = action == GLFW_PRESS;
        input::SetLeftPressed(pressed);
        input::AddMouseEvent(pressed ? input::MouseEventType::LeftDown : input::MouseEventType::LeftUp, x, y);
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        const bool pressed = action == GLFW_PRESS;
        input::SetRightPressed(pressed);
        input::AddMouseEvent(pressed ? input::MouseEventType::RightDown : input::MouseEventType::RightUp, x, y);
    }
}

static void glfw_scroll_callback(GLFWwindow*, double /*xoffset*/, double yoffset) {
    double xd = 0.0, yd = 0.0;
    glfwGetCursorPos(gWindow, &xd, &yd);
    input::AddMouseEvent(input::MouseEventType::Wheel, static_cast<int>(xd), static_cast<int>(yd), static_cast<int>(yoffset));
}

static void glfw_key_callback(GLFWwindow*, int key, int /*scancode*/, int action, int /*mods*/) {
    if (key >= 0) {
        input::SetKeyState(key, action != GLFW_RELEASE);
    }
}

bool initialize_window(int width, int height, const char* title) {
    input::initialize();

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::cerr << "[engine_core] Failed to initialize GLFW" << std::endl;
    #if defined(_WIN32)
        MessageBoxA(nullptr, "Failed to initialize GLFW", "engine_core", MB_OK | MB_ICONERROR);
    #endif
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    gWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!gWindow) {
        std::cerr << "[engine_core] Failed to create window" << std::endl;
    #if defined(_WIN32)
        MessageBoxA(nullptr, "Failed to create GLFW window", "engine_core", MB_OK | MB_ICONERROR);
    #endif
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(gWindow);
    glfwSwapInterval(1); // vsync

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "[engine_core] Failed to initialize GLAD" << std::endl;
    #if defined(_WIN32)
        MessageBoxA(nullptr, "Failed to initialize GLAD (OpenGL loader)", "engine_core", MB_OK | MB_ICONERROR);
    #endif
        return false;
    }

    int fbw = 0, fbh = 0;
    glfwGetFramebufferSize(gWindow, &fbw, &fbh);
    glViewport(0, 0, fbw, fbh);
    legacy::globals::set_framebuffer_size(fbw, fbh);

    // Setup callbacks
    glfwSetCursorPosCallback(gWindow, glfw_cursor_pos_callback);
    glfwSetMouseButtonCallback(gWindow, glfw_mouse_button_callback);
    glfwSetScrollCallback(gWindow, glfw_scroll_callback);
    glfwSetKeyCallback(gWindow, glfw_key_callback);

    render2d::initialize();

    std::cout << "[engine_core] OpenGL context initialized: "
              << glGetString(GL_VERSION) << std::endl;
    return true;
}

void run_main_loop(int maxFrames) {
    int frameCount = 0;
    while (gWindow && !glfwWindowShouldClose(gWindow)) {
        glfwPollEvents();

        int fbw = 0, fbh = 0;
        glfwGetFramebufferSize(gWindow, &fbw, &fbh);
        glViewport(0, 0, fbw, fbh);
        legacy::globals::set_framebuffer_size(fbw, fbh);

        glClearColor(0.10f, 0.10f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        render2d::begin_frame(fbw, fbh);
        if (gFrameCallback) gFrameCallback(fbw, fbh);
        render2d::end_frame();

        glfwSwapBuffers(gWindow);

        if (maxFrames > 0) {
            ++frameCount;
            if (frameCount >= maxFrames) {
                glfwSetWindowShouldClose(gWindow, GLFW_TRUE);
            }
        }
    }
}

void shutdown() {
    render2d::shutdown();
    if (gWindow) {
        glfwDestroyWindow(gWindow);
        gWindow = nullptr;
    }
    glfwTerminate();
    std::cout << "[engine_core] shutdown" << std::endl;
}

}  // namespace engine_core
