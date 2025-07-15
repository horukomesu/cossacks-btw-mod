#include "raylib_audio.h"
#include <iostream>
#include <algorithm>
#include <cmath>

// Global audio instance
RaylibAudio* g_audio = nullptr;

RaylibAudio::RaylibAudio()
    : audio_initialized(false)
    , master_volume(1.0f)
    , max_channels(RAYLIB_AUDIO_MAX_CHANNELS)
    , music_playing(false)
    , music_volume(1.0f)
    , audio_3d_enabled(false)
{
    // Initialize listener position
    listener_position = { 0.0f, 0.0f, 0.0f };
    listener_forward = { 0.0f, 0.0f, -1.0f };
    listener_up = { 0.0f, 1.0f, 0.0f };
    
    // Initialize audio channels
    channels.resize(max_channels);
}

RaylibAudio::~RaylibAudio()
{
    Shutdown();
}

bool RaylibAudio::Initialize()
{
    try
    {
        // Initialize audio device (done in raylib main)
        audio_initialized = IsAudioDeviceReady();
        
        if (!audio_initialized)
        {
            LogError("Failed to initialize audio device");
            return false;
        }
        
        // Set master volume
        SetMasterVolume(master_volume);
        
        last_error = "";
        return true;
    }
    catch (const std::exception& e)
    {
        LogError("Exception during audio initialization: " + std::string(e.what()));
        return false;
    }
}

void RaylibAudio::Shutdown()
{
    if (!audio_initialized)
        return;
    
    // Stop all sounds and music
    StopAllSounds();
    StopMusic();
    
    // Unload all sounds
    for (auto& sound : sounds)
    {
        UnloadSound(sound.second);
    }
    sounds.clear();
    
    // Unload all music
    for (auto& music : music_tracks)
    {
        UnloadMusicStream(music.second);
    }
    music_tracks.clear();
    
    // Clear channels
    for (auto& channel : channels)
    {
        channel.is_playing = false;
        channel.name.clear();
    }
    
    audio_initialized = false;
}

void RaylibAudio::SetMasterVolume(float volume)
{
    master_volume = std::max(0.0f, std::min(1.0f, volume));
    SetMasterVolume(master_volume);
}

float RaylibAudio::GetMasterVolume() const
{
    return master_volume;
}

bool RaylibAudio::LoadSound(const std::string& name, const std::string& filepath)
{
    try
    {
        Sound sound = LoadSound(filepath.c_str());
        if (sound.frameCount == 0)
        {
            LogError("Failed to load sound: " + filepath);
            return false;
        }
        
        // Unload existing sound if it exists
        if (sounds.find(name) != sounds.end())
        {
            UnloadSound(sounds[name]);
        }
        
        sounds[name] = sound;
        return true;
    }
    catch (const std::exception& e)
    {
        LogError("Exception loading sound " + filepath + ": " + e.what());
        return false;
    }
}

bool RaylibAudio::LoadSoundFromMemory(const std::string& name, const unsigned char* data, int size)
{
    try
    {
        // Raylib doesn't directly support loading from memory for sounds
        // We'll need to use a temporary file approach
        std::string temp_file = "temp_sound_" + name + ".wav";
        
        // Write data to temporary file
        FILE* file = fopen(temp_file.c_str(), "wb");
        if (!file)
        {
            LogError("Failed to create temporary file for sound: " + name);
            return false;
        }
        
        fwrite(data, 1, size, file);
        fclose(file);
        
        // Load from temporary file
        bool result = LoadSound(name, temp_file);
        
        // Clean up temporary file
        remove(temp_file.c_str());
        
        return result;
    }
    catch (const std::exception& e)
    {
        LogError("Exception loading sound from memory: " + std::string(e.what()));
        return false;
    }
}

void RaylibAudio::UnloadSound(const std::string& name)
{
    auto it = sounds.find(name);
    if (it != sounds.end())
    {
        // Stop any playing instances of this sound
        for (auto& channel : channels)
        {
            if (channel.is_playing && channel.name == name)
            {
                StopSoundMulti(channel.sound);
                channel.is_playing = false;
                channel.name.clear();
            }
        }
        
        UnloadSound(it->second);
        sounds.erase(it);
    }
}

Sound* RaylibAudio::GetSound(const std::string& name)
{
    auto it = sounds.find(name);
    return (it != sounds.end()) ? &it->second : nullptr;
}

