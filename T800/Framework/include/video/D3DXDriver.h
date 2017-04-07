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

#ifndef T800_D3DXDRIVER_H
#define T800_D3DXDRIVER_H

#include <Config.h>

#include <video\BaseDriver.h>

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

	int 	 CreateTexture(std::string);
	Texture* GetTexture(int id);
	void	 DestroyTexture();

	int 	CreateRT(int nrt, int cf, int df, int w, int h);
	void	PushRT(int id);
	void	PopRT();
	void	DestroyRTs();

	int			CreateShader(std::string src_vs, std::string src_fs, unsigned int sig);
	ShaderBase*	GetShaderSig(unsigned int sig);
	ShaderBase*	GetShaderIdx(int Id);
	void		DestroyShaders();

	void	Clear();
	void	SwapBuffers();

	int		Width;
	int		Height;
	HWND	hwnd;
};

#endif