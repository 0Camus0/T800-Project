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

#ifndef T800_SCENEPROPS_H
#define T800_SCENEPROPS_H

#include <Config.h>

#include <utils/xMaths.h>
#include <utils/Camera.h>
#include <vector>

struct Light{
	XVECTOR3 Position;
	XVECTOR3 Color;
	int		 Type;
	int		 Enabled;
};

struct GaussFilter{
	float sigma;
	float radius;
	int kernelSize;
	void Update();
	std::vector<XVECTOR3> vGaussKernel;
};
struct SceneProps{
	SceneProps() : ActiveCamera(0) , ActiveLights(1), ActiveLightCamera(0), ActiveGaussKernel(0), Exposure(0.2f) , BloomFactor(1.0f) {}

	void	AddLight(XVECTOR3 Pos, XVECTOR3 Color,bool enabled);
	void	RemoveLight(unsigned int index);
	void	SetLightPos(unsigned int index, XVECTOR3);

	void	AddCamera(Camera*);
	void	RemoveCamera(unsigned int index);

	void	AddLightCamera(Camera*);
	void	RemoveLightCamera(unsigned int index);

	void	AddGaussKernel(GaussFilter*);

	std::vector<Light>	   Lights;
	std::vector<Camera*> pCameras;

	std::vector<Camera*> pLightCameras;

	std::vector<GaussFilter*> pGaussKernels;

	XVECTOR3			AmbientColor;

	int ActiveCamera;
	int	ActiveLights;
	int ActiveLightCamera;
	int ActiveGaussKernel;
	
	// HDR
	float	Exposure;
	float	BloomFactor;
};

#endif
