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



void App::InitVars() {
	DtTimer.Init();
	Position = XVECTOR3(0.0f, 0.0f, 0.0f);
	Orientation = XVECTOR3(0.0f, 0.0f, 0.0f);
	Scaling = XVECTOR3(1.0f, 1.0f, 1.0f);

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

void App::CreateAssets() {
	PrimitiveMgr.SetVP(&VP);
	int index = PrimitiveMgr.CreateTriangle();
	Cubes.CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);	
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

	
	PrimitiveInst *Sel = &Cubes;
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
	Cubes.Draw();	
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

void App::OnPause() {

}

void App::OnResume() {

}

void App::OnReset() {

}