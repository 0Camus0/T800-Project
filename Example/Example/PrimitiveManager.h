#ifndef UAD_PRIMITIVEMANAGER_H
#define UAD_PRIMITIVEMANAGER_H

#include <vector>
#include <d3dx9math.h>
#include "PrimitiveBase.h"

class PrimitiveManager {
public:
	void SetVP(D3DXMATRIX *vp) {
		pVP = vp;
	}
	int  CreateTriangle();
	int	 CreateCube();

	void DrawPrimitives();
	void DestroyPrimitives();
	PrimitiveBase*	GetPrimitive(unsigned int);

	std::vector<PrimitiveBase*> primitives;

	D3DXMATRIX *pVP;
};

#endif