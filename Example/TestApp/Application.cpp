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
	Position = XVECTOR3(-0.580896f, 7.552873f, -18.135574f);
	Orientation = XVECTOR3(0.0f, 0.0f, 0.0f);
	Scaling = XVECTOR3(0.1f, 0.1f, 0.1f);
	SelectedMesh = 0;

	Cam.Init(XVECTOR3(0.0f, 1.0f, 10.0f), Deg2Rad(45.0f), 1280.0f / 720.0f, 0.1f, 10000.0f);
	Cam.Speed = 100.0f;
	Cam.Eye = XVECTOR3(0.0f, 9.75f, -31.0f);
	Cam.Pitch = 0.14f;
	Cam.Roll = 0.0f;
	Cam.Yaw = 0.020f;
	Cam.Update(0.0f);

	SceneProp.AddCamera(&Cam);
	SceneProp.AddLight(XVECTOR3(0.0f, 0.0f, 0.0f), XVECTOR3(1.0f, 1.0f, 1.0f), true);
	SceneProp.AmbientColor = XVECTOR3(0.15f, 0.15f, 0.15f);

	FirstFrame = true;
}

void TestApp::CreateAssets() {
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

	index = PrimitiveMgr.CreateCube();
	Pigs[5].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	
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


	SceneProp.Lights[0].Position = Position;
	
	PrimitiveInst *Sel = &Pigs[5];
	Sel->TranslateAbsolute(Position.x, Position.y, Position.z);
	Sel->RotateXAbsolute(Orientation.x);
	Sel->RotateYAbsolute(Orientation.y);
	Sel->RotateZAbsolute(Orientation.z);
	Sel->ScaleAbsolute(Scaling.x);
	Sel->Update();
	
	OnDraw();
}

void TestApp::OnDraw() {
	pFramework->pVideoDriver->Clear();
	
	for (int i = 0; i < 6; i++) {
		Pigs[i].Draw();
	}
	
	pFramework->pVideoDriver->SwapBuffers();
	FirstFrame = false;
}

void TestApp::OnInput() {

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
	
	float yaw = 0.005f*static_cast<float>(IManager.xDelta);
	Cam.MoveYaw(yaw);

	float pitch = 0.005f*static_cast<float>(IManager.yDelta);
	Cam.MovePitch(pitch);


}

void TestApp::OnPause() {

}

void TestApp::OnResume() {

}

void TestApp::OnReset() {

}