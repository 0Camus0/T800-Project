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


#include <scene/SceneProp.h>


void	SceneProps::AddLight(XVECTOR3 Pos, XVECTOR3 Color, bool enabled){
	Light l;
	l.Position=Pos;
	l.Color=Color;
	l.Enabled=(int)enabled;
	Lights.push_back(l);
}

void	SceneProps::RemoveLight(unsigned int index){
	if(index < 0 || index >= Lights.size())
		return;

	Lights.erase(Lights.begin() + index);
}

void	SceneProps::SetLightPos(unsigned int index, XVECTOR3 pos){
	if (index < 0 || index >= Lights.size())
		return;

	Lights[index].Position = pos;
}

void	SceneProps::AddCamera(Camera* cam){
	pCameras.push_back(cam);
}

void	SceneProps::RemoveCamera(unsigned int index){
	if (index < 0 || index >= pCameras.size())
		return;

	pCameras.erase(pCameras.begin() + index);
}

void	SceneProps::AddLightCamera(Camera* cam) {
	pLightCameras.push_back(cam);
}

void	SceneProps::RemoveLightCamera(unsigned int index) {
	if (index < 0 || index >= pLightCameras.size())
		return;

	pLightCameras.erase(pLightCameras.begin() + index);
}

void SceneProps::UpdateGaussKernel(float sigma, float radius, int kernelSize){
	if ((kernelSize - 1) % 2 != 0)
		kernelSize--;

	std::vector<sample_> allSamples = UpdateKernel(sigma,(float)kernelSize,1000.0f);
	XVECTOR3 KernelSize = XVECTOR3((float)allSamples.size(), radius,0.0f,0.0f);
	vGaussKernel.clear();
	vGaussKernel.push_back(KernelSize);
	for(unsigned int i=0;i<allSamples.size();i++){
		vGaussKernel.push_back(XVECTOR3(allSamples[i].weight,0.0f,0.0f,0.0f));
	}
}