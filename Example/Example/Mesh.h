#ifndef UAD_MESH_GL_H
#define UAD_MESH_GL_H

#include "Config.h"

#ifdef USING_OPENGL_ES
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "TextureGL.h"
#elif defined(USING_D3D11)
#include "TextureD3D.h"
#endif

#include <xMaths.h>
#include <XDataBase.h>

#include "PrimitiveBase.h"
#include "Utils.h"


#include <vector>

using namespace xF;

class Mesh : public PrimitiveBase {
public:
	Mesh()  {}
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