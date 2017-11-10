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

#include <scene/PrimitiveManager.h>
#include <scene/GLMesh.h>
#include <scene/GLQuad.h>

#ifdef USING_D3D11
#include <scene/windows/D3DXMesh.h>
#include <scene/windows/D3DXQuad.h>
#endif


PrimitiveBase*	PrimitiveManager::GetPrimitive(unsigned int index) {
	if (index >= primitives.size())
		return 0;

	return primitives[index];
}

int  PrimitiveManager::CreateTriangle() {
	return (int)(primitives.size()-1);
}

int	 PrimitiveManager::CreateCube(){
	return (int)(primitives.size() - 1);
}

int	 PrimitiveManager::CreateMesh(char *fname) {
#if defined(USING_GL_COMMON)
	PrimitiveBase *primitive = new GLMesh();
#elif defined(USING_D3D11)
	PrimitiveBase *primitive = new D3DXMesh();
#endif
	primitive->Create(fname);
	primitives.push_back(primitive);
	return (int)(primitives.size() - 1);
}

int PrimitiveManager::CreateQuad() {
#if defined(USING_GL_COMMON)
	PrimitiveBase *primitive = new GLQuad();
#elif defined(USING_D3D11)
	PrimitiveBase *primitive = new D3DXQuad();
#endif
	primitive->Create();
	primitives.push_back(primitive);
	return (int)(primitives.size() - 1);
}

void PrimitiveManager::SetSceneProps(SceneProps *p) {
	for (unsigned int i = 0; i < primitives.size(); i++) {
		primitives[i]->SetSceneProps(p);
	}
}

void PrimitiveManager::DrawPrimitives() {
	for(unsigned int i=0;i<primitives.size();i++){
		primitives[i]->Draw(0,&(*pVP).m[0][0]);
	}
}

void PrimitiveManager::DestroyPrimitives() {
	for (unsigned int i = 0; i < primitives.size(); i++) {
		primitives[i]->Destroy();
		delete primitives[i];
	}
	primitives.clear();
}
