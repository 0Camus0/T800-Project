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

#ifndef UAD_PRIMITIVEBASE_H
#define UAD_PRIMITIVEBASE_H

#include <Config.h>

#include <scene\SceneProp.h>

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


	void SetSceneProps(SceneProps *p) { pScProp = p; }
	SceneProps	*pScProp;
};


#endif