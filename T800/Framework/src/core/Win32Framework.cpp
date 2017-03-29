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

#include <core\Win32Framework.h>
#ifdef USING_OPENGL_ES
#include <video\GLDriver.h>
#elif defined(USING_D3D11)
#include <video\D3DXDriver.h>
#endif
// SDL
#include <SDL/SDL.h>
// Windows 
#include <windows.h>
#include <mmsystem.h>

void Win32Framework::InitGlobalVars() {

	
}

void Win32Framework::OnCreateApplication(){
	SDL_Init(SDL_INIT_VIDEO);
	SDL_WM_SetCaption("UAD Framework Daniel", 0);
	int flags = SDL_HWSURFACE;
	//flags |= SDL_FULLSCREEN;
	//flags |= SDL_RESIZABLE;
	int width = 1280;
	int height = 720;
	SDL_SetVideoMode(width, height, 32, flags);
#ifdef USING_OPENGL_ES
	pVideoDriver = new GLDriver;
#elif defined(USING_D3D11)
	pVideoDriver = new D3DXDriver;
	pVideoDriver->SetDimensions(width, height);
#endif
	pVideoDriver->SetWindow(0);
	pVideoDriver->InitDriver();

	pBaseApp->InitVars();
	pBaseApp->CreateAssets();

	
}
void Win32Framework::OnDestroyApplication() {
	pBaseApp->DestroyAssets();
	pVideoDriver->DestroyDriver();
	delete pVideoDriver;
	SDL_Quit();
}
void Win32Framework::OnInterruptApplication() {
}
void Win32Framework::OnResumeApplication() {
}
void Win32Framework::UpdateApplication() {
	while (m_alive) {
		ProcessInput();
		pBaseApp->OnUpdate();
	}
}
void Win32Framework::ProcessInput() {
	SDL_Event       evento;
	while (SDL_PollEvent(&evento)) {
		switch (evento.type) {
			case SDL_KEYDOWN: {
				if (evento.key.keysym.sym == SDLK_q) {
					m_alive = false;
				}
				pBaseApp->IManager.KeyStates[0][evento.key.keysym.sym] = true;
			}break;
			case SDL_KEYUP: {
				pBaseApp->IManager.KeyStates[0][evento.key.keysym.sym] = false;
				pBaseApp->IManager.KeyStates[1][evento.key.keysym.sym] = false;
			}break;
	
		}
	}
	static int xDelta = 0;
	static int yDelta = 0;
	int x = 0, y = 0;

	SDL_GetMouseState(&x,&y);

	xDelta = x - xDelta;
	yDelta = y - yDelta;

	pBaseApp->IManager.xDelta = xDelta;
	pBaseApp->IManager.yDelta = yDelta;

	xDelta = x;
	yDelta = y;
}

void Win32Framework::ResetApplication() {
}