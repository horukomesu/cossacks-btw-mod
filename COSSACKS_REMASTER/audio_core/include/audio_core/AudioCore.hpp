#pragma once

namespace audio_core {

bool initialize();
void shutdown();

// Placeholder; real impl will load WAV and play
void play_test_tone();

} // namespace audio_core
