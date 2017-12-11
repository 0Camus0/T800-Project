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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SEPARATED_BLUR 1
#define DEGENERATED_FBO_TEST 0

#define NUM_LIGHTS 1
#define RADI 170.0f

#if defined(OS_LINUX)
#include <sys/time.h>
#endif

#include <iostream>
#include <string>
#include <vector>

using namespace t800;
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
#include "utils/T8_Technique.h"
void App::InitVars() {
  t800::T8Technique tech("Techniques/test_technique.xml");


	DtTimer.Init();
	DtTimer.Update();
	srand((unsigned int)DtTimer.GetDTSecs());


	Position = XVECTOR3(0.0f, 0.0f, 0.0f);
	Orientation = XVECTOR3(0.0f, 0.0f, 0.0f);
	Scaling = XVECTOR3(1.0f,1.0f,1.0f);
	SelectedMesh = 0;

	CamSelection = NORMAL_CAM1;
	SceneSettingSelection = CHANGE_EXPOSURE;

	Cam.Init(XVECTOR3(0.0f, 1.0f, 10.0f), Deg2Rad(45.0f), 1280.0f / 720.0f, 1.0f, 8000.0f);
	Cam.Speed = 10.0f;
	Cam.Eye = XVECTOR3(0.0f, 9.75f, -31.0f);
	Cam.Pitch = 0.14f;
	Cam.Roll = 0.0f;
	Cam.Yaw = 0.020f;
	Cam.Update(0.0f);

	LightCam.Init(XVECTOR3(0.0f, 100.0f, 10.0f), Deg2Rad(60.0f), 1.0f, 0.1f, 8000.0f);
	LightCam.Speed = 10.0f;
	LightCam.Eye = XVECTOR3(0.0f, 25.0f, -40.0f);
	LightCam.Pitch = 0.14f;
	LightCam.Roll = 0.0f;
	LightCam.Yaw = 0.020f;
	LightCam.Update(0.0f);

	ActiveCam = &Cam;

	SceneProp.AddCamera(ActiveCam);
	SceneProp.AddLightCamera(&LightCam);

  SceneProp.AddLight(LightCam.Eye, XVECTOR3(1, 1, 1), true);

	SceneProp.ActiveLights = 1;
	SceneProp.AmbientColor = XVECTOR3(0.8f, 0.8f, 0.8f);

	ShadowFilter.kernelSize = 7;
	ShadowFilter.radius = 1.0f;
	ShadowFilter.sigma = 1.0f;
	ShadowFilter.Update();

	BloomFilter.kernelSize = 5;
	BloomFilter.radius = 2.0f;
	BloomFilter.sigma = 1.0f;
	BloomFilter.Update();

	SceneProp.AddGaussKernel(&ShadowFilter);
	SceneProp.AddGaussKernel(&BloomFilter);
	SceneProp.ActiveGaussKernel = SHADOW_KERNEL;
	ChangeActiveGaussSelection = SHADOW_KERNEL;

	RTIndex = -1;
	FirstFrame = true;

  m_spline.m_points.push_back(SplinePoint(0, 3, 0));
  m_spline.m_points.back().m_velocity = 6.0f;
  m_spline.m_points.push_back(SplinePoint(0, 3, 0));
  m_spline.m_points.back().m_velocity = 6.f;
  m_spline.m_points.push_back(SplinePoint(8, 6, 0));
  m_spline.m_points.back().m_velocity = 9;
  m_spline.m_points.push_back(SplinePoint(40, 6, 0));
  m_spline.m_points.back().m_velocity = 9;
  m_spline.m_points.push_back(SplinePoint(50, 6, 20));
  m_spline.m_points.back().m_velocity = 9;
  m_spline.m_points.push_back(SplinePoint(40, 3, 20));
  m_spline.m_points.back().m_velocity = 9;
  m_spline.m_points.push_back(SplinePoint(0, 3, 20));
  m_spline.m_points.back().m_velocity = 9;
  m_spline.m_points.push_back(SplinePoint(-50, 3, 20));
  m_spline.m_points.back().m_velocity = 45;
  m_spline.m_points.push_back(SplinePoint(-50, 6, 0));
  m_spline.m_points.back().m_velocity = 45;
  m_spline.m_points.push_back(SplinePoint(20, 6, 0));
  m_spline.m_points.back().m_velocity = 45;
  m_spline.m_points.push_back(SplinePoint(50, 15, 0));
  m_spline.m_points.back().m_velocity = 45;
  m_spline.m_points.push_back(SplinePoint(50, 15, -20));
  m_spline.m_points.back().m_velocity = 45;
  m_spline.m_points.push_back(SplinePoint(-50, 15, -20));
  m_spline.m_points.back().m_velocity = 45;
  m_spline.m_points.push_back(SplinePoint(-50, 10, 0));
  m_spline.m_points.back().m_velocity = 45;
  m_spline.m_points.push_back(SplinePoint(20, 15, 0));
  m_spline.m_points.back().m_velocity = 45;
  m_spline.m_points.push_back(SplinePoint(50, 40, 5));
  m_spline.m_points.back().m_velocity = 45;
  m_spline.m_points.push_back(SplinePoint(50, 30, -20));
  m_spline.m_points.back().m_velocity = 45;
  m_spline.m_points.push_back(SplinePoint(-50, 30, -20));
  m_spline.m_points.back().m_velocity = 20;
  m_spline.m_points.push_back(SplinePoint(-50, 30, 0));
  m_spline.m_points.back().m_velocity = 20;
  m_spline.m_points.push_back(SplinePoint(-30, 30, 0));
  m_spline.m_points.back().m_velocity = 20;
  m_spline.m_points.push_back(SplinePoint(0, 5, 0));
  m_spline.m_points.back().m_velocity = 20;

  m_spline.m_looped = false;
  m_spline.Init();

  m_agent.m_pSpline = &m_spline;
  m_agent.m_moving = true;
  m_agent.m_velocity = 15.0f;
}

