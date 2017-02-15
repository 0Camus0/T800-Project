#ifndef UAD_MESH_GL_H
#define UAD_MESH_GL_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <xMaths.h>
#include <XDataBase.h>

#include "PrimitiveBase.h"
#include "UtilsGL.h"
#include "TextureGL.h"

#include <vector>

using namespace xF;

class MeshGL : public PrimitiveBase {
public:
	MeshGL()  {}
	void Create() {}
	void Create(char *);
	void Transform(float *t);
	void Draw(float *t, float *vp);
	void Destroy();

	XMATRIX44	transform;
	XDataBase	xFile;
	std::vector<Texture*> Textures;
;
};


#endif