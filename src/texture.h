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

#ifndef TEXTURE_H
#define TEXTURE_H

#include "opengl.h"

#define TEXTURE_LOAD_ERROR -1

typedef GLuint texture_t;

texture_t texture_load_TGA(const char* filename, GLfloat minFilter, GLfloat maxFilter);
texture_t texture_load_PNG_buf(const unsigned char* buffer, int buf_len, GLfloat minFilter, GLfloat maxFilter);
texture_t texture_load_PNG(const char* filename, GLfloat minFilter, GLfloat maxFilter);

#endif
