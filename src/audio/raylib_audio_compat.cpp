#include "raylib_audio_compat.h"
#include "../Main executable/Cwave.h"
#include <algorithm>
#include <sstream>

// Global sound system instance
static RaylibAudio g_AudioSystem;
CDirSound CDIRSND;
CDirSound* CDS = &CDIRSND;

// Sound loading system compatibility
char* SoundID[MaxSnd];
word SndTable[MaxSnd][16];
byte SnDanger[MaxSnd];
word NSnd[MaxSnd];
word NSounds = 0;

// Internal sound management
static bool SoundOK = false;
static int CenterX = 0;
static int CenterY = 0;
static int SMinX = 0, SMaxX = 0, SMinY = 0, SMaxY = 0;
static int LoMinX = 0, LoMaxX = 0, LoMinY = 0, LoMaxY = 0;
static int FieldDelay = 0;

// External dependencies
extern short randoma[8192];
extern int srpos;
extern int FogMode;
extern int MyNation;
extern int mapx, mapy, smaplx, smaply;
extern int WarSound;

// Helper functions
int srando() {
    srpos++;
    srpos &= 8191;
    return randoma[srpos];
}

CDirSound::CDirSound()
{
    m_pAudioSystem = &g_AudioSystem;
    m_hWindow = NULL;
    m_initialized = false;
    m_currentBufferNum = 0;
    
    // Initialize arrays
    memset(Volume, 0, sizeof(Volume));
    memset(SrcX, 0, sizeof(SrcX));
    memset(SrcY, 0, sizeof(SrcY));
    memset(BufIsRun, 0, sizeof(BufIsRun));
    memset(m_bufferSizes, 0, sizeof(m_bufferSizes));
    
    // Initialize buffer management
    m_bufferActive.resize(MAXSND1, false);
    m_soundNames.resize(MAXSND1);
    m_bufferVolumes.resize(MAXSND1, 1.0f);
    m_bufferPans.resize(MAXSND1, 0.0f);
}

CDirSound::~CDirSound()
{
    ReleaseAll();
}

void CDirSound::CreateDirSound(HWND hWnd)
{
    m_hWindow = hWnd;
    
    // Initialize raylib audio
    if (!m_pAudioSystem->IsInitialized()) {
        m_pAudioSystem->Initialize();
    }
    
    m_initialized = m_pAudioSystem->IsInitialized();
    m_currentBufferNum = 0;
    
    // Reset all arrays
    memset(Volume, 0, sizeof(Volume));
    memset(SrcX, 0, sizeof(SrcX));
    memset(SrcY, 0, sizeof(SrcY));
    memset(BufIsRun, 0, sizeof(BufIsRun));
    
    SoundOK = m_initialized;
}

uint32_t CDirSound::CreateSoundBuffer(CWave* pWave)
{
    if (!m_initialized || !pWave || !pWave->WaveOK()) {
        return 0;
    }
    
    // Find available buffer slot
    for (uint32_t i = 1; i < MAXSND1; i++) {
        if (!m_bufferActive[i]) {
            // Create unique sound name
            std::ostringstream oss;
            oss << "sound_" << i << "_" << GetTickCount();
            std::string soundName = oss.str();
            
                         // Load sound data from CWave
             char* waveData = pWave->GetWaveDataPtr();
             uint32_t waveSize = pWave->GetWaveSize();
             WAVEFORMATEX_COMPAT* waveFormat = pWave->GetWaveFormatPtr();
             
             // Create temporary wave file for raylib (simplified approach)
             // In a full implementation, you would convert the wave data directly
             if (m_pAudioSystem->LoadSoundFromMemory(soundName, waveData, waveSize, waveFormat->nChannels, waveFormat->nSamplesPerSec, waveFormat->wBitsPerSample)) {
                m_bufferActive[i] = true;
                m_soundNames[i] = soundName;
                m_bufferSizes[i] = waveSize;
                Volume[i] = 0; // Default volume
                
                if (i > m_currentBufferNum) {
                    m_currentBufferNum = i;
                }
                
                return i;
            }
            break;
        }
    }
    
    return 0;
}