void App::LoadAssets()
{
}

void App::CreateAssets() {
  PrimitiveMgr.Init();
  //m_flare.Init(PrimitiveMgr);

	EnvMapTexIndex = g_pBaseDriver->CreateTexture(string("CubeMap_Mountains.dds"));

	GBufferPass = pFramework->pVideoDriver->CreateRT(4, BaseRT::RGBA8, BaseRT::F32, 0, 0);
	DeferredPass = pFramework->pVideoDriver->CreateRT(1, BaseRT::RGBA32F, BaseRT::F32, 0, 0,true);
	DepthPass = pFramework->pVideoDriver->CreateRT(0, BaseRT::NOTHING, BaseRT::F32, 512, 512);
	ShadowAccumPass = pFramework->pVideoDriver->CreateRT(1, BaseRT::RGBA8, BaseRT::F32, 0, 0);
	ExtraHelperPass = pFramework->pVideoDriver->CreateRT(1, BaseRT::RGBA8, BaseRT::F32, 0, 0);
	BloomAccumPass = pFramework->pVideoDriver->CreateRT(1, BaseRT::RGBA8, BaseRT::F32, 0, 0);

	PrimitiveMgr.SetVP(&VP);

	int index = PrimitiveMgr.CreateMesh("Models/Scene.X");
	Pigs[0].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
  Pigs[0].TranslateAbsolute(0.0, -10.0f, 0.0f);
  Pigs[0].Update();

	index = PrimitiveMgr.CreateMesh("Models/NuCroc.X");
	Pigs[1].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

    index = PrimitiveMgr.CreateMesh("Models/NuBatman.X");
	Pigs[2].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	index = PrimitiveMgr.CreateMesh("Models/NuVenomJok.X");
	Pigs[3].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	index = PrimitiveMgr.CreateMesh("Models/CerdoNuevo.X");
	Pigs[4].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	Pigs[5].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
  Pigs[5].SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[0], 0);
  //index = PrimitiveMgr.CreateMesh("Models/SponzaEsc.X");
  //Pigs[6].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
  //PrimitiveMgr.GetPrimitive(index)->SetEnvironmentMap(g_pBaseDriver->GetTexture(EnvMapTexIndex));

  index = PrimitiveMgr.CreateSpline(m_spline);
  splineWire = (SplineWireframe*)PrimitiveMgr.GetPrimitive(index);
  splineInst.CreateInstance(splineWire, &VP);


	Quads[0].CreateInstance(PrimitiveMgr.GetPrimitive(PrimitiveManager::QUAD), &VP);
	Quads[0].SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[0], 0);
	Quads[0].SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[1], 1);
	Quads[0].SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[2], 2);
	Quads[0].SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[3], 3);
	Quads[0].SetTexture(pFramework->pVideoDriver->RTs[0]->pDepthTexture,4);
  Quads[0].SetEnvironmentMap(g_pBaseDriver->GetTexture(EnvMapTexIndex));

	Quads[1].CreateInstance(PrimitiveMgr.GetPrimitive(PrimitiveManager::QUAD), &VP);
	Quads[2].CreateInstance(PrimitiveMgr.GetPrimitive(PrimitiveManager::QUAD), &VP);
	Quads[3].CreateInstance(PrimitiveMgr.GetPrimitive(PrimitiveManager::QUAD), &VP);
	Quads[4].CreateInstance(PrimitiveMgr.GetPrimitive(PrimitiveManager::QUAD), &VP);
	Quads[5].CreateInstance(PrimitiveMgr.GetPrimitive(PrimitiveManager::QUAD), &VP);
	Quads[6].CreateInstance(PrimitiveMgr.GetPrimitive(PrimitiveManager::QUAD), &VP);
	Quads[7].CreateInstance(PrimitiveMgr.GetPrimitive(PrimitiveManager::QUAD), &VP);

	PrimitiveMgr.SetSceneProps(&SceneProp);

  m_agent.m_actualPoint = m_spline.GetPoint(m_spline.GetNormalizedOffset(0));
  //ActiveCam->AttachAgent(m_agent);

	for(unsigned int i=0;i<g_args.size();i++){
        if(g_args[i] == "-l"){
           SceneProp.ActiveLights = atoi( g_args[i+1].c_str() );
        }
	}

  /* POSITIONS */
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

  Quads[0].TranslateAbsolute(0.0f, 0.0f, 0.0f);
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
}

