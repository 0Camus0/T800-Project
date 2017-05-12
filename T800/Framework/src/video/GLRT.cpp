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

#include <video/GLRT.h>
#include <video/GLTexture.h>
#include <video/GLDriver.h>
#include <utils/Utils.h>

#if defined(OS_LINUX)
#include <sys/time.h>
#endif

bool GLRT::LoadAPIRT(){
	GLint cfmt, dfmt;
/*
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
	}*/
	cfmt = GL_RGB;
	dfmt = GL_DEPTH_COMPONENT;

//	w = 1280;
//	h = 720;

	GLuint fbo;
	GLuint dtex;
#if defined(OS_LINUX)
	timeval start;
    gettimeofday(&start,0);
#endif
	glGenFramebuffers(1, &fbo);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);


#if defined(USING_OPENGL_ES30) || defined(USING_OPENGL_ES31)
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
#elif  defined(USING_OPENGL_ES30) || defined(USING_OPENGL_ES31)
		glFramebufferTexture2D(GL_FRAMEBUFFER, Attachments[i], GL_TEXTURE_2D, ctex, 0);
#else
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+ i, GL_TEXTURE_2D, ctex, 0);
#endif

		CheckFBStatus();

		pTextureColor->id = ctex;
		vColorTextures.push_back(pTextureColor);
		vFrameBuffers.push_back(fbo);
		vGLColorTex.push_back(ctex);
	}

	glGenTextures(1, &dtex);
	glBindTexture(GL_TEXTURE_2D, dtex);
#ifdef USING_OPENGL_ES20
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
#else
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
#endif
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dtex, 0);

#if defined(OS_LINUX)
	timeval actual;
	gettimeofday(&actual,0);
	double ttaken = double( (actual.tv_sec - start.tv_sec) + (actual.tv_usec - start.tv_usec)/1000000.0);

	static int sample = 0;
	static double avg = 0.0;

	avg +=ttaken;
	sample++;

	if(sample>50){
        avg /= static_cast<double>(sample);
        printf("Average Time taken for FBO creation: %f \n",avg);
        sample = 0;
        avg = 0.0;
	}

#endif

	GLTexture *pTextureDepth = new GLTexture;
	pTextureDepth->id = dtex;
	this->pDepthTexture = pTextureDepth;
	DepthTexture = dtex;

	CheckFBStatus();

	return true;
}

void GLRT::DestroyAPIRT(){
	GLuint FBO = vFrameBuffers[0];
	glDeleteFramebuffers(1,&FBO);
	for (int i = 0; i < number_RT; i++) {
		GLTexture* tex = dynamic_cast<GLTexture*>(vColorTextures[i]);
		tex->DestroyAPITexture();
		delete tex;
	}

	GLTexture* tex = dynamic_cast<GLTexture*>(pDepthTexture);
	tex->DestroyAPITexture();
	delete tex;
}
