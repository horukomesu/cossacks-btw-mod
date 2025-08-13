#pragma once

#include <cstdint>

namespace game_logic {

// Start gameplay logic systems and a dedicated simulation thread.
// Default tickrate is 60 Hz (one tick every ~16.666 ms).
void initialize();

// Stop the simulation thread and shutdown gameplay logic systems.
void shutdown();

// Set desired simulation frequency in ticks per second. Values <= 0 pause ticking.
void set_target_tick_hz(int ticksPerSecond);

// Query current target tick frequency (Hz). Returns 0 if paused.
int  get_target_tick_hz();

// Game speed mode compatible with legacy exFMode (0=Slow, 1=Fast).
// Maps to target tick hz: Slow=30 Hz, Fast=60 Hz.
void set_speed_mode(int mode0slow1fast);
int  get_speed_mode();

// Pause/resume simulation loop without changing target Hz (0=run, 1=pause).
void set_paused(bool paused);
bool is_paused();

// Monotonic simulation tick counter, incremented each processed tick.
std::uint64_t get_simulation_tick_counter();

}  // namespace game_logic
