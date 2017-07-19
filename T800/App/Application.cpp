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
#include <utils/InputManager.h>
#include <scene/Cube.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SEPARATED_BLUR 0
#define DEGENERATED_FBO_TEST 0

#define NUM_LIGHTS 64
#define RADI 170.0f

#if defined(OS_LINUX)
#include <sys/time.h>
#endif

#include <iostream>
#include <string>
#include <vector>


extern std::vector<std::string> g_args;


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

	CamSelection = NORMAL_CAM1;

	Cam.Init(XVECTOR3(0.0f, 1.0f, 10.0f), Deg2Rad(45.0f), 1280.0f / 720.0f, 1.0f, 8000.0f);
	Cam.Speed = 10.0f;
	Cam.Eye = XVECTOR3(0.0f, 9.75f, -31.0f);
	Cam.Pitch = 0.14f;
	Cam.Roll = 0.0f;
	Cam.Yaw = 0.020f;
	Cam.Update(0.0f);

	LightCam.Init(XVECTOR3(0.0f, 1.0f, 10.0f), Deg2Rad(60.0f), 1.0f, 0.1f, 8000.0f);
	LightCam.Speed = 10.0f;
	LightCam.Eye = XVECTOR3(0.0f, 25.0f, -40.0f);
	LightCam.Pitch = 0.14f;
	LightCam.Roll = 0.0f;
	LightCam.Yaw = 0.020f;
	LightCam.Update(0.0f);

	ActiveCam = &Cam;

	SceneProp.AddCamera(ActiveCam);
	SceneProp.AddLightCamera(&LightCam);
	for(int i=0;i<NUM_LIGHTS;i++){
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		SceneProp.AddLight(XVECTOR3(0.0f, 0.0f, 0.0f), XVECTOR3(r, g, b), true);
	}
	SceneProp.ActiveLights = 127;
	SceneProp.AmbientColor = XVECTOR3(0.15f, 0.15f, 0.15f);

	ShadowFilter.kernelSize = 7;
	ShadowFilter.radius = 2.0f;
	ShadowFilter.sigma = 1.0f;
	ShadowFilter.Update();

	BloomFilter.kernelSize = 21;
	BloomFilter.radius = 2.0f;
	BloomFilter.sigma = 3.0f;
	BloomFilter.Update();
	
	SceneProp.AddGaussKernel(&ShadowFilter);
	SceneProp.AddGaussKernel(&BloomFilter);
	SceneProp.ActiveGaussKernel = SHADOW_KERNEL;

	RTIndex = -1;
	FirstFrame = true;
}

void App::CreateAssets() {

	EnvMapTexIndex = g_pBaseDriver->CreateTexture(string("CubeMap_Granja_RAW.dds"));

	GBufferPass = pFramework->pVideoDriver->CreateRT(4, BaseRT::RGBA8, BaseRT::F32, 0, 0);
	DeferredPass = pFramework->pVideoDriver->CreateRT(1, BaseRT::RGBA8, BaseRT::F32, 0, 0);
	DepthPass = pFramework->pVideoDriver->CreateRT(0, BaseRT::NOTHING, BaseRT::F32, 512, 512);
	ShadowAccumPass = pFramework->pVideoDriver->CreateRT(1, BaseRT::RGBA8, BaseRT::F32, 0, 0);
	ExtraHelperPass = pFramework->pVideoDriver->CreateRT(1, BaseRT::RGBA8, BaseRT::F32, 0, 0);
	BloomAccumPass = pFramework->pVideoDriver->CreateRT(1, BaseRT::RGBA8, BaseRT::F32, 0, 0);

	PrimitiveMgr.SetVP(&VP);

	int index = PrimitiveMgr.CreateMesh("Models/Scene.X");
	Pigs[0].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	PrimitiveMgr.GetPrimitive(index)->SetEnvironmentMap(g_pBaseDriver->GetTexture(EnvMapTexIndex));

	index = PrimitiveMgr.CreateMesh("Models/NuCroc.X");
	Pigs[1].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	PrimitiveMgr.GetPrimitive(index)->SetEnvironmentMap(g_pBaseDriver->GetTexture(EnvMapTexIndex));

    index = PrimitiveMgr.CreateMesh("Models/NuBatman.X");
	Pigs[2].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	PrimitiveMgr.GetPrimitive(index)->SetEnvironmentMap(g_pBaseDriver->GetTexture(EnvMapTexIndex));

	index = PrimitiveMgr.CreateMesh("Models/NuVenomJok.X");
	Pigs[3].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	PrimitiveMgr.GetPrimitive(index)->SetEnvironmentMap(g_pBaseDriver->GetTexture(EnvMapTexIndex));

	index = PrimitiveMgr.CreateMesh("Models/CerdoNuevo.X");
	Pigs[4].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	PrimitiveMgr.GetPrimitive(index)->SetEnvironmentMap(g_pBaseDriver->GetTexture(EnvMapTexIndex));

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

	for(unsigned int i=0;i<g_args.size();i++){
        if(g_args[i] == "-l"){
           SceneProp.ActiveLights = atoi( g_args[i+1].c_str() );
        }
	}
}

