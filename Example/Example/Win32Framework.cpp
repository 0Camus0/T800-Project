#include "Win32Framework.h"
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
	flags |= SDL_RESIZABLE;
	int width = 1280;
	int height = 720;
	SDL_SetVideoMode(width, height, 32, flags);
}
void Win32Framework::OnDestroyApplication() {
}
void Win32Framework::OnInterruptApplication() {
}
void Win32Framework::OnResumeApplication() {
}
void Win32Framework::UpdateApplication() {
	for (;;) {
	}
}
void Win32Framework::ProcessInput() {
}
void Win32Framework::ResetApplication() {
}