bool RaylibAudio::LoadMusic(const std::string& name, const std::string& filepath)
{
    try
    {
        Music music = LoadMusicStream(filepath.c_str());
        if (music.frameCount == 0)
        {
            LogError("Failed to load music: " + filepath);
            return false;
        }
        
        // Unload existing music if it exists
        if (music_tracks.find(name) != music_tracks.end())
        {
            UnloadMusicStream(music_tracks[name]);
        }
        
        music_tracks[name] = music;
        return true;
    }
    catch (const std::exception& e)
    {
        LogError("Exception loading music " + filepath + ": " + e.what());
        return false;
    }
}

bool RaylibAudio::LoadMusicFromMemory(const std::string& name, const unsigned char* data, int size)
{
    try
    {
        // Similar to sound loading, use temporary file
        std::string temp_file = "temp_music_" + name + ".ogg";
        
        FILE* file = fopen(temp_file.c_str(), "wb");
        if (!file)
        {
            LogError("Failed to create temporary file for music: " + name);
            return false;
        }
        
        fwrite(data, 1, size, file);
        fclose(file);
        
        bool result = LoadMusic(name, temp_file);
        
        remove(temp_file.c_str());
        
        return result;
    }
    catch (const std::exception& e)
    {
        LogError("Exception loading music from memory: " + std::string(e.what()));
        return false;
    }
}

void RaylibAudio::UnloadMusic(const std::string& name)
{
    auto it = music_tracks.find(name);
    if (it != music_tracks.end())
    {
        // Stop music if it's currently playing
        if (current_music == name && music_playing)
        {
            StopMusic();
        }
        
        UnloadMusicStream(it->second);
        music_tracks.erase(it);
    }
}

Music* RaylibAudio::GetMusic(const std::string& name)
{
    auto it = music_tracks.find(name);
    return (it != music_tracks.end()) ? &it->second : nullptr;
}

int RaylibAudio::PlaySound(const std::string& name, float volume, float pitch, float pan)
{
    Sound* sound = GetSound(name);
    if (!sound)
    {
        LogError("Sound not found: " + name);
        return RAYLIB_AUDIO_INVALID_CHANNEL;
    }
    
    int channel = FindFreeChannel();
    if (channel == RAYLIB_AUDIO_INVALID_CHANNEL)
    {
        LogError("No free audio channels available");
        return RAYLIB_AUDIO_INVALID_CHANNEL;
    }
    
    // Set up the channel
    channels[channel].sound = *sound;
    channels[channel].is_playing = true;
    channels[channel].volume = volume;
    channels[channel].pitch = pitch;
    channels[channel].pan = pan;
    channels[channel].loop = false;
    channels[channel].name = name;
    channels[channel].priority = 0;
    
    // Apply sound properties
    SetSoundVolume(channels[channel].sound, volume * master_volume);
    SetSoundPitch(channels[channel].sound, pitch);
    
    // Play the sound
    PlaySoundMulti(channels[channel].sound);
    
    return channel;
}

int RaylibAudio::PlaySoundEx(const std::string& name, float volume, float pitch, float pan, bool loop, int priority)
{
    Sound* sound = GetSound(name);
    if (!sound)
    {
        LogError("Sound not found: " + name);
        return RAYLIB_AUDIO_INVALID_CHANNEL;
    }
    
    int channel = FindFreeChannel();
    if (channel == RAYLIB_AUDIO_INVALID_CHANNEL)
    {
        // Try to find a lower priority channel to replace
        int lowest_priority = INT_MAX;
        int lowest_channel = RAYLIB_AUDIO_INVALID_CHANNEL;
        
        for (int i = 0; i < max_channels; i++)
        {
            if (channels[i].is_playing && channels[i].priority < priority && channels[i].priority < lowest_priority)
            {
                lowest_priority = channels[i].priority;
                lowest_channel = i;
            }
        }
        
        if (lowest_channel != RAYLIB_AUDIO_INVALID_CHANNEL)
        {
            StopSound(lowest_channel);
            channel = lowest_channel;
        }
        else
        {
            LogError("No free audio channels available");
            return RAYLIB_AUDIO_INVALID_CHANNEL;
        }
    }
    
    // Set up the channel
    channels[channel].sound = *sound;
    channels[channel].is_playing = true;
    channels[channel].volume = volume;
    channels[channel].pitch = pitch;
    channels[channel].pan = pan;
    channels[channel].loop = loop;
    channels[channel].name = name;
    channels[channel].priority = priority;
    
    // Apply sound properties
    SetSoundVolume(channels[channel].sound, volume * master_volume);
    SetSoundPitch(channels[channel].sound, pitch);
    
    // Play the sound
    PlaySoundMulti(channels[channel].sound);
    
    return channel;
}

