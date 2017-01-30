#ifndef UAD_PRIMITIVEBASE_H
#define UAD_PRIMITIVEBASE_H

class PrimitiveBase {
public:
	virtual void Create() = 0;
	virtual void Transform(float *t) = 0;
	virtual void Draw(float *t,float *vp) = 0;
	virtual void Destroy() = 0;
};


#endif