uint32_t CDirSound::DuplicateSoundBuffer(uint32_t bufferNum)
{
    if (!m_initialized || bufferNum == 0 || bufferNum > m_currentBufferNum) {
        return 0;
    }
    
    if (!m_bufferActive[bufferNum]) {
        return 0;
    }
    
    // Find available buffer slot
    for (uint32_t i = 1; i < MAXSND1; i++) {
        if (!m_bufferActive[i]) {
            // Create unique sound name for duplicate
            std::ostringstream oss;
            oss << m_soundNames[bufferNum] << "_dup_" << i;
            std::string soundName = oss.str();
            
            // Duplicate the sound
            if (m_pAudioSystem->DuplicateSound(m_soundNames[bufferNum], soundName)) {
                m_bufferActive[i] = true;
                m_soundNames[i] = soundName;
                m_bufferSizes[i] = m_bufferSizes[bufferNum];
                Volume[i] = Volume[bufferNum];
                
                if (i > m_currentBufferNum) {
                    m_currentBufferNum = i;
                }
                
                return i;
            }
            break;
        }
    }
    
    return 0;
}

BOOL CDirSound::CopyWaveToBuffer(CWave* pWave, uint32_t bufferNum)
{
    // In raylib implementation, sound data is loaded directly in CreateSoundBuffer
    // This function exists for DirectSound compatibility
    return (bufferNum > 0 && bufferNum <= m_currentBufferNum && m_bufferActive[bufferNum]);
}

BOOL CDirSound::PlaySound(uint32_t bufferNum)
{
    if (!m_initialized || bufferNum == 0 || bufferNum > m_currentBufferNum) {
        return FALSE;
    }
    
    if (!m_bufferActive[bufferNum]) {
        return FALSE;
    }
    
    // Stop the sound first (equivalent to SetCurrentPosition(0))
    m_pAudioSystem->StopSound(m_soundNames[bufferNum]);
    
    // Play the sound
    if (m_pAudioSystem->PlaySound(m_soundNames[bufferNum])) {
        BufIsRun[bufferNum] = 0;
        return TRUE;
    }
    
    return FALSE;
}

BOOL CDirSound::StopSound(uint32_t bufferNum)
{
    if (!m_initialized || bufferNum == 0 || bufferNum > m_currentBufferNum) {
        return FALSE;
    }
    
    if (!m_bufferActive[bufferNum]) {
        return FALSE;
    }
    
    m_pAudioSystem->StopSound(m_soundNames[bufferNum]);
    BufIsRun[bufferNum] = 0;
    return TRUE;
}

BOOL CDirSound::PlayCoorSound(uint32_t bufferNum, int x, int vx)
{
    if (!m_initialized || bufferNum == 0 || bufferNum > m_currentBufferNum) {
        return FALSE;
    }
    
    if (!m_bufferActive[bufferNum]) {
        return FALSE;
    }
    
    // Stop the sound first
    m_pAudioSystem->StopSound(m_soundNames[bufferNum]);
    
    // Play the sound with positional audio
    if (m_pAudioSystem->PlaySound3D(m_soundNames[bufferNum], x, 0, vx)) {
        BufIsRun[bufferNum] = 1;
        SrcX[bufferNum] = x;
        SrcY[bufferNum] = vx;
        return TRUE;
    }
    
    return FALSE;
}

void CDirSound::SetVolume(uint32_t bufferNum, int vol)
{
    if (!m_initialized || bufferNum == 0 || bufferNum > m_currentBufferNum) {
        return;
    }
    
    if (!m_bufferActive[bufferNum]) {
        return;
    }
    
    float volume = ConvertVolume(vol);
    m_bufferVolumes[bufferNum] = volume;
    m_pAudioSystem->SetSoundVolume(m_soundNames[bufferNum], volume);
}

void CDirSound::SetPan(uint32_t bufferNum, int pan)
{
    if (!m_initialized || bufferNum == 0 || bufferNum > m_currentBufferNum) {
        return;
    }
    
    if (!m_bufferActive[bufferNum]) {
        return;
    }
    
    float panValue = ConvertPan(pan);
    m_bufferPans[bufferNum] = panValue;
    m_pAudioSystem->SetSoundPan(m_soundNames[bufferNum], panValue);
}

void CDirSound::SetLastVolume(short Vol)
{
    if (m_currentBufferNum > 0) {
        Volume[m_currentBufferNum] = Vol;
    }
}

BOOL CDirSound::DirectSoundOK()
{
    return m_initialized ? TRUE : FALSE;
}

bool CDirSound::IsPlaying(uint32_t bufferNum)
{
    if (!m_initialized || bufferNum == 0 || bufferNum > m_currentBufferNum) {
        return false;
    }
    
    if (!m_bufferActive[bufferNum]) {
        return false;
    }
    
    return m_pAudioSystem->IsSoundPlaying(m_soundNames[bufferNum]);
}