void RaylibAudio::StopSound(int channel)
{
    if (channel >= 0 && channel < max_channels && channels[channel].is_playing)
    {
        StopSoundMulti(channels[channel].sound);
        ClearChannel(channel);
    }
}

void RaylibAudio::StopAllSounds()
{
    for (int i = 0; i < max_channels; i++)
    {
        if (channels[i].is_playing)
        {
            StopSoundMulti(channels[i].sound);
            ClearChannel(i);
        }
    }
}

void RaylibAudio::PauseSound(int channel)
{
    if (channel >= 0 && channel < max_channels && channels[channel].is_playing)
    {
        PauseSoundMulti(channels[channel].sound);
    }
}

void RaylibAudio::ResumeSound(int channel)
{
    if (channel >= 0 && channel < max_channels && channels[channel].is_playing)
    {
        ResumeSoundMulti(channels[channel].sound);
    }
}

void RaylibAudio::SetSoundVolume(int channel, float volume)
{
    if (channel >= 0 && channel < max_channels && channels[channel].is_playing)
    {
        channels[channel].volume = volume;
        SetSoundVolume(channels[channel].sound, volume * master_volume);
    }
}

void RaylibAudio::SetSoundPitch(int channel, float pitch)
{
    if (channel >= 0 && channel < max_channels && channels[channel].is_playing)
    {
        channels[channel].pitch = pitch;
        SetSoundPitch(channels[channel].sound, pitch);
    }
}

void RaylibAudio::SetSoundPan(int channel, float pan)
{
    if (channel >= 0 && channel < max_channels && channels[channel].is_playing)
    {
        channels[channel].pan = pan;
        SetSoundPan(channels[channel].sound, pan);
    }
}

bool RaylibAudio::IsSoundPlaying(int channel)
{
    if (channel >= 0 && channel < max_channels && channels[channel].is_playing)
    {
        return IsSoundPlaying(channels[channel].sound);
    }
    return false;
}

void RaylibAudio::PlayMusic(const std::string& name, bool loop)
{
    Music* music = GetMusic(name);
    if (!music)
    {
        LogError("Music not found: " + name);
        return;
    }
    
    // Stop current music if playing
    if (music_playing)
    {
        StopMusic();
    }
    
    current_music = name;
    music_playing = true;
    
    // Set music properties
    music->looping = loop;
    SetMusicVolume(*music, music_volume * master_volume);
    
    // Play music
    PlayMusicStream(*music);
}

void RaylibAudio::StopMusic()
{
    if (music_playing && !current_music.empty())
    {
        Music* music = GetMusic(current_music);
        if (music)
        {
            StopMusicStream(*music);
        }
    }
    
    music_playing = false;
    current_music.clear();
}

void RaylibAudio::PauseMusic()
{
    if (music_playing && !current_music.empty())
    {
        Music* music = GetMusic(current_music);
        if (music)
        {
            PauseMusicStream(*music);
        }
    }
}

void RaylibAudio::ResumeMusic()
{
    if (music_playing && !current_music.empty())
    {
        Music* music = GetMusic(current_music);
        if (music)
        {
            ResumeMusicStream(*music);
        }
    }
}

void RaylibAudio::SetMusicVolume(float volume)
{
    music_volume = std::max(0.0f, std::min(1.0f, volume));
    
    if (music_playing && !current_music.empty())
    {
        Music* music = GetMusic(current_music);
        if (music)
        {
            SetMusicVolume(*music, music_volume * master_volume);
        }
    }
}

float RaylibAudio::GetMusicVolume() const
{
    return music_volume;
}

bool RaylibAudio::IsMusicPlaying() const
{
    if (music_playing && !current_music.empty())
    {
        Music* music = GetMusic(current_music);
        if (music)
        {
            return IsMusicStreamPlaying(*music);
        }
    }
    return false;
}

void RaylibAudio::UpdateMusic()
{
    if (music_playing && !current_music.empty())
    {
        Music* music = GetMusic(current_music);
        if (music)
        {
            UpdateMusicStream(*music);
        }
    }
    
    // Clean up finished sound channels
    CleanupFinishedChannels();
}

void RaylibAudio::Enable3DAudio(bool enabled)
{
    audio_3d_enabled = enabled;
}

void RaylibAudio::SetListenerPosition(Vector3 position)
{
    listener_position = position;
}

