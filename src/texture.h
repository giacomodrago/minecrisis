#ifndef TEXTURE_H
#define TEXTURE_H

#include "opengl.h"

#define TEXTURE_LOAD_ERROR -1

typedef GLuint texture_t;

texture_t texture_load_TGA(const char* filename, GLfloat minFilter, GLfloat maxFilter);
texture_t texture_load_PNG_buf(const unsigned char* buffer, int buf_len, GLfloat minFilter, GLfloat maxFilter);
texture_t texture_load_PNG(const char* filename, GLfloat minFilter, GLfloat maxFilter);

#endif