void App::DestroyAssets() {
	PrimitiveMgr.DestroyPrimitives();
}

void App::OnUpdate() {
	DtTimer.Update();
	DtSecs = DtTimer.GetDTSecs();
	OnInput();

  if (FirstFrame) {
    DtSecs = 1.0f / 60.0f;
  }
  m_agent.Update(DtSecs);
	ActiveCam->Update(DtSecs);
	VP = ActiveCam->VP;


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

  pFramework->pVideoDriver->Clear();

  pFramework->pVideoDriver->PushRT(DepthPass);
  SceneProp.pCameras[0] = &LightCam;
  for (int i = 0; i < 6; i++) {
    Pigs[i].SetGlobalSignature(Signature::SHADOW_MAP_PASS);
    Pigs[i].Draw();
    Pigs[i].SetGlobalSignature(Signature::FORWARD_PASS);
  }
  pFramework->pVideoDriver->PopRT();

  SceneProp.pCameras[0] = &Cam;

  pFramework->pVideoDriver->PushRT(GBufferPass);
  for (int i = 0; i < 6; i++) {
    Pigs[i].SetGlobalSignature(Signature::GBUFF_PASS);
    Pigs[i].Draw();
    Pigs[i].SetGlobalSignature(Signature::FORWARD_PASS);
  }
  pFramework->pVideoDriver->PopRT();


  pFramework->pVideoDriver->PushRT(ShadowAccumPass);
  pFramework->pVideoDriver->Clear();
  Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, BaseDriver::DEPTH_ATTACHMENT), 0);
  Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(DepthPass, BaseDriver::DEPTH_ATTACHMENT), 1);
  Quads[0].SetGlobalSignature(Signature::SHADOW_COMP_PASS);
  Quads[0].Draw();
  pFramework->pVideoDriver->PopRT();

  SceneProp.ActiveGaussKernel = SHADOW_KERNEL;
