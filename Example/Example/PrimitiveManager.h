#ifndef UAD_PRIMITIVEMANAGER_H
#define UAD_PRIMITIVEMANAGER_H

#include "Config.h"

#include <vector>
#include <xMaths.h>
#include "PrimitiveBase.h"

class PrimitiveManager {
public:
	void SetVP(XMATRIX44 *vp) {
		pVP = vp;
	}
	int  CreateTriangle();
	int	 CreateCube();
	int	 CreateMesh(char *fname);

	void DrawPrimitives();
	void DestroyPrimitives();
	PrimitiveBase*	GetPrimitive(unsigned int);

	std::vector<PrimitiveBase*> primitives;

	XMATRIX44 *pVP;
};

#endif