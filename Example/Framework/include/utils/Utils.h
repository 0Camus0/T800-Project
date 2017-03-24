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

#ifndef UAD_UTILS_GL_H
#define UAD_UTILS_GL_H

#include <Config.h>

#include <stdio.h>

#ifdef USING_OPENGL_ES
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

void checkcompilederrors(GLuint shader, GLenum type);
GLuint createShader(GLenum type, char* pSource);
#endif

char *file2string(const char *path);


#endif