int CDirSound::GetPos(uint32_t bufferNum)
{
    if (!m_initialized || bufferNum == 0 || bufferNum > m_currentBufferNum) {
        return 0;
    }
    
    if (!m_bufferActive[bufferNum]) {
        return 0;
    }
    
    // Get position in samples/bytes (simplified)
    return m_pAudioSystem->GetSoundPosition(m_soundNames[bufferNum]);
}

void CDirSound::ProcessSoundSystem()
{
    if (!m_initialized) {
        return;
    }
    
    // Update all playing sounds with positional audio
    for (uint32_t i = 1; i <= m_currentBufferNum; i++) {
        if (BufIsRun[i]) {
            ControlPan(i);
        }
    }
    
    // Update the audio system
    m_pAudioSystem->Update();
}

void CDirSound::ControlPan(uint32_t bufferNum)
{
    if (BufIsRun[bufferNum]) {
        // Update position for moving sounds
        SrcX[bufferNum] += SrcY[bufferNum];
        
        // Calculate pan based on distance from center
        int pan = (SrcX[bufferNum] - CenterX) << 1;
        if (pan < -4000) pan = -4000;
        if (pan > 4000) pan = 4000;
        
        SetPan(bufferNum, pan);
        
        // Occasionally check if sound is still playing
        if (rand() < 350) {
            if (!IsPlaying(bufferNum)) {
                BufIsRun[bufferNum] = 0;
            }
        }
    }
}

void CDirSound::ReleaseAll()
{
    if (m_initialized) {
        // Stop all sounds
        for (uint32_t i = 1; i <= m_currentBufferNum; i++) {
            if (m_bufferActive[i]) {
                m_pAudioSystem->StopSound(m_soundNames[i]);
                m_pAudioSystem->UnloadSound(m_soundNames[i]);
            }
        }
        
        // Reset state
        m_bufferActive.clear();
        m_soundNames.clear();
        m_bufferVolumes.clear();
        m_bufferPans.clear();
        m_currentBufferNum = 0;
    }
}

std::string CDirSound::GetSoundName(uint32_t bufferNum)
{
    if (bufferNum > 0 && bufferNum <= m_currentBufferNum) {
        return m_soundNames[bufferNum];
    }
    return "";
}

float CDirSound::ConvertVolume(int directSoundVolume)
{
    // DirectSound volume: 0 = full volume, negative values = quieter
    // Raylib volume: 0.0 = silence, 1.0 = full volume
    if (directSoundVolume <= -10000) {
        return 0.0f;
    }
    
    // Convert dB to linear scale (simplified)
    float volume = 1.0f + (directSoundVolume / 10000.0f);
    return std::max(0.0f, std::min(1.0f, volume));
}

float CDirSound::ConvertPan(int directSoundPan)
{
    // DirectSound pan: -10000 = left, 0 = center, +10000 = right
    // Raylib pan: -1.0 = left, 0.0 = center, +1.0 = right
    return std::max(-1.0f, std::min(1.0f, directSoundPan / 10000.0f));
}

// Sound effect system implementation
void PrepareSound() {
    if (FieldDelay) FieldDelay--;
    SMinX = mapx << 5;
    SMaxX = (mapx + smaplx) << 5;
    CenterX = (SMinX + SMaxX) >> 1;
    LoMinX = SMinX - 1024;
    LoMaxX = SMaxX + 1024;

    SMinY = mapy << 4;
    SMaxY = (mapy + smaply) << 4;
    CenterY = (SMinY + SMaxY) >> 1;
    LoMinY = SMinY - 512;
    LoMaxY = SMaxY + 512;
}

void PlayEffect(int n, int pan, int vol) {
    if (!SoundOK) return;
    vol -= (100 - WarSound) * 40;
    if (n < NSounds) {
        if (SnDanger[n]) FieldDelay = 400;
        if (NSnd[n] > 0) {
            int maxsnd = NSnd[n];
            int u = maxsnd;
            int nnn = (srando() * maxsnd) >> 15;
            bool sndmade = true;
            do {
                int sid = SndTable[n][nnn];
                bool poss = CDS->IsPlaying(sid);
                if (!poss) {
                    CDS->SetVolume(sid, vol);
                    CDS->SetPan(sid, pan);
                    CDS->PlaySound(sid);
                    sndmade = false;
                }
                else {
                    u--;
                    nnn++;
                    if (nnn >= maxsnd) nnn = 0;
                }
            } while (sndmade && u);
        }
    }
}

