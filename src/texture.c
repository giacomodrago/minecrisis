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

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <png.h>
#include "texture.h"
#include "utils.h"


texture_t texture_load_TGA(const char* filename, GLfloat minFilter, GLfloat maxFilter) {

    GLubyte		TGAheader[12] = {0,0,2,0,0,0,0,0,0,0,0,0};	// Uncompressed TGA header
    GLubyte		TGAcompare[12];					            // Used to compare TGA header
    GLubyte		header[6];						            // The first six useful bytes from the header
    GLuint		bytesPerPixel;					            // Holds the bpp of the TGA
    GLuint		imageSize;						            // Used to store image size while in RAM
    GLuint		temp;							            // Temp variable
    GLuint		type = GL_RGBA;					            // Set the default OpenGL mode to RBGA (32 BPP)

    FILE* file = fopen(filename, "rb");			            // Open The TGA File

    if (file == NULL ||   // Does File Even Exist?
        fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
        memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0 ||                // Does The Header Match What We Want?
        fread(header,1,sizeof(header),file)!=sizeof(header)) {              // If So Read Next 6 Header Bytes
        if (file == NULL) {						                                // Did The File Even Exist? *Added Jim Strong*
            return TEXTURE_LOAD_ERROR;
        } else {
            fclose(file); // If anything failed, close the file
            return TEXTURE_LOAD_ERROR;
        }
    }

    int width = header[1] * 256 + header[0];		// Determine The TGA Width	(highbyte*256+lowbyte)
    int height = header[3] * 256 + header[2];		// Determine The TGA Height	(highbyte*256+lowbyte)

    if (width <= 0	||							    // Is The Width Less Than Or Equal To Zero
        height <= 0	||							    // Is The Height Less Than Or Equal To Zero
        (header[4] != 24 && header[4] != 32) ) {	// Is The TGA 24 or 32 Bit?
        fclose(file);							// If Anything Failed, Close The File
        return TEXTURE_LOAD_ERROR;
    }

    int bpp	        = header[4];					// Grab The TGA's Bits Per Pixel (24 or 32)
    bytesPerPixel   = bpp/8;						// Divide By 8 To Get The Bytes Per Pixel
    imageSize	    = width*height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

    GLubyte* data = (GLubyte*) malloc(imageSize * sizeof(GLubyte));				// Reserve Memory To Hold The TGA Data

    if (data == NULL ||							        // Does The Storage Memory Exist?
        fread(data, 1, imageSize, file) != imageSize) {	// Does The Image Size Match The Memory Reserved?
        if (data != NULL)						        // Was Image Data Loaded
            free(data);							        // If So, Release The Image Data
        fclose(file);						            // Close The File
        return TEXTURE_LOAD_ERROR;						            // Return False
    }

    for (int i = 0; i < imageSize; i += bytesPerPixel) {	// Loop Through The Image Data
        // Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
        temp	 =data[i];						// Temporarily Store The Value At Image Data 'i'
        data[i]	 = data[i + 2];					// Set The 1st Byte To The Value Of The 3rd Byte
        data[i+2]= temp;						// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
    }

    fclose(file);								// Close the file

    texture_t id;

    // Build A Texture From The Data
    glGenTextures(1, &id);						// Generate OpenGL texture IDs

    glBindTexture(GL_TEXTURE_2D, id);			// Bind the texture to a texture object
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);	// Filtering for if texture is bigger than should be
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter);	// Filtering for if texture is smaller than it should be

    if (bpp == 24) type = GL_RGB;	// Was the TGA 24 bpp?

    glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);

    free(data);

    return id;
}

void _read_png_data(png_structp pngPtr, png_bytep data, png_size_t length) {
    data_stream_t* stream = (data_stream_t*)png_get_io_ptr(pngPtr);
    int read_bytes = length;
    memcpy(data, stream->data+stream->data_pos, read_bytes);
    stream->data_pos += length;
}

texture_t texture_load_PNG_buf(const unsigned char* buffer, int buf_len, GLfloat minFilter, GLfloat maxFilter) {

    //test if png
    int is_png = !png_sig_cmp((png_bytep)buffer, 0, 8);
    if (!is_png) return TEXTURE_LOAD_ERROR;

    //create png struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
        NULL, NULL);
    if (!png_ptr) return TEXTURE_LOAD_ERROR;

    data_stream_t data_stream = {(unsigned char*)buffer+8, buf_len, 0};

    png_set_read_fn(png_ptr, (png_voidp)&data_stream, &_read_png_data);

    //create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return TEXTURE_LOAD_ERROR;
    }

    //create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return TEXTURE_LOAD_ERROR;
    }

    //png error stuff, not sure libpng man suggests this.
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return TEXTURE_LOAD_ERROR;
    }

    //let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    //variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 width, height;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
        NULL, NULL, NULL);

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // Allocate the image_data as a big block, to be given to opengl
    png_byte* image_data = (png_byte*) malloc(rowbytes * height * sizeof(png_byte));
    if (!image_data) {
        //clean up memory and close stuff
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return TEXTURE_LOAD_ERROR;
    }

    //row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep* row_pointers = (png_bytep*) malloc(height * sizeof(png_bytep));
    if (!row_pointers) {
        //clean up memory and close stuff
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        return TEXTURE_LOAD_ERROR;
    }

    // set the individual row_pointers to point at the correct offsets of image_data
    for (int i = 0; i < height; ++i)
        row_pointers[height - 1 - i] = image_data + i * rowbytes;

    texture_t texture = TEXTURE_LOAD_ERROR;

    bool error = false;

    GLenum type;
    int depth = png_get_bit_depth(png_ptr, info_ptr);
    int channels = png_get_channels(png_ptr, info_ptr);
    if (depth == 8) {
        if (channels == 3) type = GL_RGB;
        else if (channels == 4) type = GL_RGBA;
        else error = true;
    } else {
        error = true;
    }

    if (!error) {

        //read the png into image_data through row_pointers
        png_read_image(png_ptr, row_pointers);

        //Now generate the OpenGL texture object
        glGenTextures(1, &texture);

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
            // Filtering for if texture is bigger than should be
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter);
            // Filtering for if texture is smaller than it should be
        glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0,
            type, GL_UNSIGNED_BYTE, (GLubyte*)image_data);

        if (glGetError() != GL_NO_ERROR) texture = TEXTURE_LOAD_ERROR;

    }

    //clean up memory and close stuff
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    free(row_pointers);

    return texture;

}

texture_t texture_load_PNG(const char* filename, GLfloat minFilter, GLfloat maxFilter) {

    unsigned char* buffer;
    int buf_len = readFile(filename, &buffer);
    texture_t result = texture_load_PNG_buf(buffer, buf_len, minFilter, maxFilter);
    free(buffer);
    return result;

}
