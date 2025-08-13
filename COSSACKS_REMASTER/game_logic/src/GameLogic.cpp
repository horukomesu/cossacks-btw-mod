#include "game_logic/GameLogic.hpp"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <thread>

namespace game_logic {

namespace {
// Threading state for simulation loop
std::thread g_simThread;
std::mutex g_mutex;
std::condition_variable g_cv;
std::atomic<bool> g_running{false};
std::atomic<bool> g_paused{false};
std::atomic<int>  g_targetHz{60};
std::atomic<int>  g_speedMode{1}; // legacy fast=1 default
std::atomic<std::uint64_t> g_tickCounter{0};

void simulation_tick() {
    // TODO: When gameplay subsystems are ported, execute one deterministic step here
    // For now we just increment a counter to prove progression
    g_tickCounter.fetch_add(1, std::memory_order_relaxed);
}

void simulation_thread_proc() {
    using clock_mono = std::chrono::steady_clock;
    auto nextWake = clock_mono::now();
    while (g_running.load(std::memory_order_acquire)) {
        int hz = g_targetHz.load(std::memory_order_relaxed);
        const bool paused = g_paused.load(std::memory_order_relaxed) || (hz <= 0);

        if (!paused) {
            // Compute period; clamp to sane bounds [1..1000] Hz
            if (hz < 1) hz = 1; else if (hz > 1000) hz = 1000;
            const auto period = std::chrono::nanoseconds(1'000'000'000LL / static_cast<long long>(hz));
            // Catch up if we fell behind; process multiple ticks if needed
            const auto now = clock_mono::now();
            if (now >= nextWake) {
                // Initialize nextWake if first iteration
                if (nextWake.time_since_epoch().count() == 0) {
                    nextWake = now + period;
                } else {
                    // Process as many ticks as needed to catch up, but cap to avoid spiral of death
                    int maxCatchUp = 8;
                    do {
                        simulation_tick();
                        nextWake += period;
                        --maxCatchUp;
                    } while (now >= nextWake && maxCatchUp > 0);
                }
            } else {
                // We're ahead of schedule
            }
            // Sleep until nextWake or until signaled to change
            std::unique_lock<std::mutex> lk(g_mutex);
            g_cv.wait_until(lk, nextWake, [](){ return !g_running.load(std::memory_order_acquire); });
        } else {
            // Paused: wait for unpause or shutdown
            std::unique_lock<std::mutex> lk(g_mutex);
            g_cv.wait(lk, [](){ return !g_running.load(std::memory_order_acquire) || !g_paused.load(std::memory_order_relaxed) || g_targetHz.load(std::memory_order_relaxed) > 0; });
            nextWake = clock_mono::now();
        }
    }
}
} // namespace

void initialize() {
    // Map speed mode to default Hz
    const int mode = g_speedMode.load(std::memory_order_relaxed);
    g_targetHz.store(mode == 0 ? 30 : 60, std::memory_order_relaxed);
    g_running.store(true, std::memory_order_release);
    g_simThread = std::thread(simulation_thread_proc);
    std::cout << "[game_logic] initialized (tick thread started at "
              << g_targetHz.load() << " Hz)" << std::endl;
}

void shutdown() {
    if (!g_running.exchange(false, std::memory_order_acq_rel)) return;
    {
        std::lock_guard<std::mutex> lg(g_mutex);
        // nothing
    }
    g_cv.notify_all();
    if (g_simThread.joinable()) g_simThread.join();
    std::cout << "[game_logic] shutdown" << std::endl;
}

void set_target_tick_hz(int ticksPerSecond) {
    g_targetHz.store(ticksPerSecond, std::memory_order_relaxed);
    g_cv.notify_all();
}

int get_target_tick_hz() {
    return g_targetHz.load(std::memory_order_relaxed);
}

void set_speed_mode(int mode0slow1fast) {
    const int m = (mode0slow1fast <= 0) ? 0 : 1;
    g_speedMode.store(m, std::memory_order_relaxed);
    // Legacy mapping: slow=30 Hz, fast=60 Hz
    g_targetHz.store(m == 0 ? 30 : 60, std::memory_order_relaxed);
    g_cv.notify_all();
}

int get_speed_mode() {
    return g_speedMode.load(std::memory_order_relaxed);
}

void set_paused(bool paused) {
    g_paused.store(paused, std::memory_order_relaxed);
    g_cv.notify_all();
}

bool is_paused() {
    return g_paused.load(std::memory_order_relaxed);
}

std::uint64_t get_simulation_tick_counter() {
    return g_tickCounter.load(std::memory_order_relaxed);
}

}  // namespace game_logic
