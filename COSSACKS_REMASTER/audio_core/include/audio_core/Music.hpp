#pragma once

#include <string>

namespace audio_core {
namespace music {

// Initialize/shutdown music subsystem. Uses OpenAL for playback.
bool initialize();
void shutdown();

// Set relative directory (under data root) where MP3 files reside. Default: "BUILD/ Music" (space ignored)
// Call before reload_tracks().
void set_track_directory(const std::string& relativeDir);

// Rescan directory for .mp3 files and build the track map. Also parses optional Tracks.cd.
// Returns number of discovered tracks.
int reload_tracks();

// Configure playback parameters
void set_volume_percent(int vol01to100);
int get_volume_percent();

// 0 = Off, 1 = By-nation (TracksMask[CurrentNation]), 2 = Random
void set_play_mode(int mode);
int get_play_mode();

// -1 means unknown/no nation. 0..15 map to TracksMask entries if available.
void set_current_nation(int nationIndex);
int get_current_nation();

// Control playback
void play_track_number(int trackNumber);
void play_random_next();
void stop();
void pause();
void resume();

// Per-frame maintenance (refill streaming buffers, detect end-of-track, etc.)
void update();

} // namespace music
} // namespace audio_core


