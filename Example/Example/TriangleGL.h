#ifndef UAD_TRIANGLEGL_H
#define UAD_TRIANGLEGL_H


#include "PrimitiveBase.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

struct triVertex{
	float x,y,z;
};

class TrangleGL : public PrimitiveBase {
public:
	TrangleGL() : shaderID(0) {}
	void Create();
	void Transform(float *t);
	void Draw();
	void Destroy();

	GLuint	shaderID;
	GLuint	vertexAttribLoc;

	triVertex	vertices[3];
	
};

#endif