void App::DestroyAssets() {
	PrimitiveMgr.DestroyPrimitives();
}

void App::OnUpdate() {

#if defined(OS_LINUX)
	timeval start;
    gettimeofday(&start,0);
#endif

	DtTimer.Update();
	DtSecs = DtTimer.GetDTSecs();
	OnInput();


	ActiveCam->Update(DtSecs);
	VP = ActiveCam->VP;

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

#if defined(OS_LINUX)
	timeval actual;
	gettimeofday(&actual,0);
	double ttaken = double( (actual.tv_sec - start.tv_sec) + (actual.tv_usec - start.tv_usec)/1000000.0);

	static int sample = 0;
	static double avg = 0.0;
	static int framescount = 0;

	static int total_sample = 0;
	static double total_avg = 0.0;

	avg +=ttaken;
	total_avg += ttaken;
	sample++;


	if(avg>1.0){
       // printf("FPS: %d \n",sample);
        total_sample+=sample;
        framescount++;
        sample = 0;
        avg = 0.0;
	}

	static const double TIMETOTAKE = 60.0;
	if(total_avg>TIMETOTAKE){
        total_sample /= static_cast<double>(framescount);
        printf("AVG after %f FPS: %d exiting now\n",TIMETOTAKE,total_sample);
        exit(0);
	}

#endif
}

void App::OnDraw() {

	pFramework->pVideoDriver->Clear();


	pFramework->pVideoDriver->PushRT(DepthPass);
	SceneProp.pCameras[0] = &LightCam;
	for (int i = 0; i < 6; i++) {
		Pigs[i].SetSignature(Signature::SHADOW_MAP_PASS);
		Pigs[i].Draw();
		Pigs[i].SetSignature(Signature::FORWARD_PASS);
	}
	pFramework->pVideoDriver->PopRT();

	SceneProp.pCameras[0] = &Cam;

	pFramework->pVideoDriver->PushRT(GBufferPass);
	for (int i = 0; i < 6; i++) {
		Pigs[i].SetSignature(Signature::GBUFF_PASS);
		Pigs[i].Draw();
		Pigs[i].SetSignature(Signature::FORWARD_PASS);
	}
	pFramework->pVideoDriver->PopRT();


	pFramework->pVideoDriver->PushRT(ShadowAccumPass);
	pFramework->pVideoDriver->Clear();
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass,BaseDriver::DEPTH_ATTACHMENT), 0);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(DepthPass, BaseDriver::DEPTH_ATTACHMENT), 1);
	Quads[0].SetSignature(Signature::SHADOW_COMP_PASS);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();

	SceneProp.ActiveGaussKernel = SHADOW_KERNEL;
