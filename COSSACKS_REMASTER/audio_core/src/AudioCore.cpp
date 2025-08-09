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

} // namespace audio_core
