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

#include <video\GLRT.h>
#include <video\TextureGL.h>

bool GLES20RT::LoadAPIRT(){
	GLint cfmt, dfmt;

	cfmt = GL_RGB;
	dfmt = GL_DEPTH_COMPONENT;
	
	GLuint fbo;
	GLuint dtex;

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &dtex);
	glBindTexture(GL_TEXTURE_2D, dtex);
	glTexImage2D(GL_TEXTURE_2D, 0, dfmt, w, h, 0, dfmt, GL_UNSIGNED_INT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dtex, 0);

	TextureGL *pTextureDepth = new TextureGL;
	pTextureDepth->id = dtex;
	this->pDepthTexture = pTextureDepth;
	DepthTexture = dtex;
#ifdef USING_OPENGL_ES
	number_RT = 1;
#endif
	for (int i = 0; i < number_RT; i++) {
		
		GLuint ctex;
		glGenTextures(1, &ctex);
		glBindTexture(GL_TEXTURE_2D, ctex);

		glTexImage2D(GL_TEXTURE_2D, 0, cfmt, w, h, 0, cfmt, GL_UNSIGNED_BYTE, 0);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);			

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, ctex, 0);
		
		TextureGL *pTextureColor = new TextureGL;
		pTextureColor->id = ctex;
		vColorTextures.push_back(pTextureColor);
		vFrameBuffers.push_back(fbo);
		vGLColorTex.push_back(ctex);
	}


	return true;
}