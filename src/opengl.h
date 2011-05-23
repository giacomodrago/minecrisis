#ifndef OPENGL_H
#define OPENGL_H

#ifndef __APPLE__
    #include <GL/gl.h>
    #include <GL/glut.h>
#else
    #include <OpenGL/OpenGL.h>
    #include <GLUT/glut.h>
#endif

typedef GLuint display_list_t;

#endif
