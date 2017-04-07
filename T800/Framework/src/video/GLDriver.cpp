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
#include <video\TextureGL.h>
#include <video\GLRT.h>
#include <video\GLShader.h>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <fstream>

#if defined(USING_OPENGL_ES20)
#pragma comment(lib,"libEGL.lib")
#pragma comment(lib,"libGLESv2.lib")
#elif defined (USING_OPENGL_ES30)
#pragma comment(lib,"libEGL.lib")
#pragma comment(lib,"libGLESv2.lib")
#elif defined(USING_OPENGL)
#pragma comment(lib,"glew.lib")
#pragma comment(lib,"OpenGL32.Lib")
#endif

#if defined(USING_OPENGL_ES20) || defined(USING_OPENGL_ES30)
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
#if defined(USING_OPENGL_ES20) || defined(USING_OPENGL_ES30)
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
	}else{
		std::cout << "EGL version " << iMajorVersion << "." << iMinorVersion << std::endl;
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

	if(!eglChooseConfig(eglDisplay, attribs, &eglConfig, 1, &numConfigs)){
		std::cout << "Failed to choose config" << std::endl;
	}

	EGLError("eglChooseConfig");

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
#elif defined(USING_OPENGL)
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

	std::istringstream iss(GL_Extensions);
	std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss},
		std::istream_iterator<std::string>{} };

	ExtensionsTok = tokens;
	Extensions = GL_Extensions;

	std::cout << "GL Version: " << GL_Version << "\n\nExtensions\n\n";

	for (unsigned int i = 0; i < ExtensionsTok.size(); i++) {
		printf("[%s]\n", ExtensionsTok[i].c_str());
	}
	

	glEnable(GL_DEPTH_TEST);
	glClearDepthf(1.0f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &CurrentFBO);
#if defined(USING_OPENGL) || defined(USING_OPENGL_ES30)
	for (int i = 0; i < 16; i++) {
		DrawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}
#endif
}

void	GLDriver::CreateSurfaces() {

}

void	GLDriver::DestroySurfaces() {

}

void	GLDriver::Update() {

}

void	GLDriver::DestroyDriver() {
#if defined(USING_OPENGL_ES20) || defined(USING_OPENGL_ES30)
	eglDestroySurface(eglDisplay, eglSurface);
	eglDestroyContext(eglDisplay, eglContext);
	eglTerminate(eglDisplay);
#endif
}

void	GLDriver::SetWindow(void *window) {
#if defined(USING_OPENGL_ES20) || defined(USING_OPENGL_ES30)
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
#if defined(USING_OPENGL_ES20) || defined(USING_OPENGL_ES30)
	eglSwapBuffers(eglDisplay, eglSurface);
#elif defined(USING_OPENGL)
	SDL_GL_SwapBuffers();
#endif
}

bool GLDriver::CheckExtension(std::string s){
	return (Extensions.find(s) != std::string::npos);
}

int GLDriver::CreateTexture(std::string path) {
	TextureGL *pTex = new TextureGL;
	if (pTex->LoadTexture((char*)path.c_str())) {
		Textures.push_back(pTex);
		return (Textures.size() - 1);
	}
	else {
		delete pTex;
	}
	return -1;
}

Texture* GLDriver::GetTexture(int id) {
	if (id < 0 || id >= (int)Textures.size()) {
		printf("Warning null ptr Textures Idx\n");
		return 0;
	}

	return Textures[id];
}

void GLDriver::DestroyTexture() {
	for (unsigned int i = 0; i < Textures.size(); i++) {
		TextureGL *pTex = dynamic_cast<TextureGL*>(Textures[i]);
		delete pTex;
	}
}

int  GLDriver::CreateRT(int nrt, int cf, int df, int w, int h) {
	GLRT	*pRT = new GLRT;
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

	GLRT *pRT = dynamic_cast<GLRT*>(RTs[id]);

	glBindFramebuffer(GL_FRAMEBUFFER, pRT->vFrameBuffers[0]);

#if defined(USING_OPENGL) || defined(USING_OPENGL_ES30)
	glDrawBuffers(pRT->number_RT, DrawBuffers);
#endif

	glClearColor(0.5, 0.5, 0.5, 1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLDriver::PopRT() {
	glBindFramebuffer(GL_FRAMEBUFFER, CurrentFBO);
}

void GLDriver::DestroyRTs() {
	for (unsigned int i = 0; i < RTs.size(); i++) {
		GLRT *pRT = dynamic_cast<GLRT*>(RTs[i]);
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

ShaderBase*	GLDriver::GetShaderSig(unsigned int sig) {
	for (unsigned int i = 0; i < Shaders.size(); i++) {
		if (Shaders[i]->Sig == sig) {
			return Shaders[i];
		}
	}
	return 0;
}

ShaderBase*	GLDriver::GetShaderIdx(int id) {
	if (id < 0 || id >= (int)Shaders.size()) {
		printf("Warning null ptr ShaderBase Idx\n");
		return 0;
	}

	return Shaders[id];
}

void GLDriver::DestroyShaders() {
	for (unsigned int i = 0; i < Shaders.size(); i++) {
		GLShader *pShader = dynamic_cast<GLShader*>(Shaders[i]);
		delete pShader;
	}
}