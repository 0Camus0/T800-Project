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
#include <video\GLTexture.h>
#include <video\GLDriver.h>

bool GLRT::LoadAPIRT(){
	GLint cfmt, dfmt;

	switch (cfmt) {
		case FD16: {
		}break;
		case F32: {
		}break;
		case RGB8: {
		}break;
		case RGBA8: {
		}break;
		case RGBA16F: {
		}break;
		case RGBA32F: {
		}break;
		case BGR8: {
		}break;
		case BGRA8: {
		}break;
		case BGRA32: {
		}break;
	}
	cfmt = GL_RGB;
	dfmt = GL_DEPTH_COMPONENT;
	
	GLuint fbo;
	GLuint dtex;

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &dtex);
	glBindTexture(GL_TEXTURE_2D, dtex);
#ifdef USING_OPENGL_ES20
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
#else
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, w, h, 0, dfmt, GL_FLOAT, NULL);
#endif
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dtex, 0);

	GLTexture *pTextureDepth = new GLTexture;
	pTextureDepth->id = dtex;
	this->pDepthTexture = pTextureDepth;
	DepthTexture = dtex;

#if defined(USING_OPENGL_ES30)
	int	Attachments[8];
	Attachments[0] = GL_COLOR_ATTACHMENT0;
	for(int i=1;i<8;i++){
		Attachments[i] = GL_COLOR_ATTACHMENT1 + (i-1);
	}
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

		GLTexture *pTextureColor = new GLTexture;
#if defined(USING_OPENGL_ES20)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ctex, 0);
		for (int i = 0; i < number_RT; i++) {
			pTextureColor->id = ctex;
			vColorTextures.push_back(pTextureColor);
			vFrameBuffers.push_back(fbo);
			vGLColorTex.push_back(ctex);
		}
		break;		
#elif  defined(USING_OPENGL_ES30) 
		glFramebufferTexture2D(GL_FRAMEBUFFER, Attachments[i], GL_TEXTURE_2D, ctex, 0);		
#else
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+ i, GL_TEXTURE_2D, ctex, 0);
#endif	
		pTextureColor->id = ctex;
		vColorTextures.push_back(pTextureColor);
		vFrameBuffers.push_back(fbo);
		vGLColorTex.push_back(ctex);				
	}


	return true;
}