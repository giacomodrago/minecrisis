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

#ifndef SOUND_H
#define SOUND_H

#include <float.h>
#include "geom.h"
#include "openal.h"

#define BUFFER_LOAD_ERROR AL_NONE
#define SOUND_MAX_DISTANCE FLT_MAX

typedef ALuint sound_buffer_t;
typedef ALuint sound_source_t;

void sound_init();
void sound_cleanup();
sound_buffer_t sound_create_buffer_wav(const unsigned char* data_buffer, int data_buffer_len);
void sound_destroy_buffer(sound_buffer_t buffer);
sound_source_t sound_create_source(point_t position, float max_distance);
sound_source_t sound_create_source_unpos();
void sound_set_source_position(sound_source_t source, point_t position);
void sound_destroy_source(sound_source_t source);
void sound_set_listener(point_t position, vector_t vector_look, vector_t vector_up);
void sound_play(sound_buffer_t buffer, sound_source_t source, float gain, bool loop);
void sound_pause(sound_source_t source);
void sound_resume(sound_source_t source);
void sound_stop(sound_buffer_t buffer);

#endif
