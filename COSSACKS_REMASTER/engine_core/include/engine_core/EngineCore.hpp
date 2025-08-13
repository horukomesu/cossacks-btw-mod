#pragma once

#include <functional>

struct GLFWwindow; // fwd decl

namespace engine_core {

// Initializes an OpenGL window via GLFW + GLAD. Returns false on failure.
bool initialize_window(int width, int height, const char* title);

// Runs the main loop. If maxFrames > 0, exits after that many frames.
void run_main_loop(int maxFrames = -1);

// Shuts down engine and destroys the window/context.
void shutdown();

// Adapter access
GLFWwindow* get_active_window();

// Set a per-frame callback called after frame begin and before swap
void set_frame_callback(std::function<void(int framebufferWidth, int framebufferHeight)> cb);

// Show/hide system cursor
void set_system_cursor_visible(bool visible);

  // Change window size (in pixels). No-op if window is not created.
  void set_window_size(int width, int height);

  // Get monotonic time in seconds since engine start
  double get_time_seconds();

}  // namespace engine_core
