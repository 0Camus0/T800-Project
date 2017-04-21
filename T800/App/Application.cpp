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

#include "Application.h"
#include <video/BaseDriver.h>
#include <scene/Cube.h>

#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>     

#define NUM_LIGHTS 128
#define RADI 170.0f

enum {
	SCENE = 0,
	CROC,	
	LINK,
	HOUSE_L,
	HOUSE_R,
	DRONE,
	CERDO,
	CERDOLIGHT,
	TOTAL_INSTANCES
};

void App::InitVars() {
	DtTimer.Init();
	DtTimer.Update();
	srand((unsigned int)DtTimer.GetDTSecs());

	
	Position = XVECTOR3(0.0f, 0.0f, 0.0f);
	Orientation = XVECTOR3(0.0f, 0.0f, 0.0f);
	Scaling = XVECTOR3(1.0f,1.0f,1.0f);
	SelectedMesh = 0;

	Cam.Init(XVECTOR3(0.0f, 1.0f, 10.0f), Deg2Rad(45.0f), 1280.0f / 720.0f, 1.0f, 8000.0f);
	Cam.Speed = 10.0f;
	Cam.Eye = XVECTOR3(0.0f, 9.75f, -31.0f);
	Cam.Pitch = 0.14f;
	Cam.Roll = 0.0f;
	Cam.Yaw = 0.020f;
	Cam.Update(0.0f);

	SceneProp.AddCamera(&Cam);
	for(int i=0;i<NUM_LIGHTS;i++){
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		SceneProp.AddLight(XVECTOR3(0.0f, 0.0f, 0.0f), XVECTOR3(r, g, b), true);
	}
	SceneProp.ActiveLights = 127;
	SceneProp.AmbientColor = XVECTOR3(0.15f, 0.15f, 0.15f);

	RTIndex = -1;
	FirstFrame = true;
}

void App::CreateAssets() {

	GBufferPass = pFramework->pVideoDriver->CreateRT(4, BaseRT::RGBA8, BaseRT::F32, 0, 0);
	DeferredPass = pFramework->pVideoDriver->CreateRT(1, BaseRT::RGBA8, BaseRT::F32, 0, 0);

	PrimitiveMgr.SetVP(&VP);
	
	int index = PrimitiveMgr.CreateMesh("Models/Scene.X");
	Pigs[0].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	index = PrimitiveMgr.CreateMesh("Models/NuCroc.X");
	Pigs[1].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	index = PrimitiveMgr.CreateMesh("Models/NuBatman.X");
	Pigs[2].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	index = PrimitiveMgr.CreateMesh("Models/NuVenomJok.X");
	Pigs[3].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	index = PrimitiveMgr.CreateMesh("Models/CerdoNuevo.X");
	Pigs[4].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	Pigs[5].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	PrimitiveMgr.GetPrimitive(index)->SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[0], 0);

	QuadIndex = PrimitiveMgr.CreateQuad();
	Quads[0].CreateInstance(PrimitiveMgr.GetPrimitive(QuadIndex), &VP);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[0], 0);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[1], 1);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[2], 2);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[3], 3);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[0]->pDepthTexture,4);

	Quads[1].CreateInstance(PrimitiveMgr.GetPrimitive(QuadIndex), &VP);
	Quads[2].CreateInstance(PrimitiveMgr.GetPrimitive(QuadIndex), &VP);
	Quads[3].CreateInstance(PrimitiveMgr.GetPrimitive(QuadIndex), &VP);
	Quads[4].CreateInstance(PrimitiveMgr.GetPrimitive(QuadIndex), &VP);
	Quads[5].CreateInstance(PrimitiveMgr.GetPrimitive(QuadIndex), &VP);
	Quads[6].CreateInstance(PrimitiveMgr.GetPrimitive(QuadIndex), &VP);
	Quads[7].CreateInstance(PrimitiveMgr.GetPrimitive(QuadIndex), &VP);


	PrimitiveMgr.SetSceneProps(&SceneProp);	
}

void App::DestroyAssets() {
	PrimitiveMgr.DestroyPrimitives();
}

