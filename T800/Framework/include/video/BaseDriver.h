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

#ifndef T800_BASEDRIVER_H
#define T800_BASEDRIVER_H

#include <Config.h>
#include <vector>

struct BaseRT {
	enum ATTACHMENTS{
		COLOR0_ATTACHMENT = 1,
		COLOR1_ATTACHMENT = 2,
		COLOR2_ATTACHMENT = 4,
		COLOR3_ATTACHMENT = 8,
		DEPTH_ATTACHMENT  = 16
	};

	enum FORMAT {
		FD16 = 0,
		F32,
		RGB8,
		RGBA8,
		RGBA32F,
		BGR8,
		BGRA8,
		BGRA32
	};

	void			LoadRT(int nrt, int cf, int df, int w, int h);
	virtual void	LoadAPIRT() = 0;

	int ID;
	int w;
	int h;
	int number_RT;
	int color_format;
	int depth_format;
};

class BaseDriver {
public:
	BaseDriver() {  }
	virtual	void	InitDriver() = 0;
	virtual void	CreateSurfaces() = 0;
	virtual void	DestroySurfaces() = 0;
	virtual void	Update() = 0;
	virtual void	DestroyDriver() = 0;
	virtual void	SetWindow(void *window) = 0;
	virtual void	SetDimensions(int,int) = 0;
	virtual void	Clear() = 0;	
	virtual void	SwapBuffers() = 0;

	virtual int 	CreateRT(int nrt, int cf, int df,int w, int h) = 0;
	virtual void	PushRT(int id) = 0;
	virtual void	PopRT() = 0;


	std::vector<BaseRT*> RTs;
};

#endif