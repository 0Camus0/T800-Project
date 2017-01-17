#ifndef UAD_BASEDRIVER_H
#define UAD_BASEDRIVER_H

class BaseDriver {
public:
	BaseDriver() {  }
	virtual	void	InitDriver() = 0;
	virtual void	CreateSurfaces() = 0;
	virtual void	DestroySurfaces() = 0;
	virtual void	Update() = 0;
	virtual void	DestroyDriver() = 0;
	virtual void	SetWindow(void *window) = 0;

	virtual void	Clear() = 0;	
	virtual void	SwapBuffers() = 0;
};

#endif