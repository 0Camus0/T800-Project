#ifndef UAD_PRIMITIVEBASE_H
#define UAD_PRIMITIVEBASE_H

#include "Config.h"

#ifndef BUFFER_OFFSET
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#endif

class PrimitiveBase {
public:
	virtual void Create() = 0;
	virtual void Create(char *) = 0;
	virtual void Transform(float *t) = 0;
	virtual void Draw(float *t,float *vp) = 0;
	virtual void Destroy() = 0;
};


#endif