void RaylibAudio::SetListenerOrientation(Vector3 forward, Vector3 up)
{
    listener_forward = forward;
    listener_up = up;
}

int RaylibAudio::PlaySound3D(const std::string& name, Vector3 position, float volume, float pitch)
{
    if (!audio_3d_enabled)
    {
        return PlaySound(name, volume, pitch, 0.0f);
    }
    
    // Calculate distance-based volume attenuation
    float distance = Vector3Distance(listener_position, position);
    float distance_volume = 1.0f / (1.0f + distance * 0.1f); // Simple distance attenuation
    
    // Calculate pan based on position relative to listener
    Vector3 to_source = Vector3Subtract(position, listener_position);
    Vector3 right = Vector3CrossProduct(listener_forward, listener_up);
    float pan = Vector3DotProduct(to_source, right) * 0.5f; // Simple pan calculation
    
    return PlaySound(name, volume * distance_volume, pitch, pan);
}

void RaylibAudio::SetSound3DPosition(int channel, Vector3 position)
{
    if (!audio_3d_enabled || channel < 0 || channel >= max_channels || !channels[channel].is_playing)
        return;
    
    // Update 3D position for the channel
    float distance = Vector3Distance(listener_position, position);
    float distance_volume = 1.0f / (1.0f + distance * 0.1f);
    
    Vector3 to_source = Vector3Subtract(position, listener_position);
    Vector3 right = Vector3CrossProduct(listener_forward, listener_up);
    float pan = Vector3DotProduct(to_source, right) * 0.5f;
    
    SetSoundVolume(channel, channels[channel].volume * distance_volume);
    SetSoundPan(channel, pan);
}

void RaylibAudio::SetMaxChannels(int max_ch)
{
    max_channels = std::max(1, std::min(max_ch, 256));
    channels.resize(max_channels);
}

int RaylibAudio::GetMaxChannels() const
{
    return max_channels;
}

int RaylibAudio::GetActiveChannels() const
{
    int active = 0;
    for (const auto& channel : channels)
    {
        if (channel.is_playing)
            active++;
    }
    return active;
}

int RaylibAudio::FindFreeChannel()
{
    for (int i = 0; i < max_channels; i++)
    {
        if (!channels[i].is_playing)
            return i;
    }
    return RAYLIB_AUDIO_INVALID_CHANNEL;
}

void RaylibAudio::ClearChannel(int channel)
{
    if (channel >= 0 && channel < max_channels)
    {
        channels[channel].is_playing = false;
        channels[channel].name.clear();
        channels[channel].volume = 1.0f;
        channels[channel].pitch = 1.0f;
        channels[channel].pan = 0.0f;
        channels[channel].loop = false;
        channels[channel].priority = 0;
    }
}

void RaylibAudio::CleanupFinishedChannels()
{
    for (int i = 0; i < max_channels; i++)
    {
        if (channels[i].is_playing && !IsSoundPlaying(channels[i].sound))
        {
            ClearChannel(i);
        }
    }
}

float RaylibAudio::GetSoundLength(const std::string& name)
{
    Sound* sound = GetSound(name);
    if (sound)
    {
        return GetSoundLength(*sound);
    }
    return 0.0f;
}

float RaylibAudio::GetMusicLength(const std::string& name)
{
    Music* music = GetMusic(name);
    if (music)
    {
        return GetMusicTimeLength(*music);
    }
    return 0.0f;
}

bool RaylibAudio::IsAudioDeviceReady() const
{
    return audio_initialized;
}

std::string RaylibAudio::GetLastError() const
{
    return last_error;
}

void RaylibAudio::LogError(const std::string& error)
{
    last_error = error;
    std::cerr << "RaylibAudio Error: " << error << std::endl;
}

