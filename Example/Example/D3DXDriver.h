#ifndef UAD_D3DXDRIVER_H
#define UAD_D3DXDRIVER_H

#include "Config.h"

#include "BaseDriver.h"

#include <d3d11.h>
#include <dxgi.h>
#include <D3Dcompiler.h>

#include <wrl.h>
#include <wrl/client.h>
using namespace Microsoft::WRL;

class D3DXDriver : public BaseDriver {
public:
	D3DXDriver() {  }
	void	InitDriver();
	void	CreateSurfaces();
	void	DestroySurfaces();
	void	Update();
	void	DestroyDriver();
	void	SetWindow(void *window);
	void	SetDimensions(int, int);

	void	Clear();
	void	SwapBuffers();

	int		Width;
	int		Height;
	HWND	hwnd;
};

#endif