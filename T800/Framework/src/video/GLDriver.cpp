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

#include <video\GLDriver.h>
#include <video\GLRT.h>
#include <video\GLShader.h>
#include <iostream>
#include <string>

#if defined(USING_OPENGL_ES)
#pragma comment(lib,"libEGL.lib")
#pragma comment(lib,"libGLESv2.lib")
#else defined(USING_OPENGL)
#pragma comment(lib,"glew.lib")
#pragma comment(lib,"OpenGL32.Lib")
#endif

#if defined(USING_OPENGL_ES)
void EGLError(const char* c_ptr) {

	EGLint iErr = eglGetError();
	if (iErr != EGL_SUCCESS) {
		std::cout << "EGL CALL: " << c_ptr << " Error Code: " << iErr << std::endl;
	}

}
bool OpenNativeDisplay(EGLNativeDisplayType* nativedisp_out)
{
	*nativedisp_out = (EGLNativeDisplayType)NULL;
	return true;
}
#endif
void	GLDriver::InitDriver() {
#if defined(USING_OPENGL_ES)
	EGLint numConfigs;

	EGLNativeDisplayType nativeDisplay;

	if(!OpenNativeDisplay(&nativeDisplay)){
		std::cout << "can't open native display" << std::endl;
	}

	eglDisplay = eglGetDisplay(nativeDisplay);

	EGLError("eglGetDisplay");

	EGLint iMajorVersion, iMinorVersion;

	if (!eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion)) {
		std::cout << "Failed to initialize egl" << std::endl;
	}

	eglBindAPI(EGL_OPENGL_ES_API);

	EGLError("eglBindAPI");

	const EGLint attribs[] = {
		EGL_SURFACE_TYPE,	EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
		EGL_BLUE_SIZE,		8,
		EGL_GREEN_SIZE,		8,
		EGL_RED_SIZE,		8,
		EGL_DEPTH_SIZE,		24,
		EGL_NONE
	};

	eglChooseConfig(eglDisplay, attribs, &eglConfig, 1, &numConfigs);

	EGLError("eglBindAPI");

	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);

	EGLError("eglCreateWindowSurface");

	EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, ai32ContextAttribs);

	EGLError("eglCreateContext");

	if (eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) == EGL_FALSE) {
		std::cout << "Failed to make current" << std::endl;
		return;
	}

	eglQuerySurface(eglDisplay, eglSurface, EGL_WIDTH, &width);
	eglQuerySurface(eglDisplay, eglSurface, EGL_HEIGHT, &height);
#else defined(USING_OPENGL)
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		printf("Error: %s\n", glewGetErrorString(err));
	}
	else {
		printf("GLEW OK\n");
	}
	SDL_Surface *sur = SDL_GetVideoSurface();	
	width = sur->w;
	height = sur->h;
#endif
	 
	
	std::string GL_Version = std::string((const char*)glGetString(GL_VERSION));
	std::string GL_Extensions = std::string((const char*)glGetString(GL_EXTENSIONS));
	std::cout << "GL Version: " << GL_Version << "\n\nExtensions\n\n" << GL_Extensions << std::endl;

	glEnable(GL_DEPTH_TEST);
	glClearDepthf(1.0f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &CurrentFBO);
}

void	GLDriver::CreateSurfaces() {

}

void	GLDriver::DestroySurfaces() {

}

void	GLDriver::Update() {

}

void	GLDriver::DestroyDriver() {
#if defined(USING_OPENGL_ES)
	eglDestroySurface(eglDisplay, eglSurface);
	eglDestroyContext(eglDisplay, eglContext);
	eglTerminate(eglDisplay);
#endif
}

void	GLDriver::SetWindow(void *window) {
#if defined(USING_OPENGL_ES)
	eglWindow = GetActiveWindow();
#endif
}

void	GLDriver::SetDimensions(int w, int h){

}

void	GLDriver::Clear() {
	glClearColor(0.5,0.5,0.5,1.0);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

}

void	GLDriver::SwapBuffers() {
#if defined(USING_OPENGL_ES)
	eglSwapBuffers(eglDisplay, eglSurface);
#elif defined(USING_OPENGL)
	SDL_GL_SwapBuffers();
#endif
}

int  GLDriver::CreateRT(int nrt, int cf, int df, int w, int h) {
	GLES20RT	*pRT = new GLES20RT;
	if (w == 0)
		w = width;
	if (h == 0)
		h = height;
	if (pRT->LoadRT(nrt, cf, df, w, h)) {
		RTs.push_back(pRT);
		return (RTs.size() - 1);
	}
	else {
		delete pRT;
	}
	return -1;
}

void GLDriver::PushRT(int id) {
	if (id < 0 || id >= (int)RTs.size())
		return;

	GLES20RT *pRT = dynamic_cast<GLES20RT*>(RTs[id]);

	glBindFramebuffer(GL_FRAMEBUFFER, pRT->vFrameBuffers[0]);

	glClearColor(0.5, 0.5, 0.5, 1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLDriver::PopRT() {
	glBindFramebuffer(GL_FRAMEBUFFER, CurrentFBO);
}

void GLDriver::DestroyRTs() {
	for (unsigned int i = 0; i < RTs.size(); i++) {
		GLES20RT *pRT = dynamic_cast<GLES20RT*>(RTs[i]);
		delete pRT;
	}
}

int GLDriver::CreateShader(std::string src_vs, std::string src_fs, unsigned int sig) {
	for (unsigned int i = 0; i < Shaders.size(); i++) {
		if (Shaders[i]->Sig == sig) {
			return i;
		}
	}

	GLShader* shader = new GLShader();
	if (shader->CreateShader(src_vs, src_fs, sig)) {
		Shaders.push_back(shader);
		return (Shaders.size() - 1);
	}
	else {
		delete shader;
	}
	return -1;
}

ShaderBase*	GLDriver::GetShader(unsigned int sig) {
	for (unsigned int i = 0; i < Shaders.size(); i++) {
		if (Shaders[i]->Sig == sig) {
			return Shaders[i];
		}
	}
	return 0;
}

void GLDriver::DestroyShaders() {
	for (unsigned int i = 0; i < Shaders.size(); i++) {
		GLShader *pShader = dynamic_cast<GLShader*>(Shaders[i]);
		delete pShader;
	}
}