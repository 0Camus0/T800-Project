#ifndef UAD_PRIMITIVEMANAGER_H
#define UAD_PRIMITIVEMANAGER_H

#include <vector>
#include "PrimitiveBase.h"

class PrimitiveManager {
public:
	int  CreateTriangle();

	void TransformPrimitive(unsigned int,float *);
	void DrawPrimitives();
	void DestroyPrimitives();


	std::vector<PrimitiveBase*> primitives;
};

#endif