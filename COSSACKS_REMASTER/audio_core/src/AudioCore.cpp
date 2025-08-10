#include "audio_core/AudioCore.hpp"

#include <AL/al.h>
#include <AL/alc.h>
#include <cmath>
#include <iostream>
#include <vector>

namespace audio_core {

namespace {
ALCdevice* gDevice = nullptr;
ALCcontext* gContext = nullptr;
std::vector<ALuint> gLiveSources;
unsigned int gMaxSources = 64; // conservative default cap
}

bool initialize() {
    gDevice = alcOpenDevice(nullptr);
    if (!gDevice) {
        std::cerr << "[audio_core] Failed to open default device" << std::endl;
        return false;
    }
    gContext = alcCreateContext(gDevice, nullptr);
    if (!gContext || !alcMakeContextCurrent(gContext)) {
        std::cerr << "[audio_core] Failed to create/make current context" << std::endl;
        if (gContext) alcDestroyContext(gContext);
        alcCloseDevice(gDevice);
        gContext = nullptr;
        gDevice = nullptr;
        return false;
    }
    std::cout << "[audio_core] initialized" << std::endl;
    return true;
}

void shutdown() {
    alcMakeContextCurrent(nullptr);
    if (gContext) alcDestroyContext(gContext);
    if (gDevice) alcCloseDevice(gDevice);
    gContext = nullptr;
    gDevice = nullptr;
    std::cout << "[audio_core] shutdown" << std::endl;
}

void play_test_tone() {
    if (!gContext) return;
    constexpr int sampleRate = 44100;
    constexpr float durationSec = 0.2f;
    const int numSamples = static_cast<int>(sampleRate * durationSec);
    std::vector<short> samples(numSamples);
    const float frequency = 880.0f;
    for (int i = 0; i < numSamples; ++i) {
        float t = static_cast<float>(i) / sampleRate;
        float s = std::sin(2.0f * 3.14159265f * frequency * t);
        samples[i] = static_cast<short>(s * 32767);
    }
    ALuint buffer = 0, source = 0;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, AL_FORMAT_MONO16, samples.data(), static_cast<ALsizei>(samples.size() * sizeof(short)), sampleRate);
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    alSourcePlay(source);
}

static ALenum pick_format(int channels, int bitsPerSample) {
    if (channels == 1 && bitsPerSample == 8) return AL_FORMAT_MONO8;
    if (channels == 1 && bitsPerSample == 16) return AL_FORMAT_MONO16;
    if (channels == 2 && bitsPerSample == 8) return AL_FORMAT_STEREO8;
    if (channels == 2 && bitsPerSample == 16) return AL_FORMAT_STEREO16;
    return 0;
}

unsigned int create_buffer_from_wav_bytes(const unsigned char* data, size_t size) {
    if (!gContext || !data || size < 44) return 0;
    // Minimal WAV parser (PCM): RIFF/WAVE, fmt, data
    auto rd32 = [&](size_t off) -> uint32_t { if (off + 4 > size) return 0; uint32_t v; std::memcpy(&v, data + off, 4); return v; };
    auto rd16 = [&](size_t off) -> uint16_t { if (off + 2 > size) return 0; uint16_t v; std::memcpy(&v, data + off, 2); return v; };
    if (rd32(0) != 0x46464952 /*'RIFF'*/ || rd32(8) != 0x45564157 /*'WAVE'*/) return 0;
    size_t off = 12;
    int channels = 0, bits = 0, sampleRate = 0;
    const unsigned char* pData = nullptr; size_t dataSize = 0;
    while (off + 8 <= size) {
        uint32_t chunkId; std::memcpy(&chunkId, data + off, 4); off += 4; uint32_t chunkSize = rd32(off); off += 4;
        if (off + chunkSize > size) break;
        if (chunkId == 0x20746d66 /*'fmt '*/) {
            uint16_t audioFormat = rd16(off + 0);
            channels = rd16(off + 2);
            sampleRate = rd32(off + 4);
            bits = rd16(off + 14);
            if (audioFormat != 1) return 0; // PCM only
        } else if (chunkId == 0x61746164 /*'data'*/) {
            pData = data + off; dataSize = chunkSize;
        }
        off += chunkSize + (chunkSize & 1);
    }
    if (!pData || channels <= 0 || bits <= 0 || sampleRate <= 0) return 0;
    ALenum format = pick_format(channels, bits);
    if (!format) return 0;
    ALuint buffer = 0; alGenBuffers(1, &buffer);
    if (!buffer) return 0;
    alBufferData(buffer, format, pData, static_cast<ALsizei>(dataSize), sampleRate);
    return buffer;
}

unsigned int play_buffer(unsigned int buffer, float gain, float panX) {
    if (!gContext || buffer == 0) return 0;
    // Reclaim and cap sources
    update();
    if (gLiveSources.size() >= gMaxSources) {
        return 0;
    }
    ALuint src = 0; alGenSources(1, &src);
    if (!src) return 0;
    alSourcei(src, AL_BUFFER, static_cast<ALint>(buffer));
    alSourcef(src, AL_GAIN, (gain < 0.f ? 0.f : (gain > 1.f ? 1.f : gain)));
    alSource3f(src, AL_POSITION, panX, 0.0f, 0.0f);
    alSourcei(src, AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcePlay(src);
    gLiveSources.push_back(src);
    return src;
}

void update() {
    // Reclaim finished sources
    std::vector<ALuint> keep;
    keep.reserve(gLiveSources.size());
    for (ALuint s : gLiveSources) {
        ALint state = 0; alGetSourcei(s, AL_SOURCE_STATE, &state);
        if (state == AL_STOPPED) {
            alDeleteSources(1, &s);
        } else {
            keep.push_back(s);
        }
    }
    gLiveSources.swap(keep);
}

void set_max_live_sources(unsigned int maxSources) {
    gMaxSources = (maxSources == 0 ? 1u : maxSources);
}

} // namespace audio_core
