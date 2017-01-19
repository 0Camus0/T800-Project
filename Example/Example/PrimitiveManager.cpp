
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

}

void PrimitiveManager::DestroyPrimitives() {

}