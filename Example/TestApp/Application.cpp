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

void TestApp::InitVars() {
	DtTimer.Init();
	Position = XVECTOR3(0.0f, 0.05f, 0.0f);
	Orientation = XVECTOR3(0.0f, 0.0f, 0.0f);
	Scaling = XVECTOR3(0.01f, 0.01f, 0.01f);
	SelectedMesh = 0;
	/*
	Cam.Init(XVECTOR3(0.0f, 1.0f, 10.0f), Deg2Rad(45.0f), 1280.0f / 720.0f, 1.0f, 10000.0f);
	Cam.Speed = 2500.0f;
	Cam.Eye = XVECTOR3(-18.804996f, 38.739578f, -250.309113f);
	Cam.Pitch = 0.060000f;
	Cam.Roll  = 0.0f;
	Cam.Yaw = 0.02000f;
	*/
	Cam.Init(XVECTOR3(0.0f, 1.0f, 10.0f), Deg2Rad(45.0f), 1280.0f / 720.0f, 0.1f, 10000.0f);
	Cam.Speed = 250.0f;
	Cam.Eye = XVECTOR3(0.0f, 0.5f, -1.8f);
	Cam.Pitch = 0.0f;
	Cam.Roll = 0.0f;
	Cam.Yaw = 0.0f;
	Cam.Update(0.0f);

	SceneProp.AddCamera(&Cam);
	SceneProp.AddLight(XVECTOR3(0.0f, 0.0f, 0.0f), XVECTOR3(1.0f, 1.0f, 1.0f), true);
	SceneProp.AmbientColor = XVECTOR3(0.15f, 0.15f, 0.15f);

	FirstFrame = true;
}

void TestApp::CreateAssets() {
	PrimitiveMgr.SetVP(&VP);

	int index = PrimitiveMgr.CreateMesh("Models/NuCroc.X");
	Pigs[0].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	index = PrimitiveMgr.CreateCube();
	Pigs[1].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	/*
	int index = PrimitiveMgr.CreateMesh("Models/Scene.X");
	Pigs[0].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	
	index = PrimitiveMgr.CreateMesh("Models/NuCroc.X");
	Pigs[1].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	

	index = PrimitiveMgr.CreateMesh("Models/Link.X");
	Pigs[2].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	index = PrimitiveMgr.CreateMesh("Models/House.X");
	Pigs[3].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	Pigs[4].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	index = PrimitiveMgr.CreateMesh("Models/Drone.X");
	Pigs[5].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	index = PrimitiveMgr.CreateMesh("Models/Cerdo.X");
	Pigs[6].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	Pigs[7].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	*/
	PrimitiveMgr.SetSceneProps(&SceneProp);

}

void TestApp::DestroyAssets() {
	PrimitiveMgr.DestroyPrimitives();
}

