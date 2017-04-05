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

#ifndef T800_GLESXRT_H
#define T800_GLESXRT_H

#include <video/BaseDriver.h>
#include <vector>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

class GLES20RT : public BaseRT {
public:

	bool	LoadAPIRT();

	std::vector<GLuint>		vFrameBuffers;
	std::vector<GLuint>		vGLColorTex;
	GLuint					DepthFrameBuffer;
	GLuint					DepthTexture;
};


#endif