void PlayCoorEffect(int n, int x, int y, int pan, int vol) {
    if (!SoundOK) return;
    vol -= (100 - WarSound) * 40;
    if (n < NSounds) {
        if (SnDanger[n]) FieldDelay = 400;
        if (NSnd[n] > 0) {
            int maxsnd = NSnd[n];
            int u = maxsnd;
            int nnn = (srando() * maxsnd) >> 15;
            bool sndmade = true;
            do {
                int sid = SndTable[n][nnn];
                bool poss = CDS->IsPlaying(sid);
                if (!poss) {
                    CDS->SetVolume(sid, vol);
                    CDS->SetPan(sid, pan);
                    CDS->PlayCoorSound(sid, x, y);
                    sndmade = false;
                }
                else {
                    u--;
                    nnn++;
                    if (nnn >= maxsnd) nnn = 0;
                }
            } while (sndmade && u);
        }
    }
}

void PlaySingleEffect(int n, int pan, int vol) {
    if (!SoundOK) return;
    vol -= (100 - WarSound) * 40;
    if (n < NSounds) {
        if (SnDanger[n]) FieldDelay = 400;
        if (NSnd[n] > 0) {
            int maxsnd = NSnd[n];
            for (int i = 0; i < maxsnd; i++) {
                int sid = SndTable[n][i];
                if (CDS->IsPlaying(sid)) return;
            }

            int u = maxsnd;
            int nnn = (srando() * maxsnd) >> 15;
            bool sndmade = true;
            do {
                int sid = SndTable[n][nnn];
                bool poss = CDS->IsPlaying(sid);
                if (!poss) {
                    CDS->SetVolume(sid, vol);
                    CDS->SetPan(sid, pan);
                    CDS->PlaySound(sid);
                    sndmade = false;
                }
                else {
                    u--;
                    nnn++;
                    if (nnn >= maxsnd) nnn = 0;
                }
            } while (sndmade && u);
        }
    }
}

extern word GetFog(int x, int y);

void AddEffectV(int x, int y, int vx, int id) {
    if (!SoundOK) return;
    if (id < 0) return;
    if (x < LoMinX || x > LoMaxX || y < LoMinY || y > LoMaxY) return;
    
    // Fog checking
    if (FogMode && GetFog(x, y << 1) < 900) return;

    int pan = (x - CenterX) << 1;
    if (pan < -4000) pan = -4000;
    if (pan > 4000) pan = 4000;
    
    if (x >= SMinX && y >= SMinY && x < SMaxX && y < SMaxY) {
        PlayCoorEffect(id, x, vx, pan, 0);
    }
    else {
        PlayCoorEffect(id, x, vx, pan, -800);
    }
}

void AddEffect(int x, int y, int id) {
    AddEffectV(x, y, 0, id);
}

void AddSingleEffect(int x, int y, int id) {
    if (!SoundOK) return;
    if (id < 0) return;
    if (x < LoMinX || x > LoMaxX || y < LoMinY || y > LoMaxY) return;
    
    // Fog checking
    if (GetFog(x, y << 1) < 900 && FogMode) return;

    int pan = (x - CenterX) << 1;
    if (pan < -4000) pan = -4000;
    if (pan > 4000) pan = 4000;
    
    if (x >= SMinX && y >= SMinY && x < SMaxX && y < SMaxY) {
        PlaySingleEffect(id, pan, 0);
    }
    else {
        PlaySingleEffect(id, pan, -800);
    }
}

void AddUnlimitedEffect(int x, int y, int id) {
    if (!SoundOK) return;
    if (id < 0) return;
    
    // Fog checking
    if (GetFog(x, y << 1) < 900 && FogMode) return;

    int pan = (x - CenterX) << 1;
    if (pan < -4000) pan = -4000;
    if (pan > 4000) pan = 4000;
    
    PlayEffect(id, pan, 0);
}

int GetSound(char* Name) {
    if (!NSounds) return -1;
    
    for (int i = 0; i < NSounds; i++) {
        if (SoundID[i] && strcmp(SoundID[i], Name) == 0) {
            return i;
        }
    }
    
    return -1;
}

// Sound loading will be handled by the existing LoadSounds function
// but it will use the new CDirSound class internally 