#if SEPARATED_BLUR
	pFramework->pVideoDriver->PushRT(ExtraHelperPass);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(ShadowAccumPass, BaseDriver::COLOR0_ATTACHMENT), 0);
	Quads[0].SetSignature(Signature::VERTICAL_BLUR_PASS);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();

	pFramework->pVideoDriver->PushRT(ShadowAccumPass);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(ExtraHelperPass, BaseDriver::COLOR0_ATTACHMENT), 0);
	Quads[0].SetSignature(Signature::HORIZONTAL_BLUR_PASS);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();
#else
	pFramework->pVideoDriver->PushRT(ExtraHelperPass);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(ShadowAccumPass, BaseDriver::COLOR0_ATTACHMENT), 0);
	Quads[0].SetSignature(Signature::ONE_PASS_BLUR);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();
#endif

	pFramework->pVideoDriver->PushRT(DeferredPass);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, BaseDriver::COLOR0_ATTACHMENT), 0);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, BaseDriver::COLOR1_ATTACHMENT), 1);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, BaseDriver::COLOR2_ATTACHMENT), 2);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, BaseDriver::COLOR3_ATTACHMENT), 3);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, BaseDriver::DEPTH_ATTACHMENT), 4);
#if SEPARATED_BLUR
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(ShadowAccumPass, BaseDriver::COLOR0_ATTACHMENT), 5);
#else
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(ExtraHelperPass, BaseDriver::COLOR0_ATTACHMENT), 5);
#endif
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetEnvironmentMap(g_pBaseDriver->GetTexture(EnvMapTexIndex));
	Quads[0].SetSignature(Signature::DEFERRED_PASS);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();

	pFramework->pVideoDriver->PushRT(BloomAccumPass);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(DeferredPass, BaseDriver::COLOR0_ATTACHMENT), 0);
	Quads[0].SetSignature(Signature::BRIGHT_PASS);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();

	SceneProp.ActiveGaussKernel = BLOOM_KERNEL;
#if SEPARATED_BLUR
	pFramework->pVideoDriver->PushRT(ExtraHelperPass);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(BloomAccumPass, BaseDriver::COLOR0_ATTACHMENT), 0);
	Quads[0].SetSignature(Signature::HORIZONTAL_BLUR_PASS);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();

	pFramework->pVideoDriver->PushRT(BloomAccumPass);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(ExtraHelperPass, BaseDriver::COLOR0_ATTACHMENT), 0);
	Quads[0].SetSignature(Signature::VERTICAL_BLUR_PASS);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();
#else
	pFramework->pVideoDriver->PushRT(ExtraHelperPass);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(BloomAccumPass, BaseDriver::COLOR0_ATTACHMENT), 0);
	Quads[0].SetSignature(Signature::ONE_PASS_BLUR);
	Quads[0].Draw();
	pFramework->pVideoDriver->PopRT();
#endif

	pFramework->pVideoDriver->Clear();
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, BaseDriver::DEPTH_ATTACHMENT),0);
	Quads[1].SetSignature(Signature::FSQUAD_1_TEX);
	Quads[1].Draw();

	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(ShadowAccumPass, BaseDriver::COLOR0_ATTACHMENT), 0);
	Quads[2].SetSignature(Signature::FSQUAD_1_TEX);
	Quads[2].Draw();

	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, BaseDriver::COLOR1_ATTACHMENT), 0);
	Quads[3].SetSignature(Signature::FSQUAD_1_TEX);
	Quads[3].Draw();

	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, BaseDriver::COLOR2_ATTACHMENT), 0);
	Quads[4].SetSignature(Signature::FSQUAD_1_TEX);
	Quads[4].Draw();

	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, BaseDriver::COLOR3_ATTACHMENT), 0);
	Quads[5].SetSignature(Signature::FSQUAD_1_TEX);
	Quads[5].Draw();

#if	SEPARATED_BLUR
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(BloomAccumPass, BaseDriver::COLOR0_ATTACHMENT), 0);
#else
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(ExtraHelperPass, BaseDriver::COLOR0_ATTACHMENT), 0);
#endif
	Quads[6].SetSignature(Signature::FSQUAD_1_TEX);
	Quads[6].Draw();

