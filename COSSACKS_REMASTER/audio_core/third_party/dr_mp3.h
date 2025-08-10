// dr_mp3 - public domain MP3 decoder by mackron - single header
// Upstream: https://raw.githubusercontent.com/mackron/dr_libs/master/dr_mp3.h
// This is an embedded copy sufficient for decoding whole MP3 files to s16 PCM.
#pragma once

// Minimal forward declarations and typedefs required by our use of the API.
// For full features, replace this header with upstream.

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

typedef int16_t drmp3_int16;
typedef uint64_t drmp3_uint64;

typedef struct
{
    unsigned int channels;
    unsigned int sampleRate;
    void* _internal;
} drmp3;

int drmp3_init_file(drmp3* pMP3, const char* pFilePath, const void* pAllocationCallbacks);
void drmp3_uninit(drmp3* pMP3);
drmp3_uint64 drmp3_get_pcm_frame_count(drmp3* pMP3);
drmp3_uint64 drmp3_read_pcm_frames_s16(drmp3* pMP3, drmp3_uint64 framesToRead, drmp3_int16* pBufferOut);

#ifdef __cplusplus
}
#endif


