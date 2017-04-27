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

#ifndef T800_QUAD_GL_H
#define T800_QUAD_GL_H

#include <Config.h>
#ifdef USING_OPENGL_ES20
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <video/GLTexture.h>
#elif defined(USING_OPENGL_ES30)
#include <GLES3/gl3.h>
#include <video/GLTexture.h>
#elif defined(USING_OPENGL_ES31)
#include <GLES3/gl31.h>
#include <video/GLTexture.h>
#elif defined(USING_OPENGL)
#include <GL/glew.h>
#include <video/GLTexture.h>
#endif

#include <video/BaseDriver.h>
#include <utils/xMaths.h>
#include <scene/PrimitiveBase.h>

class GLQuad : public PrimitiveBase{
public:
	struct Vert {
		float x,y,z,w;
		float u,v;
	};

	GLQuad(){}
	void Create();
	void Create(char *){}
	void Transform(float *t);
	void Draw(float *t, float *vp);
	void Destroy();

	unsigned int	SigBase;

	Vert			vertices[4];
	unsigned short	indices[6];
	unsigned int	VB;
	unsigned int	IB;
	XMATRIX44		transform;
	XVECTOR3		LightPositions[128];
	XVECTOR3		LightColors[128];

};

#endif
