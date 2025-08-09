#include "legacy/Time_compat.hpp"

#include <atomic>
#include <chrono>

namespace legacy { namespace time_compat {

using clock_mono = std::chrono::steady_clock;
static std::atomic<bool> g_init{false};
static clock_mono::time_point g_start;

void initialize() {
    g_start = clock_mono::now();
    g_init.store(true, std::memory_order_release);
}

std::uint32_t GetTickCount() {
    if (!g_init.load(std::memory_order_acquire)) initialize();
    const auto now = clock_mono::now();
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - g_start).count();
    // Emulate WinAPI GetTickCount 32-bit wrap
    return static_cast<std::uint32_t>(ms & 0xFFFFFFFFu);
}

std::uint64_t GetRealTime() {
    if (!g_init.load(std::memory_order_acquire)) initialize();
    const auto now = clock_mono::now();
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - g_start).count();
    return static_cast<std::uint64_t>(ms);
}

} } // namespace legacy::time_compat


