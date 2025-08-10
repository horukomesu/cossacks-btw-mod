#include "audio_core/Music.hpp"
#include "audio_core/AudioCore.hpp"
#include "resource_io/ResourceIO.hpp"

#include <AL/al.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#if defined(_WIN32)
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#  include <windows.h>
#  include <mfapi.h>
#  include <mfidl.h>
#  include <mfreadwrite.h>
#  pragma comment(lib, "mfplat.lib")
#  pragma comment(lib, "mfreadwrite.lib")
#  pragma comment(lib, "mfuuid.lib")
#endif
#include <cstring>

namespace audio_core { namespace music {

namespace {
struct TrackInfo {
    std::string filePath;  // absolute or data-root relative path
};

std::vector<TrackInfo> gTracks;    // 1-based logical numbering like CD tracks
std::array<int, 32> gTracksMask{}; // nation-indexed preferred track numbers
int gStartTrack = 1;
int gNTracks = 0; // count
int gPrevTrack1 = -1, gPrevTrack2 = -1, gPrevTrack3 = -1;
int gNextCommand = -1; // -1 auto, [0..] explicit track number
int gPlayMode = 2;     // 0 off, 1 nation, 2 random
int gCurrentNation = -1;
int gVolume = 50; // percent
std::string gDir = "BUILD/Music";

// OpenAL state
ALuint gSource = 0;
ALuint gBuffer = 0;
bool gIsPlaying = false;

// Decode entire MP3 file to memory PCM (interleaved S16). Returns sample rate and channels.
static bool decode_mp3_file(const std::string& path, std::vector<int16_t>& outPCM, int& outRate, int& outCh) {
#if defined(_WIN32)
    // Initialize Media Foundation on first use
    static bool s_mfInit = false;
    if (!s_mfInit) {
        if (SUCCEEDED(MFStartup(MF_VERSION))) s_mfInit = true;
    }
    if (!s_mfInit) return false;

    // Convert to wide path
    int wlen = MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, nullptr, 0);
    if (wlen <= 0) return false;
    std::wstring wpath; wpath.resize(static_cast<size_t>(wlen));
    MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, wpath.data(), wlen);

    IMFSourceReader* pReader = nullptr;
    HRESULT hr = MFCreateSourceReaderFromURL(wpath.c_str(), nullptr, &pReader);
    if (FAILED(hr) || !pReader) return false;

    IMFMediaType* pOutType = nullptr;
    hr = MFCreateMediaType(&pOutType);
    if (SUCCEEDED(hr)) hr = pOutType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
    if (SUCCEEDED(hr)) hr = pOutType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
    if (SUCCEEDED(hr)) hr = pOutType->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, 16);
    if (SUCCEEDED(hr)) hr = pReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, pOutType);
    if (pOutType) pOutType->Release();
    if (FAILED(hr)) { pReader->Release(); return false; }

    IMFMediaType* pCurrType = nullptr;
    hr = pReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &pCurrType);
    if (FAILED(hr) || !pCurrType) { pReader->Release(); return false; }
    UINT32 rate = 0, ch = 0;
    rate = MFGetAttributeUINT32(pCurrType, MF_MT_AUDIO_SAMPLES_PER_SECOND, 44100);
    ch   = MFGetAttributeUINT32(pCurrType, MF_MT_AUDIO_NUM_CHANNELS, 2);
    pCurrType->Release();
    outRate = static_cast<int>(rate);
    outCh = static_cast<int>(ch);
    if (outRate <= 0 || (outCh != 1 && outCh != 2)) { pReader->Release(); return false; }

    outPCM.clear();
    for (;;) {
        DWORD dwFlags = 0; LONGLONG ts = 0; IMFSample* pSample = nullptr;
        hr = pReader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &dwFlags, &ts, &pSample);
        if (FAILED(hr)) break;
        if (dwFlags & MF_SOURCE_READERF_ENDOFSTREAM) { if (pSample) pSample->Release(); break; }
        if (!pSample) continue;
        IMFMediaBuffer* pBuffer = nullptr;
        hr = pSample->ConvertToContiguousBuffer(&pBuffer);
        if (SUCCEEDED(hr) && pBuffer) {
            BYTE* pData = nullptr; DWORD maxLen = 0, curLen = 0;
            if (SUCCEEDED(pBuffer->Lock(&pData, &maxLen, &curLen))) {
                size_t count = static_cast<size_t>(curLen / sizeof(int16_t));
                size_t old = outPCM.size();
                outPCM.resize(old + count);
                std::memcpy(outPCM.data() + old, pData, curLen);
                pBuffer->Unlock();
            }
            pBuffer->Release();
        }
        pSample->Release();
    }
    pReader->Release();
    return !outPCM.empty();
#else
    (void)path; (void)outPCM; (void)outRate; (void)outCh; return false;
#endif
}

static void stop_internal() {
    if (gSource) {
        alSourceStop(gSource);
        alDeleteSources(1, &gSource);
        gSource = 0;
    }
    if (gBuffer) {
        alDeleteBuffers(1, &gBuffer);
        gBuffer = 0;
    }
    gIsPlaying = false;
}

static void apply_gain() {
    if (!gSource) return;
    const float gain = std::clamp(gVolume / 100.0f, 0.0f, 1.0f);
    alSourcef(gSource, AL_GAIN, gain);
}

