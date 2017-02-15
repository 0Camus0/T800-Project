#ifndef UAD_CUBE_GL_H
#define UAD_CUBE_GL_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <xMaths.h>

#include "PrimitiveBase.h"
#include "UtilsGL.h"



struct CVertex {

	float x, y, z,w;
	float nx,ny,nz,nw;
	float s,t;
};

class CubeGL : public PrimitiveBase {
public:
	CubeGL() : shaderID(0) {}
	void Create();
	void Create(char *) {}
	void Transform(float *t);
	void Draw(float *t,float *vp);
	void Destroy();

	GLuint	shaderID;
	GLint	vertexAttribLoc;
	GLint	normalAttribLoc;
	GLint	uvAttribLoc;

	GLint  matWorldViewProjUniformLoc;
	GLint  matWorldUniformLoc;

	CVertex			vertices[24];
	unsigned short	indices[36];
	GLuint			VB;
	GLuint			IB;

	XMATRIX44	transform;
};


#endif