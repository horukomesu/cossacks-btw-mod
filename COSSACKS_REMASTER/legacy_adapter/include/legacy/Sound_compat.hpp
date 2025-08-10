#pragma once

#include <string>

namespace legacy { namespace sound_compat {

// Load SOUNDLIST.TXT into memory and prepare caches
bool initialize();
void shutdown();

// Returns an OpenAL buffer for a random entry of the group (loads and caches on demand)
unsigned int get_random_buffer(const std::string& groupName);

// Convenience: pick a random buffer from the group and play it immediately
// Returns true if a sound was played
bool play_group(const std::string& groupName, float gain = 1.0f, float panX = 0.0f);

// Load and cache a WAV by relative path (tries sounds\, Sound\, and exact). Returns buffer or 0.
unsigned int get_buffer_for_file(const std::string& relPath);

// Play a specific WAV file (via caching). Returns true if played.
bool play_file(const std::string& relPath, float gain = 1.0f, float panX = 0.0f);

} } // namespace legacy::sound_compat


