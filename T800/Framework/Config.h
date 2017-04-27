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

#ifndef T800_CONFIG_H
#define T800_CONFIG_H

#ifdef __linux__
#ifndef OS_LINUX
#define OS_LINUX
#endif
#else
#define OS_WINDOWS
#endif


#ifndef VDEBUG_NO_LIGHT
#define VDEBUG_NO_LIGHT 0
#endif

#ifndef VDEBUG_SIMPLE_COLOR
#define VDEBUG_SIMPLE_COLOR 0
#endif

#if VDEBUG_SIMPLE_COLOR && VDEBUG_NO_LIGHT
#undef VDEBUG_NO_LIGHT
#define VDEBUG_NO_LIGHT 0
#endif

#define D3DX 1
#define OGLES20 2
#define OGLES30 3
#define OGLES31 4
#define OGL 5

#define DRIVER_SELECTED OGLES30

#if   DRIVER_SELECTED == OGLES20
	#define USING_OPENGL_ES20
#elif DRIVER_SELECTED == OGLES30
	#define USING_OPENGL_ES30
#elif DRIVER_SELECTED == OGLES31
	#define USING_OPENGL_ES31
#elif DRIVER_SELECTED == D3DX
	#define USING_D3D11
#elif DRIVER_SELECTED == OGL
	#define USING_OPENGL
#else
	#define USING_OPENGL // Default GL
#endif

#if defined(OS_LINUX)
#undef USING_D3D11
#undef USING_OPENGL
#define USING_OPENGL_ES30
#endif

#if defined(USING_OPENGL_ES20) || defined(USING_OPENGL_ES30)  || defined(USING_OPENGL_ES31) || defined(USING_OPENGL)
	#define USING_GL_COMMON
#endif


#define SDL 1
#define FREEGLUT 2

#define WINDOW_MANAGER FREEGLUT

#if WINDOW_MANAGER == SDL
#define USING_SDL
#elif WINDOW_MANAGER == FREEGLUT
#define USING_FREEGLUT
#else
#define USING_SDL
#endif

#ifdef OS_WINDOWS
#undef USING_FREEGLUT
#define USING_SDL
#endif



#endif
