#pragma once

#include <cstdint>

namespace legacy { namespace time_compat {

// Milliseconds since process start (wraps around on 32-bit like WinAPI GetTickCount)
std::uint32_t GetTickCount();

// Milliseconds monotonic (does not wrap for many years); used as GetRealTime replacement
std::uint64_t GetRealTime();

// Initialization (called from engine setup)
void initialize();

} } // namespace legacy::time_compat


