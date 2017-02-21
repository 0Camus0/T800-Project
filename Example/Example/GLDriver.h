#ifndef UAD_GLDRIVER_H
#define UAD_GLDRIVER_H

#include "BaseDriver.h"

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

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

	void	Clear();
	void	SwapBuffers();

	EGLDisplay			eglDisplay;
	EGLConfig			eglConfig;
	EGLSurface			eglSurface;
	EGLContext			eglContext;

	EGLNativeWindowType	eglWindow;

};

#endif