static void play_file(const std::string& path) {
    stop_internal();
    std::vector<int16_t> pcm;
    int rate = 0, ch = 0;
    if (!decode_mp3_file(path, pcm, rate, ch)) {
        return;
    }
    const ALenum fmt = (ch == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16);
    alGenBuffers(1, &gBuffer);
    if (!gBuffer) return;
    alBufferData(gBuffer, fmt, pcm.data(), static_cast<ALsizei>(pcm.size() * sizeof(int16_t)), rate);
    alGenSources(1, &gSource);
    if (!gSource) {
        alDeleteBuffers(1, &gBuffer); gBuffer = 0; return;
    }
    alSourcei(gSource, AL_BUFFER, static_cast<ALint>(gBuffer));
    alSourcei(gSource, AL_SOURCE_RELATIVE, AL_TRUE);
    apply_gain();
    alSourcePlay(gSource);
    gIsPlaying = true;
}

static int pick_random_track() {
    if (gPlayMode == 1 && gCurrentNation >= 0) {
        const int t = gTracksMask[static_cast<size_t>(gCurrentNation)];
        if (t > 0 && t <= gNTracks) return t;
    }
    if (gNTracks <= 0) return -1;
    // Use tick-based randomness like legacy: GetTickCount() equivalent
    const uint32_t tick = static_cast<uint32_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count());
    int track = -1;
    do {
        track = static_cast<int>((tick & 4095u) * static_cast<uint32_t>(gNTracks) >> 12) + gStartTrack;
        if (track == gPrevTrack1 || track == gPrevTrack2 || track == gPrevTrack3) track = -1;
    } while (track == -1);
    return track;
}

static std::string to_lower_copy(const std::string& s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(), [](unsigned char c){ return static_cast<char>(std::tolower(c)); });
    return r;
}

static int parse_tracks_cd(const std::string& contents) {
    // Expected: first line StartTrack NTracks, then N lines of mask ints
    int start = 1, n = 0;
    int assigned = 0;
    std::istringstream in(contents);
    if (!(in >> start >> n)) return 0;
    gStartTrack = std::max(1, start);
    gNTracks = std::max(0, n);
    for (int i = 0; i < 32; ++i) gTracksMask[i] = gStartTrack;
    for (int i = 0; i < gNTracks && i < 32; ++i) {
        int v = gStartTrack + i;
        if (in >> v) { gTracksMask[static_cast<size_t>(i)] = v; ++assigned; }
    }
    return assigned;
}
} // namespace

bool initialize() {
    // nothing beyond AudioCore/OpenAL context
    return true;
}

void shutdown() {
    stop_internal();
    gTracks.clear();
}

void set_track_directory(const std::string& relativeDir) {
    gDir = relativeDir;
}

int reload_tracks() {
    gTracks.clear();
    gStartTrack = 1;
    gNTracks = 0;
    gPrevTrack1 = gPrevTrack2 = gPrevTrack3 = -1;

    // Try to parse Tracks.cd from data root if present (optional)
    std::vector<unsigned char> cdBytes;
    if (resource_io::read_file_anywhere("Tracks.cd", cdBytes)) {
        std::string txt(cdBytes.begin(), cdBytes.end());
        parse_tracks_cd(txt);
    }

    // Scan directory under data root for .mp3 files
    std::filesystem::path base = gDir;
    // Normalize spaces/backslashes in stored path handling via resource_io data root
    // We will join resource_io data root externally; here assume working dir is already set as data root
    std::error_code ec;
    if (!std::filesystem::exists(base, ec)) {
        // fallback: try without slash variants
        base = std::filesystem::path("BUILD") / "Music";
    }
    std::vector<std::filesystem::path> found;
    if (std::filesystem::exists(base, ec)) {
        for (const auto& de : std::filesystem::directory_iterator(base, ec)) {
            if (!de.is_regular_file()) continue;
            const std::string ext = to_lower_copy(de.path().extension().string());
            if (ext == ".mp3") found.push_back(de.path());
        }
    }
    std::sort(found.begin(), found.end());
    for (const auto& p : found) {
        TrackInfo ti; ti.filePath = p.string(); gTracks.push_back(std::move(ti));
    }
    gNTracks = static_cast<int>(gTracks.size());
    if (gNTracks > 0 && gStartTrack < 1) gStartTrack = 1;
    return gNTracks;
}

void set_volume_percent(int vol01to100) { gVolume = std::clamp(vol01to100, 0, 100); apply_gain(); }
int get_volume_percent() { return gVolume; }

void set_play_mode(int mode) { gPlayMode = (mode < 0 ? 0 : (mode > 2 ? 2 : mode)); }
int get_play_mode() { return gPlayMode; }

void set_current_nation(int nationIndex) { gCurrentNation = nationIndex; }
int get_current_nation() { return gCurrentNation; }

void play_track_number(int trackNumber) {
    if (trackNumber < gStartTrack) return;
    int idx = trackNumber - gStartTrack; // 0-based index into gTracks
    if (idx < 0 || idx >= static_cast<int>(gTracks.size())) return;
    const auto& ti = gTracks[static_cast<size_t>(idx)];
    play_file(ti.filePath);
    gPrevTrack3 = gPrevTrack2; gPrevTrack2 = gPrevTrack1; gPrevTrack1 = trackNumber;
}

void play_random_next() {
    if (gPlayMode == 0 || gNTracks <= 0) return;
    const int t = pick_random_track();
    if (t > 0) play_track_number(t);
}

void stop() { stop_internal(); }
void pause() { if (gSource) alSourcePause(gSource); }
void resume() { if (gSource) { alSourcePlay(gSource); apply_gain(); } }

void update() {
    if (!gSource) return;
    ALint state = 0; alGetSourcei(gSource, AL_SOURCE_STATE, &state);
    if (state == AL_STOPPED) {
        stop_internal();
        // Auto-advance like CD_MCINotify successful
        if (gPlayMode != 0) play_random_next();
    }
}

} } // namespace audio_core::music


