///////////////////////////////////////////////////////////
// CWAVE.CPP: Cross-platform implementation file for the WAVE class.
///////////////////////////////////////////////////////////

#include "cwave.h"
#include "../cross_platform/platform_compat.h"
#include <stdio.h>
#include <string.h>

// Wave file chunk structures
struct WaveChunk
{
    char chunkID[4];
    uint32_t chunkSize;
};

struct WaveFormatChunk
{
    char chunkID[4];      // "fmt "
    uint32_t chunkSize;   // 16 for PCM
    uint16_t audioFormat; // 1 for PCM
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
};

struct WaveDataChunk
{
    char chunkID[4];      // "data"
    uint32_t chunkSize;   // Size of the data section
};

///////////////////////////////////////////////////////////
// CWave::CWave()
///////////////////////////////////////////////////////////
CWave::CWave(char* fileName)
{
    // Initialize the class's members.
    m_waveSize = 0;
    m_waveOK = FALSE;
    m_pWave = NULL;
    memset(&m_waveFormatEx, 0, sizeof(m_waveFormatEx));

    // Load the wave file.
    m_waveOK = LoadWaveFile(fileName);
}

///////////////////////////////////////////////////////////
// CWave::~CWave()
///////////////////////////////////////////////////////////
CWave::~CWave()
{
    // Free the memory assigned to the wave data.
    if (m_pWave) {
        free(m_pWave);
        m_pWave = NULL;
    }
}

///////////////////////////////////////////////////////////
// CWave::LoadWaveFile()
//
// This function loads a wave from disk into memory using
// cross-platform file operations.
///////////////////////////////////////////////////////////
BOOL CWave::LoadWaveFile(char* fileName)
{
    FILE* file = fopen(fileName, "rb");
    if (!file) {
        return FALSE;
    }

    // Read RIFF header
    WaveChunk riffChunk;
    if (fread(&riffChunk, sizeof(WaveChunk), 1, file) != 1) {
        fclose(file);
        return FALSE;
    }

    // Verify RIFF signature
    if (strncmp(riffChunk.chunkID, "RIFF", 4) != 0) {
        fclose(file);
        return FALSE;
    }

    // Read WAVE signature
    char waveSignature[4];
    if (fread(waveSignature, 4, 1, file) != 1) {
        fclose(file);
        return FALSE;
    }

    if (strncmp(waveSignature, "WAVE", 4) != 0) {
        fclose(file);
        return FALSE;
    }

    // Find format chunk
    WaveFormatChunk formatChunk;
    bool foundFormat = false;
    bool foundData = false;
    WaveDataChunk dataChunk;

    while (!foundFormat || !foundData) {
        WaveChunk chunk;
        if (fread(&chunk, sizeof(WaveChunk), 1, file) != 1) {
            fclose(file);
            return FALSE;
        }

        if (strncmp(chunk.chunkID, "fmt ", 4) == 0) {
            // Read format chunk
            if (fread(&formatChunk.audioFormat, chunk.chunkSize, 1, file) != 1) {
                fclose(file);
                return FALSE;
            }
            
            // Copy to our format structure
            m_waveFormatEx.wFormatTag = formatChunk.audioFormat;
            m_waveFormatEx.nChannels = formatChunk.numChannels;
            m_waveFormatEx.nSamplesPerSec = formatChunk.sampleRate;
            m_waveFormatEx.nAvgBytesPerSec = formatChunk.byteRate;
            m_waveFormatEx.nBlockAlign = formatChunk.blockAlign;
            m_waveFormatEx.wBitsPerSample = formatChunk.bitsPerSample;
            m_waveFormatEx.cbSize = 0;
            
            foundFormat = true;
        }
        else if (strncmp(chunk.chunkID, "data", 4) == 0) {
            // Found data chunk
            m_waveSize = chunk.chunkSize;
            
            // Allocate buffer for wave data
            m_pWave = (char*)malloc(m_waveSize);
            if (!m_pWave) {
                fclose(file);
                return FALSE;
            }
            
            // Read wave data
            if (fread(m_pWave, m_waveSize, 1, file) != 1) {
                free(m_pWave);
                m_pWave = NULL;
                fclose(file);
                return FALSE;
            }
            
            foundData = true;
        }
        else {
            // Skip unknown chunk
            fseek(file, chunk.chunkSize, SEEK_CUR);
        }
    }

    fclose(file);
    return foundFormat && foundData;
}

///////////////////////////////////////////////////////////
// CWave::GetWaveSize()
//
// This returns the size in bytes of the wave data.
///////////////////////////////////////////////////////////
uint32_t CWave::GetWaveSize()
{
    return m_waveSize;
}

///////////////////////////////////////////////////////////
// CWave::GetWaveFormatPtr()
//
// This function returns a pointer to the wave file's
// WAVEFORMATEX structure.
///////////////////////////////////////////////////////////
WAVEFORMATEX_COMPAT* CWave::GetWaveFormatPtr()
{
    return &m_waveFormatEx;
}

///////////////////////////////////////////////////////////
// CWave::GetWaveDataPtr()
//
// This function returns a pointer to the wave's
// actual sound data.
///////////////////////////////////////////////////////////
char* CWave::GetWaveDataPtr()
{
    return m_pWave;
}

///////////////////////////////////////////////////////////
// CWave::WaveOK()
//
// This function returns a Boolean value indicating whether
// the wave file object was set up successfully.
///////////////////////////////////////////////////////////
BOOL CWave::WaveOK()
{
    return m_waveOK;
}
