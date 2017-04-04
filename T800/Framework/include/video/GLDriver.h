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

#ifndef T800_GLDRIVER_H
#define T800_GLDRIVER_H

#include <Config.h>

#include <video\BaseDriver.h>
#if defined(USING_OPENGL_ES)
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#pragma comment(lib,"libEGL.lib")
#pragma comment(lib,"libGLESv2.lib")
#else defined(USING_OPENGL)
#include <GL/glew.h>
#include <SDL/SDL.h>
#pragma comment(lib,"glew.lib")
#pragma comment(lib,"OpenGL32.Lib")
#endif

class GLDriver : public BaseDriver {
public:
	GLDriver() {  }
	void	InitDriver();
	void	CreateSurfaces();
	void	DestroySurfaces();
	void	Update();
	void	DestroyDriver();
	void	SetWindow(void *window) ;
	void	SetDimensions(int, int);

	int 	CreateRT(int nrt, int cf, int df, int w, int h);
	void	PushRT(int id);
	void	PopRT();
	void	DestroyRTs();

	void	Clear();
	void	SwapBuffers();
#if defined(USING_OPENGL_ES)
	EGLDisplay			eglDisplay;
	EGLConfig			eglConfig;
	EGLSurface			eglSurface;
	EGLContext			eglContext;

	EGLNativeWindowType	eglWindow;
#endif
};

#endif