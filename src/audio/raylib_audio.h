#pragma once

#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

// Forward declarations
struct RaylibSound;
struct RaylibMusic;

// Audio channel for sound mixing
struct AudioChannel
{
    Sound sound;
    bool is_playing;
    float volume;
    float pitch;
    float pan;
    bool loop;
    int priority;
    std::string name;
    
    AudioChannel() : is_playing(false), volume(1.0f), pitch(1.0f), pan(0.0f), loop(false), priority(0) {}
};

// Raylib Audio System
class RaylibAudio
{
private:
    // Audio device
    bool audio_initialized;
    float master_volume;
    
    // Sounds and music
    std::unordered_map<std::string, Sound> sounds;
    std::unordered_map<std::string, Music> music_tracks;
    
    // Audio channels for mixing
    std::vector<AudioChannel> channels;
    int max_channels;
    
    // Current music
    std::string current_music;
    bool music_playing;
    float music_volume;
    
    // 3D Audio support
    bool audio_3d_enabled;
    Vector3 listener_position;
    Vector3 listener_forward;
    Vector3 listener_up;
    
public:
    RaylibAudio();
    ~RaylibAudio();
    
    // Initialization
    bool Initialize();
    void Shutdown();
    
    // Master volume control
    void SetMasterVolume(float volume);
    float GetMasterVolume() const;
    
    // Sound loading and management
    bool LoadSound(const std::string& name, const std::string& filepath);
    bool LoadSoundFromMemory(const std::string& name, const unsigned char* data, int size);
    void UnloadSound(const std::string& name);
    Sound* GetSound(const std::string& name);
    
    // Music loading and management
    bool LoadMusic(const std::string& name, const std::string& filepath);
    bool LoadMusicFromMemory(const std::string& name, const unsigned char* data, int size);
    void UnloadMusic(const std::string& name);
    Music* GetMusic(const std::string& name);
    
    // Sound playing
    int PlaySound(const std::string& name, float volume = 1.0f, float pitch = 1.0f, float pan = 0.0f);
    int PlaySoundEx(const std::string& name, float volume, float pitch, float pan, bool loop, int priority);
    void StopSound(int channel);
    void StopAllSounds();
    void PauseSound(int channel);
    void ResumeSound(int channel);
    
    // Sound properties
    void SetSoundVolume(int channel, float volume);
    void SetSoundPitch(int channel, float pitch);
    void SetSoundPan(int channel, float pan);
    bool IsSoundPlaying(int channel);
    
    // Music playing
    void PlayMusic(const std::string& name, bool loop = true);
    void StopMusic();
    void PauseMusic();
    void ResumeMusic();
    void SetMusicVolume(float volume);
    float GetMusicVolume() const;
    bool IsMusicPlaying() const;
    void UpdateMusic(); // Must be called every frame
    
    // 3D Audio support
    void Enable3DAudio(bool enabled);
    void SetListenerPosition(Vector3 position);
    void SetListenerOrientation(Vector3 forward, Vector3 up);
    int PlaySound3D(const std::string& name, Vector3 position, float volume = 1.0f, float pitch = 1.0f);
    void SetSound3DPosition(int channel, Vector3 position);
    
    // Channel management
    void SetMaxChannels(int max_channels);
    int GetMaxChannels() const;
    int GetActiveChannels() const;
    int FindFreeChannel();
    void ClearChannel(int channel);
    
    // Audio streaming (for large audio files)
    bool StartAudioStream(const std::string& name, const std::string& filepath);
    void StopAudioStream(const std::string& name);
    void UpdateAudioStream(const std::string& name);
    
    // Audio effects (basic)
    void SetAudioEffect(int channel, int effect_type, float intensity);
    void ClearAudioEffect(int channel);
    
    // Utility functions
    float GetSoundLength(const std::string& name);
    float GetMusicLength(const std::string& name);
    bool IsAudioDeviceReady() const;
    
    // Error handling
    std::string GetLastError() const;
    
private:
    std::string last_error;
    
    // Helper functions
    void LogError(const std::string& error);
    int FindChannelByName(const std::string& name);
    void CleanupFinishedChannels();
    bool LoadSoundInternal(const std::string& name, const std::string& filepath);
    bool LoadMusicInternal(const std::string& name, const std::string& filepath);
};

// Global audio instance
extern RaylibAudio* g_audio;

// C-style interface for compatibility with original code
extern "C" {
    // Initialization
    void InitRaylibAudio();
    void CleanupRaylibAudio();
    void UpdateRaylibAudio();
    
    // Master volume
    void SetRaylibMasterVolume(float volume);
    float GetRaylibMasterVolume();
    
    // Sound loading
    bool LoadRaylibSound(const char* name, const char* filepath);
    void UnloadRaylibSound(const char* name);
    
    // Sound playing
    int PlayRaylibSound(const char* name, float volume);
    int PlayRaylibSoundEx(const char* name, float volume, float pitch, float pan, bool loop, int priority);
    void StopRaylibSound(int channel);
    void StopAllRaylibSounds();
    void PauseRaylibSound(int channel);
    void ResumeRaylibSound(int channel);
    
    // Sound properties
    void SetRaylibSoundVolume(int channel, float volume);
    void SetRaylibSoundPitch(int channel, float pitch);
    void SetRaylibSoundPan(int channel, float pan);
    bool IsRaylibSoundPlaying(int channel);
    
    // Music loading
    bool LoadRaylibMusic(const char* name, const char* filepath);
    void UnloadRaylibMusic(const char* name);
    
    // Music playing
    void PlayRaylibMusic(const char* name, bool loop);
    void StopRaylibMusic();
    void PauseRaylibMusic();
    void ResumeRaylibMusic();
    void SetRaylibMusicVolume(float volume);
    float GetRaylibMusicVolume();
    bool IsRaylibMusicPlaying();
    
    // 3D Audio
    void EnableRaylib3DAudio(bool enabled);
    void SetRaylibListenerPosition(float x, float y, float z);
    void SetRaylibListenerOrientation(float fx, float fy, float fz, float ux, float uy, float uz);
    int PlayRaylibSound3D(const char* name, float x, float y, float z, float volume);
    void SetRaylibSound3DPosition(int channel, float x, float y, float z);
    
    // Utility
    float GetRaylibSoundLength(const char* name);
    float GetRaylibMusicLength(const char* name);
    bool IsRaylibAudioDeviceReady();
}

// Audio effect types
enum RaylibAudioEffect
{
    RAYLIB_AUDIO_EFFECT_NONE = 0,
    RAYLIB_AUDIO_EFFECT_REVERB,
    RAYLIB_AUDIO_EFFECT_ECHO,
    RAYLIB_AUDIO_EFFECT_DISTORTION,
    RAYLIB_AUDIO_EFFECT_LOWPASS,
    RAYLIB_AUDIO_EFFECT_HIGHPASS
};

// Audio constants
#define RAYLIB_AUDIO_MAX_CHANNELS 64
#define RAYLIB_AUDIO_DEFAULT_VOLUME 1.0f
#define RAYLIB_AUDIO_DEFAULT_PITCH 1.0f
#define RAYLIB_AUDIO_DEFAULT_PAN 0.0f
#define RAYLIB_AUDIO_INVALID_CHANNEL -1 