#if SEPARATED_BLUR
  pFramework->pVideoDriver->PushRT(ExtraHelperPass);
  Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(ShadowAccumPass, BaseDriver::COLOR0_ATTACHMENT), 0);
  Quads[0].SetGlobalSignature(Signature::VERTICAL_BLUR_PASS);
  Quads[0].Draw();
  pFramework->pVideoDriver->PopRT();

  pFramework->pVideoDriver->PushRT(ShadowAccumPass);
  Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(ExtraHelperPass, BaseDriver::COLOR0_ATTACHMENT), 0);
  Quads[0].SetGlobalSignature(Signature::HORIZONTAL_BLUR_PASS);
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
  Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, BaseDriver::COLOR0_ATTACHMENT), 0);
  Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, BaseDriver::COLOR1_ATTACHMENT), 1);
  Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, BaseDriver::COLOR2_ATTACHMENT), 2);
  Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, BaseDriver::COLOR3_ATTACHMENT), 3);
  Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, BaseDriver::DEPTH_ATTACHMENT), 4);
#if SEPARATED_BLUR
  Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(ShadowAccumPass, BaseDriver::COLOR0_ATTACHMENT), 5);
#else
  PrimitiveMgr.GetPrimitive(PrimitiveManager::QUAD)->SetTexture(pFramework->pVideoDriver->GetRTTexture(ExtraHelperPass, BaseDriver::COLOR0_ATTACHMENT), 5);
#endif
  Quads[0].SetEnvironmentMap(g_pBaseDriver->GetTexture(EnvMapTexIndex));
  Quads[0].SetGlobalSignature(Signature::DEFERRED_PASS);
  Quads[0].Draw();
  pFramework->pVideoDriver->PopRT();

  pFramework->pVideoDriver->PushRT(BloomAccumPass);
  Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(DeferredPass, BaseDriver::COLOR0_ATTACHMENT), 0);
  Quads[0].SetGlobalSignature(Signature::BRIGHT_PASS);
  Quads[0].Draw();
  pFramework->pVideoDriver->PopRT();

  SceneProp.ActiveGaussKernel = BLOOM_KERNEL;
#if SEPARATED_BLUR
  pFramework->pVideoDriver->PushRT(ExtraHelperPass);
  Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(BloomAccumPass, BaseDriver::COLOR0_ATTACHMENT), 0);
  Quads[0].SetGlobalSignature(Signature::HORIZONTAL_BLUR_PASS);
  Quads[0].Draw();
  pFramework->pVideoDriver->PopRT();

  pFramework->pVideoDriver->PushRT(BloomAccumPass);
  Quads[0].SetTexture(pFramework->pVideoDriver->GetRTTexture(ExtraHelperPass, BaseDriver::COLOR0_ATTACHMENT), 0);
  Quads[0].SetGlobalSignature(Signature::VERTICAL_BLUR_PASS);
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
  Quads[1].SetTexture(pFramework->pVideoDriver->GetRTTexture(DeferredPass, BaseDriver::COLOR0_ATTACHMENT), 0);
  Quads[1].SetGlobalSignature(Signature::HDR_COMP_PASS);
  Quads[1].Draw();

  Quads[2].SetTexture(pFramework->pVideoDriver->GetRTTexture(ShadowAccumPass, BaseDriver::COLOR0_ATTACHMENT), 0);
  Quads[2].SetGlobalSignature(Signature::FSQUAD_1_TEX);
  Quads[2].Draw();

  Quads[3].SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, BaseDriver::COLOR1_ATTACHMENT), 0);
  Quads[3].SetGlobalSignature(Signature::FSQUAD_1_TEX);
  Quads[3].Draw();

  Quads[4].SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, BaseDriver::COLOR2_ATTACHMENT), 0);
  Quads[4].SetGlobalSignature(Signature::FSQUAD_1_TEX);
  Quads[4].Draw();

  Quads[5].SetTexture(pFramework->pVideoDriver->GetRTTexture(GBufferPass, BaseDriver::COLOR3_ATTACHMENT), 0);
  Quads[5].SetGlobalSignature(Signature::FSQUAD_1_TEX);
  Quads[5].Draw();

