#ifndef MUSIC_H
#define MUSIC_H

#include <pthread.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>
#include "utils.h"

#define OGG_READ_BLOCK_SIZE 8192
#define NUM_BUFFERS 8

typedef struct {
    OggVorbis_File oggStream;
    vorbis_info* vorbisInfo;
    data_stream_t data_stream;
    ALenum format;
    float gain;
    pthread_t thread;
    sound_buffer_t buffers[NUM_BUFFERS];
    sound_source_t source;
    bool error;
    volatile bool stopped;
} music_context_t;

bool music_create_context_ogg(music_context_t* context, const unsigned char* data, int data_len, float gain);
void music_start(music_context_t* context);
void music_pause(music_context_t* context);
void music_resume(music_context_t* context);
void music_stop(music_context_t* context);

#endif
