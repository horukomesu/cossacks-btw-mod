///////////////////////////////////////////////////////////
// CWAVE.H: Header file for the cross-platform WAVE class.
///////////////////////////////////////////////////////////

#ifndef __CWAVE_H
#define __CWAVE_H

#include "../cross_platform/platform_compat.h"

// Cross-platform wave format structure
struct WAVEFORMATEX_COMPAT
{
    uint16_t wFormatTag;
    uint16_t nChannels;
    uint32_t nSamplesPerSec;
    uint32_t nAvgBytesPerSec;
    uint16_t nBlockAlign;
    uint16_t wBitsPerSample;
    uint16_t cbSize;
};

class CWave
{
protected:
    uint32_t m_waveSize;
    BOOL m_waveOK;
    char* m_pWave;
    WAVEFORMATEX_COMPAT m_waveFormatEx;

public:
    CWave(char* fileName);
    ~CWave();

    uint32_t GetWaveSize();
    WAVEFORMATEX_COMPAT* GetWaveFormatPtr();
    char* GetWaveDataPtr();
    BOOL WaveOK();

protected:
    BOOL LoadWaveFile(char* fileName);
};

#endif