#if	SEPARATED_BLUR
  Quads[6].SetTexture(pFramework->pVideoDriver->GetRTTexture(BloomAccumPass, BaseDriver::COLOR0_ATTACHMENT), 0);
#else
  PrimitiveMgr.GetPrimitive(PrimitiveManager::QUAD)->SetTexture(pFramework->pVideoDriver->GetRTTexture(ExtraHelperPass, BaseDriver::COLOR0_ATTACHMENT), 0);
#endif
  Quads[6].SetGlobalSignature(Signature::FSQUAD_1_TEX);
  Quads[6].Draw();

#if	SEPARATED_BLUR
  Quads[7].SetTexture(pFramework->pVideoDriver->GetRTTexture(DeferredPass, BaseDriver::COLOR0_ATTACHMENT), 0);
  Quads[7].SetTexture(pFramework->pVideoDriver->GetRTTexture(BloomAccumPass, BaseDriver::COLOR0_ATTACHMENT), 1);
  Quads[7].SetGlobalSignature(Signature::HDR_COMP_PASS);
  Quads[7].Draw();
#else
  PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(DeferredPass, BaseDriver::COLOR0_ATTACHMENT), 0);
  PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->GetRTTexture(ExtraHelperPass, BaseDriver::COLOR0_ATTACHMENT), 1);
  Quads[7].SetSignature(Signature::HDR_COMP_PASS);
  Quads[7].Draw();
#endif

  //m_flare.Draw();

  pFramework->pVideoDriver->SwapBuffers();

  FirstFrame = false;
}

void  App::ChangeSettingsOnPlus() {
	switch (SceneSettingSelection) {
		case CHANGE_EXPOSURE: {
			float prevVal = SceneProp.Exposure;
			SceneProp.Exposure += 0.1f;
			cout << "[CHANGE_EXPOSURE] Previous Value[" << prevVal << "] Actual Value[" << SceneProp.Exposure << "]" << endl;
		}break;
		case CHANGE_BLOOM_FACTOR: {
			float prevVal = SceneProp.BloomFactor;
			SceneProp.BloomFactor += 0.1f;
			cout << "[CHANGE_BLOOM_FACTOR] Previous Value[" << prevVal << "] Actual Value[" << SceneProp.BloomFactor << "]" << endl;
		}break;
		case CHANGE_NUM_LIGHTS: {			
			int prevVal = SceneProp.ActiveLights;
			SceneProp.ActiveLights *= 2;
			if (SceneProp.ActiveLights >= 127) {
				SceneProp.ActiveLights = 127;
			}
			cout << "[CHANGE_NUM_LIGHTS] Previous Value[" << prevVal << "] Actual Value[" << SceneProp.ActiveLights << "]" << endl;
		}break;
		case CHANGE_ACTIVE_GAUSS_KERNEL: {
			int prevVal = ChangeActiveGaussSelection;
			ChangeActiveGaussSelection++;
			if (ChangeActiveGaussSelection >= (int)SceneProp.pGaussKernels.size()) {
				ChangeActiveGaussSelection = SceneProp.pGaussKernels.size() - 1;
			}
			cout << "[CHANGE_ACTIVE_GAUSS_KERNEL] Previous Value[" << prevVal << "] Actual Value[" << ChangeActiveGaussSelection << "]" << endl;
		}break;
		case CHANGE_GAUSS_KERNEL_SAMPLE_COUNT: {
			int prevVal = SceneProp.pGaussKernels[ChangeActiveGaussSelection]->kernelSize;
			SceneProp.pGaussKernels[ChangeActiveGaussSelection]->kernelSize += 2;
			SceneProp.pGaussKernels[ChangeActiveGaussSelection]->Update();
			cout << "[CHANGE_GAUSS_KERNEL_SAMPLE_COUNT] Previous Value[" << prevVal << "] Actual Value[" << SceneProp.pGaussKernels[ChangeActiveGaussSelection]->kernelSize << "]" << endl;
		}break;
		case CHANGE_GAUSS_KERNEL_RADIUS: {
			float prevVal = SceneProp.pGaussKernels[ChangeActiveGaussSelection]->radius;
			SceneProp.pGaussKernels[ChangeActiveGaussSelection]->radius += 0.5;
			SceneProp.pGaussKernels[ChangeActiveGaussSelection]->Update();
			cout << "[CHANGE_GAUSS_KERNEL_RADIUS] Previous Value[" << prevVal << "] Actual Value[" << SceneProp.pGaussKernels[ChangeActiveGaussSelection]->radius << "]" << endl;
		}break;
		case CHANGE_GAUSS_KERNEL_DEVIATION: {
			float prevVal = SceneProp.pGaussKernels[ChangeActiveGaussSelection]->sigma;
			SceneProp.pGaussKernels[ChangeActiveGaussSelection]->sigma += 0.5;
			SceneProp.pGaussKernels[ChangeActiveGaussSelection]->Update();
			cout << "[CHANGE_GAUSS_KERNEL_DEVIATION] Previous Value[" << prevVal << "] Actual Value[" << SceneProp.pGaussKernels[ChangeActiveGaussSelection]->sigma << "]" << endl;
		}break;
	}
}

