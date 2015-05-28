#pragma once


#ifdef WIN32
  #include <windows.h>      // must be included before GL
#endif // WIN32

#define GL_V_MAJOR 2        // OpenGL 2.0 required for GLEW
#define GL_V_MINOR 0

#include <GL/gl.h>          // PC uses OpenGL rather than OpenGL ES

