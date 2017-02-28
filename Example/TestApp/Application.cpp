#include "Application.h"

void TestApp::InitVars() {
	DtTimer.Init();
	Position = XVECTOR3(0.0f, 0.0f, 0.0f);
	Orientation = XVECTOR3(0.0f, 0.0f, 0.0f);
	Scaling = XVECTOR3(1.0f, 1.0f, 1.0f);
	SelectedMesh = 0;
	Cam.Init(XVECTOR3(0.0f, 1.0f, -5.0f), Deg2Rad(45.0f), 1280.0f / 720.0f, 0.01f, 100000.0f);
	Cam.SetLookAt(XVECTOR3(0.0001f, 0.0001f, 0.0001f));
}

void TestApp::CreateAssets() {
	PrimitiveMgr.SetVP(&VP);

	int indexScene= PrimitiveMgr.CreateMesh("Models/Scene.X");
	Pigs[0].CreateInstance(PrimitiveMgr.GetPrimitive(indexScene), &VP);

	int indexCerdo = PrimitiveMgr.CreateMesh("Models/NuCroc.X");
	Pigs[1].CreateInstance(PrimitiveMgr.GetPrimitive(indexCerdo), &VP);
	
	UpdateVP();
}

void TestApp::UpdateVP() {
	XMATRIX44 View;
	XVECTOR3 Pos = XVECTOR3(0.0f, 1.0f, -5.0f);
	XVECTOR3 Up = XVECTOR3(0.0f, 1.0f, 0.0f);
	XVECTOR3 LookAt = XVECTOR3(0.0001f, 0.0001f, 0.0001f) - Pos;
	XMatViewLookAtLH(View, Pos, LookAt, Up);

	XMATRIX44 Proj;
	XMatPerspectiveLH(Proj, Deg2Rad(45.0f), 1280.0f / 720.0f, 0.01f, 100000.0f);
	VP = Proj;
}

void TestApp::DestroyAssets() {
	PrimitiveMgr.DestroyPrimitives();
}

void TestApp::OnUpdate() {
	DtTimer.Update();
	
	OnInput();

	Cam.Update(DtTimer.GetDTSecs());
	VP = Cam.VP;

	PrimitiveInst *Sel = &Pigs[1];
	Sel->TranslateAbsolute(Position.x, Position.y, Position.z);
	Sel->RotateXAbsolute(Orientation.x);
	Sel->RotateYAbsolute(Orientation.y);
	Sel->RotateZAbsolute(Orientation.z);
	Sel->ScaleAbsolute(Orientation.z);
	Sel->ScaleAbsolute(Scaling.x);
	Sel->Update();

	OnDraw();
}

void TestApp::OnDraw() {
	pFramework->pVideoDriver->Clear();
	
	Pigs[0].Draw();
	Pigs[1].Draw();

	pFramework->pVideoDriver->SwapBuffers();
}

void TestApp::OnInput() {
	const float speedFactor = 10.0f;
	if (IManager.PressedKey(SDLK_UP)) {
		Position.y += 1.0f*speedFactor*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_DOWN)) {
		Position.y -= 1.0f*speedFactor*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_LEFT)) {
		Position.x -= 1.0f*speedFactor*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_RIGHT)) {
		Position.x += 1.0f*speedFactor*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_z)) {
		Position.z -= 1.0f*speedFactor*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_x)) {
		Position.z += 1.0f*speedFactor*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP_PLUS)) {
		Scaling.x += 0.1f*speedFactor*DtTimer.GetDTSecs();
		Scaling.y += 0.1f*speedFactor*DtTimer.GetDTSecs();
		Scaling.z += 0.1f*speedFactor*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP_MINUS)) {
		Scaling.x -= 0.1f*speedFactor*DtTimer.GetDTSecs();
		Scaling.y -= 0.1f*speedFactor*DtTimer.GetDTSecs();
		Scaling.z -= 0.1f*speedFactor*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP5)) {
		Orientation.x -= 60.0f*speedFactor*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP6)) {
		Orientation.x += 60.0f*speedFactor*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP2)) {
		Orientation.y -= 60.0f*speedFactor*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP3)) {
		Orientation.y += 60.0f*speedFactor*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP0)) {
		Orientation.z -= 60.0f*speedFactor*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP_PERIOD)) {
		Orientation.z += 60.0f*speedFactor*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP_PERIOD)) {
		Orientation.z += 60.0f*speedFactor*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_w)) {
		Cam.MoveForward(DtTimer.GetDTSecs());
	}

	if (IManager.PressedKey(SDLK_s)) {
		Cam.MoveBackward(DtTimer.GetDTSecs());
	}

	if (IManager.PressedKey(SDLK_a)) {
		Cam.StrafeLeft(DtTimer.GetDTSecs());
	}

	if (IManager.PressedKey(SDLK_d)) {
		Cam.StrafeRight(DtTimer.GetDTSecs());
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