void App::OnUpdate() {
	DtTimer.Update();
	DtSecs = DtTimer.GetDTSecs();
	OnInput();

	Cam.Update(DtSecs);
	VP = Cam.VP;

	int count = 1;
	Pigs[count].TranslateAbsolute(-15.0f, 0.0f, 0.0f);
	Pigs[count].RotateXAbsolute(0.0f);
	Pigs[count].RotateYAbsolute(0.0f);
	Pigs[count].RotateXAbsolute(0.0f);
	Pigs[count].ScaleAbsolute(0.15f);
	Pigs[count].Update();
	count++;
	
	Pigs[count].TranslateAbsolute(-5.0f, 0.0f, 0.0f);
	Pigs[count].RotateXAbsolute(0.0f);
	Pigs[count].RotateYAbsolute(0.0f);
	Pigs[count].RotateXAbsolute(0.0f);
	Pigs[count].ScaleAbsolute(0.15f);
	Pigs[count].Update();
	count++;

	Pigs[count].TranslateAbsolute(5.0f, 0.0f, 0.0f);
	Pigs[count].RotateXAbsolute(0.0f);
	Pigs[count].RotateYAbsolute(0.0f);
	Pigs[count].RotateXAbsolute(0.0f);
	Pigs[count].ScaleAbsolute(0.15f);
	Pigs[count].Update();
	count++;

	Pigs[count].TranslateAbsolute(18.0f, 4.2f, 0.0f);
	Pigs[count].RotateXAbsolute(0.0f);
	Pigs[count].RotateYAbsolute(60.0f);
	Pigs[count].RotateZAbsolute(90.0f);
	Pigs[count].ScaleAbsolute(3.0f);
	Pigs[count].Update();
	count++;

	Quads[0].TranslateAbsolute(0.0f,0.0f, 0.0f);
	Quads[0].Update();

	Quads[1].ScaleAbsolute(0.25f);
	Quads[1].TranslateAbsolute(-0.75f, +0.75f, 0.0f);
	Quads[1].Update();

	Quads[2].ScaleAbsolute(0.25f);
	Quads[2].TranslateAbsolute(0.75f, +0.75f, 0.0f);
	Quads[2].Update();

	Quads[3].ScaleAbsolute(0.25f);
	Quads[3].TranslateAbsolute(-0.75f, -0.75f, 0.0f);
	Quads[3].Update();

	Quads[4].ScaleAbsolute(0.25f);
	Quads[4].TranslateAbsolute(0.75f, -0.75f, 0.0f);
	Quads[4].Update();

	Quads[5].ScaleAbsolute(0.25f);
	Quads[5].TranslateAbsolute(0.75f, 0.0f, 0.0f);
	Quads[5].Update();

	Quads[6].ScaleAbsolute(0.25f);
	Quads[6].TranslateAbsolute(-0.75f, 0.0f, 0.0f);
	Quads[6].Update();

	Quads[7].ScaleAbsolute(1.0f);
	Quads[7].TranslateAbsolute(0.0f, 0.0f, 0.1f);
	Quads[7].Update();


	float speed = 0.5f;
	static float freq = 0.0f;
	freq += DtSecs*speed;
	static float freq2 = 3.1415f / 2.0f;
	freq2 += DtSecs*speed;

	float dist = RADI;
	float Offset = 2.0f*3.1415f/ NUM_LIGHTS;
	float Offset2 = 4.0f*3.1415f / NUM_LIGHTS;
	for (int i = 0; i<NUM_LIGHTS; i++) {
		SceneProp.Lights[i].Position = Position; 
		float RadA = dist*0.35f + dist*0.4f * sin(freq + float(i*Offset))*cos(freq + float(i*Offset));
		float RadB = dist*0.35f + dist*0.4f * sin(freq2 + float(i*Offset2))*cos(freq2 + float(i*Offset2));
		SceneProp.Lights[i].Position.x += RadA*sin(freq + float(i*Offset));
		SceneProp.Lights[i].Position.z += RadB*cos(freq + float(i*Offset2));
	}

	
	PrimitiveInst *Sel = &Pigs[5];
	Sel->TranslateAbsolute(Position.x, Position.y, Position.z);
	Sel->RotateXAbsolute(Orientation.x);
	Sel->RotateYAbsolute(Orientation.y);
	Sel->RotateZAbsolute(Orientation.z);
	Sel->ScaleAbsolute(Scaling.x);
	Sel->Update();
	
	OnDraw();
}