// C-style interface implementations
extern "C" {

void InitRaylibAudio()
{
    if (!g_audio)
    {
        g_audio = new RaylibAudio();
        g_audio->Initialize();
    }
}

void CleanupRaylibAudio()
{
    if (g_audio)
    {
        delete g_audio;
        g_audio = nullptr;
    }
}

void UpdateRaylibAudio()
{
    if (g_audio)
    {
        g_audio->UpdateMusic();
    }
}

void SetRaylibMasterVolume(float volume)
{
    if (g_audio)
    {
        g_audio->SetMasterVolume(volume);
    }
}

float GetRaylibMasterVolume()
{
    return g_audio ? g_audio->GetMasterVolume() : 0.0f;
}

bool LoadRaylibSound(const char* name, const char* filepath)
{
    return g_audio ? g_audio->LoadSound(name, filepath) : false;
}

void UnloadRaylibSound(const char* name)
{
    if (g_audio)
    {
        g_audio->UnloadSound(name);
    }
}

int PlayRaylibSound(const char* name, float volume)
{
    return g_audio ? g_audio->PlaySound(name, volume) : RAYLIB_AUDIO_INVALID_CHANNEL;
}

int PlayRaylibSoundEx(const char* name, float volume, float pitch, float pan, bool loop, int priority)
{
    return g_audio ? g_audio->PlaySoundEx(name, volume, pitch, pan, loop, priority) : RAYLIB_AUDIO_INVALID_CHANNEL;
}

void StopRaylibSound(int channel)
{
    if (g_audio)
    {
        g_audio->StopSound(channel);
    }
}

void StopAllRaylibSounds()
{
    if (g_audio)
    {
        g_audio->StopAllSounds();
    }
}

void PauseRaylibSound(int channel)
{
    if (g_audio)
    {
        g_audio->PauseSound(channel);
    }
}

void ResumeRaylibSound(int channel)
{
    if (g_audio)
    {
        g_audio->ResumeSound(channel);
    }
}

void SetRaylibSoundVolume(int channel, float volume)
{
    if (g_audio)
    {
        g_audio->SetSoundVolume(channel, volume);
    }
}

void SetRaylibSoundPitch(int channel, float pitch)
{
    if (g_audio)
    {
        g_audio->SetSoundPitch(channel, pitch);
    }
}

void SetRaylibSoundPan(int channel, float pan)
{
    if (g_audio)
    {
        g_audio->SetSoundPan(channel, pan);
    }
}

bool IsRaylibSoundPlaying(int channel)
{
    return g_audio ? g_audio->IsSoundPlaying(channel) : false;
}

bool LoadRaylibMusic(const char* name, const char* filepath)
{
    return g_audio ? g_audio->LoadMusic(name, filepath) : false;
}

void UnloadRaylibMusic(const char* name)
{
    if (g_audio)
    {
        g_audio->UnloadMusic(name);
    }
}

void PlayRaylibMusic(const char* name, bool loop)
{
    if (g_audio)
    {
        g_audio->PlayMusic(name, loop);
    }
}

void StopRaylibMusic()
{
    if (g_audio)
    {
        g_audio->StopMusic();
    }
}

void PauseRaylibMusic()
{
    if (g_audio)
    {
        g_audio->PauseMusic();
    }
}

void ResumeRaylibMusic()
{
    if (g_audio)
    {
        g_audio->ResumeMusic();
    }
}

void SetRaylibMusicVolume(float volume)
{
    if (g_audio)
    {
        g_audio->SetMusicVolume(volume);
    }
}

float GetRaylibMusicVolume()
{
    return g_audio ? g_audio->GetMusicVolume() : 0.0f;
}

bool IsRaylibMusicPlaying()
{
    return g_audio ? g_audio->IsMusicPlaying() : false;
}

void EnableRaylib3DAudio(bool enabled)
{
    if (g_audio)
    {
        g_audio->Enable3DAudio(enabled);
    }
}

void SetRaylibListenerPosition(float x, float y, float z)
{
    if (g_audio)
    {
        g_audio->SetListenerPosition({ x, y, z });
    }
}

void SetRaylibListenerOrientation(float fx, float fy, float fz, float ux, float uy, float uz)
{
    if (g_audio)
    {
        g_audio->SetListenerOrientation({ fx, fy, fz }, { ux, uy, uz });
    }
}

int PlayRaylibSound3D(const char* name, float x, float y, float z, float volume)
{
    return g_audio ? g_audio->PlaySound3D(name, { x, y, z }, volume) : RAYLIB_AUDIO_INVALID_CHANNEL;
}

void SetRaylibSound3DPosition(int channel, float x, float y, float z)
{
    if (g_audio)
    {
        g_audio->SetSound3DPosition(channel, { x, y, z });
    }
}

float GetRaylibSoundLength(const char* name)
{
    return g_audio ? g_audio->GetSoundLength(name) : 0.0f;
}

float GetRaylibMusicLength(const char* name)
{
    return g_audio ? g_audio->GetMusicLength(name) : 0.0f;
}

bool IsRaylibAudioDeviceReady()
{
    return g_audio ? g_audio->IsAudioDeviceReady() : false;
}

} // extern "C" 