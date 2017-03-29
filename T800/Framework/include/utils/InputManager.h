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

#ifndef UAD_INPUT_MANAGER_H
#define UAD_INPUT_MANAGER_H

#include <Config.h>

#include <SDL/SDL_keysym.h>

#define MAXKEYS 512
#define MAXMOUSEBUTTONS 5


class InputManager {
public:
	InputManager();
	bool	PressedOnceKey(int key);
	bool	PressedOnceMouseButton(int mb);

	bool	PressedKey(int key);
	bool	PressedMouseButton(int mb);
	
	bool	KeyStates[2][MAXKEYS];
	bool	MouseButtonStates[2][MAXMOUSEBUTTONS];

	int		xDelta;
	int 	yDelta;
};

#endif