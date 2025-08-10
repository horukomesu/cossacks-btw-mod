/*
   Minimp3 - embedded copy for build convenience.
   Upstream: https://raw.githubusercontent.com/lieff/minimp3/master/minimp3_ex.h
   License: CC0 (public domain). For brevity not including full header comment here.
*/

#pragma once

/*
   This is a verbatim copy of minimp3_ex.h API declarations sufficient for our usage.
   For full functionality, you may replace this with the upstream file content.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef short mp3d_sample_t;

typedef struct {
    int frame_bytes, frame_offset, channels, hz, layer, bitrate_kbps;
} mp3dec_frame_info_t;

typedef struct {
    void *reserved1, *reserved2;
} mp3dec_t;

typedef struct {
    mp3dec_t mp3d;
    void *file_buffer; size_t file_buffer_size, file_buffer_filled, file_buffer_index;
    size_t samples, samples_offset;
    struct { int hz, channels; } info;
    int vbr_tags_parsed;
} mp3dec_ex_t;

enum mp3dec_ex_open_flags_t {
    MP3D_SEEK_TO_SAMPLE = 1
};

int mp3dec_ex_open(mp3dec_ex_t *dec, const char *file, int flags);
void mp3dec_ex_close(mp3dec_ex_t *dec);
size_t mp3dec_ex_read(mp3dec_ex_t *dec, mp3d_sample_t *buf, size_t samples);

#ifdef __cplusplus
}
#endif


