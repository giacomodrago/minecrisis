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

#include "sound.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

void sound_init() {

    ALCdevice* pDevice;
    ALCcontext* pContext;

    // Get handle to device.
    pDevice = alcOpenDevice(NULL);

    // Create audio context.
    pContext = alcCreateContext(pDevice, NULL);

    // Set active context.
    alcMakeContextCurrent(pContext);

    // Check for an error.
    if (alcGetError(pDevice) != ALC_NO_ERROR)
        fprintf(stderr, "Warning: cannot initialize sound environment.");

    alDistanceModel(AL_LINEAR_DISTANCE);

}

void sound_cleanup() {

    ALCcontext* pCurContext;
    ALCdevice* pCurDevice;

    // Get the current context.
    pCurContext = alcGetCurrentContext();

    // Get the device used by that context.
    pCurDevice = alcGetContextsDevice(pCurContext);

    // Reset the current context to NULL.
    alcMakeContextCurrent(NULL);

    // Release the context and the device.
    alcDestroyContext(pCurContext);
    alcCloseDevice(pCurDevice);

}

/* Can ONLY load PCM signed 16 bit mono 44100hz (little-endian) */
#define DATA_OFFSET 0x2C
sound_buffer_t sound_create_buffer_wav(const unsigned char* data_buffer, int data_buffer_len) {

    sound_buffer_t buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, AL_FORMAT_MONO16, data_buffer+DATA_OFFSET,data_buffer_len-DATA_OFFSET, 44100);

    ALenum error = alGetError();
    if (error != AL_NONE) buffer = BUFFER_LOAD_ERROR;

    return buffer;

}


void sound_destroy_buffer(sound_buffer_t buffer) {
    alDeleteBuffers(1, &buffer);
}

static sound_source_t _create_source(bool positional, point_t position, float max_distance) {
    sound_source_t source;
    alGenSources(1, &source);
    if (positional) {
        const float position_v[] = {position.x, position.y, position.z};
        alSourcefv(source, AL_POSITION, position_v);
        alSourcei(source, AL_SOURCE_RELATIVE, false);
        alSourcei(source, AL_REFERENCE_DISTANCE, 1.0);
        alSourcef(source, AL_MAX_DISTANCE, max_distance);
    } else {
        static const float origin[] = {0.0, 0.0, 0.0};
        alSourcefv(source, AL_POSITION, origin);
        alSourcei(source, AL_SOURCE_RELATIVE, true);
    }
    return source;
}

sound_source_t sound_create_source(point_t position, float max_distance) {
    return _create_source(true, position, max_distance);
}

sound_source_t sound_create_source_unpos() {
    return _create_source(false, ORIGIN, 0.0);
}


void sound_set_source_position(sound_source_t source, point_t position) {
    const float position_v[] = {position.x, position.y, position.z};
    alSourcefv(source, AL_POSITION, position_v);
}

void sound_destroy_source(sound_source_t source) {
    sound_stop(source);
    alDeleteSources(1, &source);
}

void sound_set_listener(point_t position, vector_t vector_look, vector_t vector_up) {
    const float position_v[] = {position.x, position.y, position.z};
    const float orientation_v[] =
        {vector_look.x, vector_look.y, vector_look.z,
        vector_up.x, vector_up.y, vector_up.z};
    alListenerfv(AL_POSITION, position_v);
    alListenerfv(AL_ORIENTATION, orientation_v);
}

void sound_play(sound_buffer_t buffer, sound_source_t source, float gain, bool loop) {
    alSourcei(source, AL_BUFFER, buffer);
    alSourcef(source, AL_GAIN, gain);
    alSourcei(source, AL_LOOPING, loop);
    alSourcePlay(source);
}

void sound_pause(sound_source_t source) {
    alSourcePause(source);
}

void sound_resume(sound_source_t source) {
    alSourcePlay(source);
}

void sound_stop(sound_source_t source) {
    alSourceStop(source);
}
