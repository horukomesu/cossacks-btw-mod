#pragma once

namespace audio_core {

bool initialize();
void shutdown();

// Placeholder; real impl will load WAV and play
void play_test_tone();

// Decode a little-endian PCM WAV from memory and create an OpenAL buffer.
// Supports 8/16-bit PCM, mono/stereo. Returns 0 on failure.
unsigned int create_buffer_from_wav_bytes(const unsigned char* data, size_t size);

// Play an OpenAL buffer once. gain in [0..1], pan in [-1..1] (source-relative X).
// Returns a source handle (>0) or 0 on failure. Source will be reclaimed in update().
unsigned int play_buffer(unsigned int buffer, float gain, float panX);

// Reclaim finished sources and perform maintenance. Call each frame.
void update();

} // namespace audio_core
