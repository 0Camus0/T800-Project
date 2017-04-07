/*********************************************************
* Copyright (C) 2017 Daniel Enriquez (camus_mm@hotmail.com)
* All Rights Reserved
*
* You may use, distribute and modify this code under the
* following terms:
* ** Do not claim that you wrote this software
* ** A mention would be appreciated but not needed
* ** I do not and will not provide support, this software is "as is"
* ** Enjoy, learn and share.
*********************************************************/

#ifndef T800_UTILS_GL_H
#define T800_UTILS_GL_H

#include <Config.h>

#include <stdio.h>

#if defined(USING_OPENGL_ES)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else defined(USING_OPENGL)
#include <GL/glew.h>
#endif

#if defined(USING_OPENGL_ES)||defined(USING_OPENGL)
void checkcompilederrors(GLuint shader, GLenum type);
GLuint createShader(GLenum type, char* pSource);
#else
void checkcompilederrors(unsigned int shader, unsigned int type);
unsigned int createShader(unsigned int type, char* pSource);
#endif

char *file2string(const char *path);


#endif