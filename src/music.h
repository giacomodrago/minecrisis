/*
 * MineCrisis
 * Copyright (C) 2010-2011 Giacomo Drago
 * http://giacomodrago.com/go/minecrisis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

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