#if	SEPARATED_BLUR
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(DeferredPass, BaseDriver::COLOR0_ATTACHMENT), 0);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(BloomAccumPass, BaseDriver::COLOR0_ATTACHMENT), 1);
	Quads[7].SetSignature(Signature::FSQUAD_2_TEX);
	Quads[7].Draw();
#else
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(DeferredPass, BaseDriver::COLOR0_ATTACHMENT), 0);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(ExtraHelperPass, BaseDriver::COLOR0_ATTACHMENT), 1);
	Quads[7].SetSignature(Signature::FSQUAD_2_TEX);
	Quads[7].Draw();
#endif

	pFramework->pVideoDriver->SwapBuffers();

	FirstFrame = false;
}

void App::OnInput() {

	if (FirstFrame)
		return;

	bool changed = false;
	const float speedFactor = 10.0f;
	if (IManager.PressedKey(T800K_UP)) {
		Position.y += 1.0f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedKey(T800K_DOWN)) {
		Position.y -= 1.0f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedKey(T800K_LEFT)) {
		Position.x -= 1.0f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedKey(T800K_RIGHT)) {
		Position.x += 1.0f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedKey(T800K_z)) {
		Position.z -= 1.0f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedKey(T800K_x)) {
		Position.z += 1.0f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedOnceKey(T800K_KP_PLUS)) {
		SceneProp.ActiveLights *= 2;
	}

	if (IManager.PressedOnceKey(T800K_KP_MINUS)) {
		SceneProp.ActiveLights /= 2;
		if(SceneProp.ActiveLights<=0)
			SceneProp.ActiveLights = 1;
	}

	if (IManager.PressedKey(T800K_KP5)) {
		Orientation.x -= 60.0f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedKey(T800K_KP6)) {
		Orientation.x += 60.0f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedKey(T800K_KP2)) {
		Orientation.y -= 60.0f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedKey(T800K_KP3)) {
		Orientation.y += 60.0f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedKey(T800K_KP0)) {
		Orientation.z -= 60.0f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedKey(T800K_KP_PERIOD)) {
		Orientation.z += 60.0f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedKey(T800K_KP_PERIOD)) {
		Orientation.z += 60.0f*speedFactor*DtSecs;
		changed = true;
	}

	bool displayInfo = false;
	if (changed && displayInfo) {
		printf("Position[%f,%f,%f] Rot[%f,%f,%f] Sc[%f]\n",Position.x,Position.y,Position.z,Orientation.x,Orientation.y,Orientation.z,Scaling.x);
	}

	//printf("Position[%f, %f, %f]\n\n", Cam.Eye.x, Cam.Eye.y, Cam.Eye.z);
	//printf("Orientation[%f, %f, %f]\n\n", Cam.Pitch, Cam.Roll, Cam.Yaw);

	if (IManager.PressedOnceKey(T800K_c)) {
		if (ActiveCam == (&Cam)) {
			ActiveCam = &LightCam;			
		}
		else {
			ActiveCam = &Cam;
		}
		SceneProp.pCameras[0] = ActiveCam;
	}

	if (IManager.PressedKey(T800K_w)) {
		ActiveCam->MoveForward(DtSecs);
	}

	if (IManager.PressedKey(T800K_s)) {
		ActiveCam->MoveBackward(DtSecs);
	}

	if (IManager.PressedKey(T800K_a)) {
		ActiveCam->StrafeLeft(DtSecs);
	}

	if (IManager.PressedKey(T800K_d)) {
		ActiveCam->StrafeRight(DtSecs);
	}

	float yaw = 0.005f*static_cast<float>(IManager.xDelta);
	ActiveCam->MoveYaw(yaw);

	float pitch = 0.005f*static_cast<float>(IManager.yDelta);
	ActiveCam->MovePitch(pitch);

}

void App::OnPause() {

}

void App::OnResume() {

}

void App::OnReset() {

}
