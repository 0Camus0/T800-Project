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

#if defined(USING_OPENGL_ES20)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#elif defined(USING_OPENGL_ES30)
#include <GLES3/gl3.h>
#elif defined(USING_OPENGL_ES31)
#include <GLES3/gl31.h>
#elif defined(USING_OPENGL)
#include <GL/glew.h>
#else
#include <GL/glew.h>
#include <SDL/SDL.h>
#endif

#include <utils/Utils.h>


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

		tex0_loc = -1;
		tex1_loc = -1;
		tex2_loc = -1;
		tex3_loc = -1;
		tex4_loc = -1;
		tex5_loc = -1;
		tex6_loc = -1;
		tex7_loc = -1;
	}

	virtual ~GLShader() {}

	bool CreateShaderAPI(std::string src_vs, std::string src_fs, unsigned int sig);

	unsigned int ShaderProg;

	int			 matWorldViewProjUniformLoc;
	int			 matWorldUniformLoc;
	int			 matWorldViewUniformLoc;
	int			 matWVPInverseUniformLoc;

	int			 vertexAttribLoc;
	int			 normalAttribLoc;
	int			 tangentAttribLoc;
	int			 binormalAttribLoc;
	int			 uvAttribLoc;
	int			 uvSecAttribLoc;

	int			Light0Pos_Loc;
	int			Light0Color_Loc;

	int			LightPositions_Loc;
	int			LightColors_Loc;

	int			CameraPos_Loc;
	int			CameraInfo_Loc;

	int			Ambient_loc;

	int			DiffuseTex_loc;
	int			SpecularTex_loc;
	int			GlossTex_loc;
	int			NormalTex_loc;
	int			ReflectTex_loc;

	int			tex0_loc;
	int			tex1_loc;
	int			tex2_loc;
	int			tex3_loc;
	int			tex4_loc;
	int			tex5_loc;
	int			tex6_loc;
	int			tex7_loc;
};


#endif
