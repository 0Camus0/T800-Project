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

#ifndef UAD_SCENEPROPS_H
#define UAD_SCENEPROPS_H

#include <Config.h>

#include <utils\xMaths.h>
#include <utils\Camera.h>
#include <vector>

struct Light{
	XVECTOR3 Position;
	XVECTOR3 Color;
	int		 Type;
	int		 Enabled;
};

struct SceneProps{
	SceneProps() : ActiveCamera(0) , ActiveLights(0) {}

	void	AddLight(XVECTOR3 Pos, XVECTOR3 Color,bool enabled);
	void	RemoveLight(unsigned int index);
	void	SetLightPos(unsigned int index, XVECTOR3);

	void	AddCamera(Camera*);
	void	RemoveCamera(unsigned int index);
	

	std::vector<Light>	   Lights;
	std::vector<Camera*> pCameras;

	XVECTOR3			AmbientColor;

	int	ActiveLights;
	int ActiveCamera;
};

#endif