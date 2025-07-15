#pragma once

#include "raylib_audio.h"
#include "../cross_platform/platform_compat.h"
#include "../Main executable/Cwave.h"

// Forward declarations
class CWave;
struct WAVEFORMATEX_COMPAT;

// DirectSound compatibility constants
#define MAXSND 600
#define MAXSND1 601
#define MaxSnd 1024

// DirectSound return codes
#define DS_OK 0
#define DSERR_ALLOCATED 1
#define DSERR_BADFORMAT 2
#define DSERR_INVALIDPARAM 3
#define DSERR_NOAGGREGATION 4
#define DSERR_OUTOFMEMORY 5
#define DSERR_UNINITIALIZED 6
#define DSERR_UNSUPPORTED 7

// DirectSound cooperation levels
#define DSSCL_EXCLUSIVE 1
#define DSSCL_NORMAL 2

// DirectSound buffer capabilities
#define DSBCAPS_PRIMARYBUFFER 1

// DirectSound buffer description
struct DSBUFFERDESC
{
    uint32_t dwSize;
    uint32_t dwFlags;
    uint32_t dwBufferBytes;
    void* lpwfxFormat;
};

// Wave format structure
struct WAVEFORMATEX
{
    uint16_t wFormatTag;
    uint16_t nChannels;
    uint32_t nSamplesPerSec;
    uint32_t nAvgBytesPerSec;
    uint16_t nBlockAlign;
    uint16_t wBitsPerSample;
    uint16_t cbSize;
};

// DirectSound compatibility layer using raylib
class CDirSound
{
protected:
    RaylibAudio* m_pAudioSystem;
    HWND m_hWindow;
    bool m_initialized;
    
    // Sound buffer management
    std::vector<bool> m_bufferActive;
    std::vector<std::string> m_soundNames;
    std::vector<float> m_bufferVolumes;
    std::vector<float> m_bufferPans;
    uint32_t m_bufferSizes[MAXSND1];

public:
    // DirectSound compatibility members
    short Volume[MAXSND1];
    short SrcX[MAXSND1];
    short SrcY[MAXSND1];
    byte  BufIsRun[MAXSND1];
    uint32_t m_currentBufferNum;
    
    // Constructor/Destructor
    CDirSound();
    ~CDirSound();
    
    // Initialization
    void CreateDirSound(HWND hWnd);
    
    // Buffer management
    uint32_t CreateSoundBuffer(CWave* pWave);
    uint32_t DuplicateSoundBuffer(uint32_t bufferNum);
    BOOL CopyWaveToBuffer(CWave* pWave, uint32_t bufferNum);
    
    // Sound control
    BOOL PlaySound(uint32_t bufferNum);
    BOOL StopSound(uint32_t bufferNum);
    BOOL PlayCoorSound(uint32_t bufferNum, int x, int vx);
    
    // Audio settings
    void SetVolume(uint32_t bufferNum, int vol);
    void SetPan(uint32_t bufferNum, int pan);
    void SetLastVolume(short Vol);
    
    // Status
    BOOL DirectSoundOK();
    bool IsPlaying(uint32_t bufferNum);
    int GetPos(uint32_t bufferNum);
    
    // Processing
    void ProcessSoundSystem();
    void ControlPan(uint32_t bufferNum);
    
protected:
    void ReleaseAll();
    std::string GetSoundName(uint32_t bufferNum);
    float ConvertVolume(int directSoundVolume);
    float ConvertPan(int directSoundPan);
};

// Global sound system instance
extern CDirSound CDIRSND;
extern CDirSound* CDS;

// Sound loading system compatibility
extern char* SoundID[MaxSnd];
extern word SndTable[MaxSnd][16];
extern byte SnDanger[MaxSnd];
extern word NSnd[MaxSnd];
extern word NSounds;

// Sound effect functions
void LoadSounds(char* fn);
void PlayEffect(int n, int pan, int vol);
void PlayCoorEffect(int n, int x, int y, int pan, int vol);
void PlaySingleEffect(int n, int pan, int vol);
void AddEffect(int x, int y, int id);
void AddSingleEffect(int x, int y, int id);
void AddUnlimitedEffect(int x, int y, int id);
void AddEffectV(int x, int y, int vx, int id);
void PrepareSound();
int GetSound(char* Name); 