void TestApp::OnUpdate() {
	DtTimer.Update();
	DtSecs = DtTimer.GetDTSecs();
	OnInput();

	Cam.Update(DtSecs);
	VP = Cam.VP;

	Pigs[0].TranslateAbsolute(0.0f, 0.0f, 0.0f);
	Pigs[0].RotateXAbsolute(0.0f);
	Pigs[0].RotateYAbsolute(0.0f);
	Pigs[0].RotateXAbsolute(0.0f);
	Pigs[0].ScaleAbsolute(0.015f);
	Pigs[0].Update();

	/*
	Pigs[CROC].TranslateAbsolute(-43.871941f, 0.064795f, -58.153839f);
	Pigs[CROC].RotateXAbsolute(0.0f);
	Pigs[CROC].RotateYAbsolute(207.731613f);
	Pigs[CROC].RotateXAbsolute(0.0f);
	Pigs[CROC].ScaleAbsolute(0.151794f);
	Pigs[CROC].Update();
	
	Pigs[LINK].TranslateAbsolute(12.499269f, -0.079694f, -63.019135f);
	Pigs[LINK].RotateXAbsolute(0.0f);
	Pigs[LINK].RotateYAbsolute(-410.563721f);
	Pigs[LINK].RotateXAbsolute(0.0f);
	Pigs[LINK].ScaleAbsolute(0.114174f);
	Pigs[LINK].Update();

	Pigs[HOUSE_L].TranslateAbsolute(56.104416f, 0.574123f, 29.808973f);
	Pigs[HOUSE_L].RotateXAbsolute(0.0f);
	Pigs[HOUSE_L].RotateYAbsolute(141.33f);
	Pigs[HOUSE_L].RotateXAbsolute(0.0f);
	Pigs[HOUSE_L].ScaleAbsolute(0.670580f);
	Pigs[HOUSE_L].Update();

	Pigs[HOUSE_R].TranslateAbsolute(-82.823868f, 0.443788f, 34.599747f);
	Pigs[HOUSE_R].RotateXAbsolute(0.0f);
	Pigs[HOUSE_R].RotateYAbsolute(51.43101f);
	Pigs[HOUSE_R].RotateXAbsolute(0.0f);
	Pigs[HOUSE_R].ScaleAbsolute(0.670580f);
	Pigs[HOUSE_R].Update();

	static float xp = 0.0f, yp = 0.0f, zp = 0.0f, freq = 3.1415f, ampl = 180.0f;
	xp =  ampl * sin(freq);
	zp =  ampl * cos(freq);
	yp = 10.0f * sin(freq*10.0f);
	
	Pigs[DRONE].TranslateAbsolute(xp, yp + 10.0f,zp);
	Pigs[DRONE].RotateXAbsolute(0.0f);
	Pigs[DRONE].RotateYAbsolute(Rad2Deg(freq));
	Pigs[DRONE].RotateXAbsolute(0.0f);
	Pigs[DRONE].ScaleAbsolute(0.670580f);
	Pigs[DRONE].Update();

	freq += DtSecs;

	Pigs[CERDO].TranslateAbsolute(-14.064236f, -3.514139f, -29.351925f);
	Pigs[CERDO].RotateXAbsolute(0.0f);
	Pigs[CERDO].RotateYAbsolute(0.0f);
	Pigs[CERDO].RotateXAbsolute(0.0f);
	Pigs[CERDO].ScaleAbsolute(27.208776f);
	Pigs[CERDO].Update();
	*/

	SceneProp.Lights[0].Position = Position;
	
	PrimitiveInst *Sel = &Pigs[1];
	Sel->TranslateAbsolute(Position.x, Position.y, Position.z);
	Sel->RotateXAbsolute(Orientation.x);
	Sel->RotateYAbsolute(Orientation.y);
	Sel->RotateZAbsolute(Orientation.z);
	Sel->ScaleAbsolute(Scaling.x);
	Sel->Update();
	
/*
	PrimitiveInst *Sel = &Pigs[DRONE];
	Sel->TranslateAbsolute(Position.x, Position.y, Position.z);
	Sel->RotateXAbsolute(Orientation.x);
	Sel->RotateYAbsolute(Orientation.y);
	Sel->RotateZAbsolute(Orientation.z);
	Sel->ScaleAbsolute(Scaling.x);
	Sel->Update();
*/
	OnDraw();
}

void TestApp::OnDraw() {
	pFramework->pVideoDriver->Clear();
	/*
	for (int i = 0; i < TOTAL_INSTANCES; i++) {
		Pigs[i].Draw();
	}
	*/
	Pigs[0].Draw();
	Pigs[1].Draw();

	pFramework->pVideoDriver->SwapBuffers();
	FirstFrame = false;
}

void TestApp::OnInput() {

	if (FirstFrame)
		return;

	bool changed = false;
	const float speedFactor = 1.0f;
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

	if (IManager.PressedKey(SDLK_KP_PLUS)) {
		Scaling.x += 0.1f*speedFactor*DtSecs;
		Scaling.y += 0.1f*speedFactor*DtSecs;
		Scaling.z += 0.1f*speedFactor*DtSecs;
		changed = true;
	}

	if (IManager.PressedKey(SDLK_KP_MINUS)) {
		Scaling.x -= 0.1f*speedFactor*DtSecs;
		Scaling.y -= 0.1f*speedFactor*DtSecs;
		Scaling.z -= 0.1f*speedFactor*DtSecs;
		changed = true;
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
	
	float yaw = 0.01f*static_cast<float>(IManager.xDelta);
	Cam.MoveYaw(yaw);

	float pitch = 0.01f*static_cast<float>(IManager.yDelta);
	Cam.MovePitch(pitch);


}

void TestApp::OnPause() {

}

void TestApp::OnResume() {

}

void TestApp::OnReset() {

}