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

#ifndef UAD_BASEDRIVER_H
#define UAD_BASEDRIVER_H

#include <Config.h>

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
};

#endif