void  App::ChangeSettingsOnMinus() {
	switch (SceneSettingSelection) {
		case CHANGE_EXPOSURE: {
			float prevVal = SceneProp.Exposure;
			SceneProp.Exposure -= 0.1f;
			cout << "[CHANGE_EXPOSURE] Previous Value[" << prevVal << "] Actual Value[" << SceneProp.Exposure << "]" << endl;
		}break;
		case CHANGE_BLOOM_FACTOR: {
			float prevVal = SceneProp.BloomFactor;
			SceneProp.BloomFactor -= 0.1f;
			cout << "[CHANGE_BLOOM_FACTOR] Previous Value[" << prevVal << "] Actual Value[" << SceneProp.BloomFactor << "]" << endl;
		}break;
		case CHANGE_NUM_LIGHTS: {
			int prevVal = SceneProp.ActiveLights;
			SceneProp.ActiveLights /= 2;
			if (SceneProp.ActiveLights <= 0) {
				SceneProp.ActiveLights = 1;
			}
			cout << "[CHANGE_NUM_LIGHTS] Previous Value[" << prevVal << "] Actual Value[" << SceneProp.ActiveLights << "]" << endl;
		}break;
		case CHANGE_ACTIVE_GAUSS_KERNEL: {
			int prevVal = ChangeActiveGaussSelection;
			ChangeActiveGaussSelection--;
			if (ChangeActiveGaussSelection < 0) {
				ChangeActiveGaussSelection = 0;
			}
			cout << "[CHANGE_ACTIVE_GAUSS_KERNEL] Previous Value[" << prevVal << "] Actual Value[" << ChangeActiveGaussSelection << "]" << endl;
		}break;
		case CHANGE_GAUSS_KERNEL_SAMPLE_COUNT: {
			int prevVal = SceneProp.pGaussKernels[ChangeActiveGaussSelection]->kernelSize;
			SceneProp.pGaussKernels[ChangeActiveGaussSelection]->kernelSize -= 2;
			if (SceneProp.pGaussKernels[ChangeActiveGaussSelection]->kernelSize <= 2) {
				SceneProp.pGaussKernels[ChangeActiveGaussSelection]->kernelSize = 3;
			}
			SceneProp.pGaussKernels[ChangeActiveGaussSelection]->Update();
			cout << "[CHANGE_GAUSS_KERNEL_SAMPLE_COUNT] Previous Value[" << prevVal << "] Actual Value[" << SceneProp.pGaussKernels[ChangeActiveGaussSelection]->kernelSize << "]" << endl;
		}break;
		case CHANGE_GAUSS_KERNEL_RADIUS: {
			float prevVal = SceneProp.pGaussKernels[ChangeActiveGaussSelection]->radius;
			SceneProp.pGaussKernels[ChangeActiveGaussSelection]->radius -= 0.5f;
			if (SceneProp.pGaussKernels[ChangeActiveGaussSelection]->radius <= 0.5f) {
				SceneProp.pGaussKernels[ChangeActiveGaussSelection]->radius = 0.5f;
			}
			SceneProp.pGaussKernels[ChangeActiveGaussSelection]->Update();
			cout << "[CHANGE_GAUSS_KERNEL_RADIUS] Previous Value[" << prevVal << "] Actual Value[" << SceneProp.pGaussKernels[ChangeActiveGaussSelection]->radius << "]" << endl;
		}break;
		case CHANGE_GAUSS_KERNEL_DEVIATION: {
			float prevVal = SceneProp.pGaussKernels[ChangeActiveGaussSelection]->sigma;
			SceneProp.pGaussKernels[ChangeActiveGaussSelection]->sigma -= 0.5;
			SceneProp.pGaussKernels[ChangeActiveGaussSelection]->Update();
			cout << "[CHANGE_GAUSS_KERNEL_DEVIATION] Previous Value[" << prevVal << "] Actual Value[" << SceneProp.pGaussKernels[ChangeActiveGaussSelection]->sigma << "]" << endl;
		}break;
	}
}

