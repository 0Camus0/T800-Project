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

#include "BasicApplication.h"
#include <video/BaseDriver.h>
#include <utils/InputManager.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SEPARATED_BLUR 1
#define DEGENERATED_FBO_TEST 0

#define NUM_LIGHTS 64
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
#include "utils\T8_Technique.h"
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
	FirstFrame = true;
}

void App::LoadAssets()
{
}

void App::CreateAssets() {

	EnvMapTexIndex = g_pBaseDriver->CreateTexture(string("CubeMap_Mountains.dds"));

	PrimitiveMgr.SetVP(&VP);

	int index = PrimitiveMgr.CreateMesh("Models/Scene.X");
	Pigs[0].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	PrimitiveMgr.GetPrimitive(index)->SetEnvironmentMap(g_pBaseDriver->GetTexture(EnvMapTexIndex));

	index = PrimitiveMgr.CreateMesh("Models/NuCroc.X");
	Pigs[1].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	PrimitiveMgr.GetPrimitive(index)->SetEnvironmentMap(g_pBaseDriver->GetTexture(EnvMapTexIndex));

  index = PrimitiveMgr.CreateSpline();
  splineWire = (SplineWireframe*)PrimitiveMgr.GetPrimitive(index);
  splineInst.CreateInstance(splineWire, &VP);

	PrimitiveMgr.SetSceneProps(&SceneProp);

  m_agent.m_pSpline = &splineWire->m_spline;
  m_agent.m_moving = true;
  m_agent.m_velocity = 15.0f;

  m_agent.m_actualPoint = splineWire->m_spline.GetPoint(splineWire->m_spline.GetNormalizedOffset(0));
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
  m_agent.Update(1/60.0f);
  Pigs[1].TranslateAbsolute(m_agent.m_actualPoint.x, m_agent.m_actualPoint.y, m_agent.m_actualPoint.z);
  Pigs[1].Update();
	OnDraw();
}

void App::OnDraw() {

  pFramework->pVideoDriver->Clear();
  SceneProp.pCameras[0] = &Cam;
  for (int i = 0; i < 2; i++) {
    Pigs[i].Draw();
  }
  splineInst.Draw();
  pFramework->pVideoDriver->SwapBuffers();
  FirstFrame = false;
}

void  App::ChangeSettingsOnPlus() {

}

void  App::ChangeSettingsOnMinus() {

}

void App::printCurrSelection() {
	
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
