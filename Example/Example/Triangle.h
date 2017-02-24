#ifndef UAD_TRIANGLEGL_H
#define UAD_TRIANGLEGL_H

#include "Config.h"

#ifdef USING_OPENGL_ES
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#elif defined(USING_D3D11)
#endif

#include <xMaths.h>

#include "PrimitiveBase.h"
#include "Utils.h"


//#define USE_ARRAY_OF_STRUCTS
#define USE_VBO

#ifdef USE_VBO
struct triVertex {
	float x, y, z;
	float r, g, b;
};
#else
	#ifdef USE_ARRAY_OF_STRUCTS
	struct triVertex{
		float x,y,z;
		float r,g,b;
	};
	#else
	struct triVertex {
		float x, y, z;
	};
	#endif
#endif

class Trangle : public PrimitiveBase {
public:
	Trangle()
#ifdef USING_OPENGL_ES
		: shaderID(0) {}
#elif defined(USING_D3D11)
	{}
#endif

	void Create();
	void Create(char *) {}
	void Transform(float *t);
	void Draw(float *t,float *vp);
	void Destroy();

#ifdef USING_OPENGL_ES
	GLuint	shaderID;
	GLuint	vertexAttribLoc;
	GLuint	colorAttribLoc;
	
	GLuint  matUniformLoc;
#ifdef USE_VBO
	triVertex		vertices[4];
	unsigned short	indices[6];
	GLuint			VB;
	GLuint			IB;
#else
	#ifdef USE_ARRAY_OF_STRUCTS
		triVertex	vertices[6];
	#else
		triVertex	positions[6];
		triVertex	colors[6];
	#endif
#endif
#elif defined(USING_D3D11)
	triVertex		vertices[4];
	unsigned short	indices[6];
#endif

	XMATRIX44	transform;
};

#endif