void App::OnDraw() {

	pFramework->pVideoDriver->PushRT(GBufferPass);
	for (int i = 0; i < 6; i++) {
		Pigs[i].SetSignature(Signature::GBUFF_PASS);
		Pigs[i].Draw();
		Pigs[i].SetSignature(Signature::FORWARD_PASS);
	}
	pFramework->pVideoDriver->PopRT();

	
	pFramework->pVideoDriver->PushRT(DeferredPass);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[0], 0);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[1], 1);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[2], 2);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[3], 3);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[0]->pDepthTexture, 4);
	Quads[0].SetSignature(Signature::DEFERRED_PASS);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();


	pFramework->pVideoDriver->Clear();
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[0]->pDepthTexture,0);
	Quads[1].SetSignature(Signature::FSQUAD_1_TEX);
	Quads[1].Draw();

	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[0], 0);
	Quads[2].SetSignature(Signature::FSQUAD_1_TEX);
	Quads[2].Draw();

	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[1], 0);
	Quads[3].SetSignature(Signature::FSQUAD_1_TEX);
	Quads[3].Draw();

	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[2], 0);
	Quads[4].SetSignature(Signature::FSQUAD_1_TEX);
	Quads[4].Draw();

	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[3], 0);
	Quads[5].SetSignature(Signature::FSQUAD_1_TEX);
	Quads[5].Draw();

	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[1]->vColorTextures[0], 0);
	Quads[6].SetSignature(Signature::FSQUAD_1_TEX);
	Quads[6].Draw();

	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[1]->vColorTextures[0], 0);
	Quads[7].SetSignature(Signature::FSQUAD_1_TEX);
	Quads[7].Draw();


	pFramework->pVideoDriver->SwapBuffers();

	FirstFrame = false;
}

void App::OnInput() {

	if (FirstFrame)
		return;

	bool changed = false;
	const float speedFactor = 10.0f;
	if (IManager.PressedKey(SDLK_UP)) {
		Position.y += 1.0f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedKey(SDLK_DOWN)) {
		Position.y -= 1.0f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedKey(SDLK_LEFT)) {
		Position.x -= 1.0f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedKey(SDLK_RIGHT)) {
		Position.x += 1.0f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedKey(SDLK_z)) {
		Position.z -= 1.0f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedKey(SDLK_x)) {
		Position.z += 1.0f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedOnceKey(SDLK_KP_PLUS)) {
		SceneProp.ActiveLights *= 2;
		/*Scaling.x += 0.1f*speedFactor*DtSecs;
		Scaling.y += 0.1f*speedFactor*DtSecs;
		Scaling.z += 0.1f*speedFactor*DtSecs;
		changed = true;*/
	}

	if (IManager.PressedOnceKey(SDLK_KP_MINUS)) {
		SceneProp.ActiveLights /= 2;
		if(SceneProp.ActiveLights<=0)
			SceneProp.ActiveLights = 1;
		/*Scaling.x -= 0.1f*speedFactor*DtSecs;
		Scaling.y -= 0.1f*speedFactor*DtSecs;
		Scaling.z -= 0.1f*speedFactor*DtSecs;
		changed = true;*/
	}

	if (IManager.PressedKey(SDLK_KP5)) {
		Orientation.x -= 60.0f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedKey(SDLK_KP6)) {
		Orientation.x += 60.0f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedKey(SDLK_KP2)) {
		Orientation.y -= 60.0f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedKey(SDLK_KP3)) {
		Orientation.y += 60.0f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedKey(SDLK_KP0)) {
		Orientation.z -= 60.0f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedKey(SDLK_KP_PERIOD)) {
		Orientation.z += 60.0f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedKey(SDLK_KP_PERIOD)) {
		Orientation.z += 60.0f*speedFactor*DtSecs;
		changed = true;
	}

	bool displayInfo = false;
	if (changed && displayInfo) {
		printf("Position[%f,%f,%f] Rot[%f,%f,%f] Sc[%f]\n",Position.x,Position.y,Position.z,Orientation.x,Orientation.y,Orientation.z,Scaling.x);
	}

	//printf("Position[%f, %f, %f]\n\n", Cam.Eye.x, Cam.Eye.y, Cam.Eye.z);
	//printf("Orientation[%f, %f, %f]\n\n", Cam.Pitch, Cam.Roll, Cam.Yaw);

	if (IManager.PressedKey(SDLK_w)) {
		Cam.MoveForward(DtSecs);
	}

	if (IManager.PressedKey(SDLK_s)) {
		Cam.MoveBackward(DtSecs);
	}

	if (IManager.PressedKey(SDLK_a)) {
		Cam.StrafeLeft(DtSecs);
	}

	if (IManager.PressedKey(SDLK_d)) {
		Cam.StrafeRight(DtSecs);
	}
	
	float yaw = 0.005f*static_cast<float>(IManager.xDelta);
	Cam.MoveYaw(yaw);

	float pitch = 0.005f*static_cast<float>(IManager.yDelta);
	Cam.MovePitch(pitch);


}

void App::OnPause() {

}

void App::OnResume() {

}

void App::OnReset() {

}