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

#ifndef T800_GLSHADER_H
#define T800_GLSHADER_H

#include <Config.h>

#include <video/BaseDriver.h>

#if defined(USING_OPENGL_ES)
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else defined(USING_OPENGL)
#include <GL/glew.h>
#include <SDL/SDL.h>
#endif

#include <utils\Utils.h>


class GLShader : public ShaderBase {
public:
	GLShader() {
		matWorldViewProjUniformLoc = -1;
		matWorldUniformLoc = -1;

		vertexAttribLoc = -1;
		normalAttribLoc = -1;
		tangentAttribLoc = -1;
		binormalAttribLoc = -1;
		uvAttribLoc = -1;
		uvSecAttribLoc = -1;

		Light0Pos_Loc = -1;
		Light0Color_Loc = -1;
		CameraPos_Loc = -1;
		Ambient_loc = -1;
		DiffuseTex_loc = -1;
		SpecularTex_loc = -1;
		GlossTex_loc = -1;
		NormalTex_loc = -1;
		ReflectTex_loc = -1;
	}

	bool CreateShaderAPI(std::string src_vs, std::string src_fs, unsigned int sig);

	unsigned int ShaderProg;

	int			 matWorldViewProjUniformLoc;
	int			 matWorldUniformLoc;

	int			 vertexAttribLoc;
	int			 normalAttribLoc;
	int			 tangentAttribLoc;
	int			 binormalAttribLoc;
	int			 uvAttribLoc;
	int			 uvSecAttribLoc;

	int			Light0Pos_Loc;
	int			Light0Color_Loc;

	int			CameraPos_Loc;

	int			Ambient_loc;

	int			DiffuseTex_loc;
	int			SpecularTex_loc;
	int			GlossTex_loc;
	int			NormalTex_loc;
	int			ReflectTex_loc;
};


#endif