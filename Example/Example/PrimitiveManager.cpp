
#include "PrimitiveManager.h"
#include "TriangleGL.h"


int  PrimitiveManager::CreateTriangle() {
	PrimitiveBase *primitive = new TrangleGL();
	primitive->Create();
	primitives.push_back(primitive);
	return (int)(primitives.size()-1);
}

void PrimitiveManager::TransformPrimitive(unsigned int index, float *mat) {
	if (index >= primitives.size())
		return;

	primitives[index]->Transform(mat);
}

void PrimitiveManager::DrawPrimitives() {
	for(unsigned int i=0;i<primitives.size();i++){
		primitives[i]->Draw();
	}
}

void PrimitiveManager::DestroyPrimitives() {
	for (unsigned int i = 0; i < primitives.size(); i++) {
		primitives[i]->Destroy();
		delete primitives[i];
	}
	primitives.clear();
}