void App::printCurrSelection() {
	switch (SceneSettingSelection) {
	case CHANGE_EXPOSURE: {
		cout << "Option[CHANGE_EXPOSURE] Value[" << SceneProp.Exposure << "]" << endl;
	}break;
	case CHANGE_BLOOM_FACTOR: {
		cout << "Option[CHANGE_BLOOM_FACTOR] Value[" << SceneProp.BloomFactor << "]" << endl;
	}break;
	case CHANGE_NUM_LIGHTS: {
		cout << "Option[CHANGE_NUM_LIGHTS] Value[" << SceneProp.ActiveLights << "]" << endl;
	}break;
	case CHANGE_ACTIVE_GAUSS_KERNEL: {
		cout << "Option[CHANGE_ACTIVE_GAUSS_KERNEL] Value[" << ChangeActiveGaussSelection << "]" << endl;
	}break;
	case CHANGE_GAUSS_KERNEL_SAMPLE_COUNT: {
		cout << "Option[CHANGE_GAUSS_KERNEL_SAMPLE_COUNT] Value[" << SceneProp.pGaussKernels[ChangeActiveGaussSelection]->kernelSize << "]" << endl;
	}break;
	case CHANGE_GAUSS_KERNEL_RADIUS: {
		cout << "Option[CHANGE_GAUSS_KERNEL_RADIUS] Value[" << SceneProp.pGaussKernels[ChangeActiveGaussSelection]->radius << "]" << endl;
	}break;
	case CHANGE_GAUSS_KERNEL_DEVIATION: {
		cout << "Option[CHANGE_GAUSS_KERNEL_DEVIATION] Value[" << SceneProp.pGaussKernels[ChangeActiveGaussSelection]->sigma << "]" << endl;
	}break;
	}
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
		ChangeSettingsOnPlus();	
	}

	if (IManager.PressedOnceKey(T800K_KP_MINUS)) {

		ChangeSettingsOnMinus();		
	}

	if (IManager.PressedOnceKey(T800K_b)) {
		SceneSettingSelection--;
		if (SceneSettingSelection < 0) {
			SceneSettingSelection = CHANGE_MAX_NUM_OPTIONS - 1;
		}

		printCurrSelection();
	}

	if (IManager.PressedOnceKey(T800K_n)) {
		SceneSettingSelection++;
		if(SceneSettingSelection == CHANGE_MAX_NUM_OPTIONS) {
			SceneSettingSelection = CHANGE_EXPOSURE;
		}

		printCurrSelection();
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

  if (IManager.PressedOnceKey(T800K_1)) {
    pFramework->ChangeAPI(GRAPHICS_API::D3D11);
  }

  if (IManager.PressedOnceKey(T800K_2)) {
    pFramework->ChangeAPI(GRAPHICS_API::OPENGL);
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
