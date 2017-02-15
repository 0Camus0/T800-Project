#include "Application.h"

void TestApp::InitVars() {
	DtTimer.Init();
	Position	= XVECTOR3(0.0f, -2.0f, 0.0f);
	Orientation = XVECTOR3(0.0f, 0.0f, 0.0f);
	Scaling		= XVECTOR3(0.045f, 0.045f, 0.045f);
}

void TestApp::CreateAssets() {
	PrimitiveMgr.SetVP(&VP);
	int indexCube = PrimitiveMgr.CreateCube();
	Cubes[0].CreateInstance(PrimitiveMgr.GetPrimitive(indexCube), &VP);
	Cubes[1].CreateInstance(PrimitiveMgr.GetPrimitive(indexCube), &VP);
	int indexTriangle = PrimitiveMgr.CreateTriangle();
	Triangles[0].CreateInstance(PrimitiveMgr.GetPrimitive(indexTriangle), &VP);
	int indexCerdo = PrimitiveMgr.CreateMesh("NuCroc.X");
	Pigs[0].CreateInstance(PrimitiveMgr.GetPrimitive(indexCerdo), &VP);

	XMATRIX44 View;
	XVECTOR3 Pos = XVECTOR3(0.0f, 1.0f, -5.0f);
	XVECTOR3 Up = XVECTOR3(0.0f, 1.0f, 0.0f);
	XVECTOR3 LookAt = XVECTOR3(0.0001f, 0.0001f, 0.0001f) - Pos;
	XMatViewLookAtLH(View, Pos, LookAt, Up);

	XMATRIX44 Proj;
	XMatPerspectiveLH(Proj, Deg2Rad(45.0f), 1280.0f / 720.0f, 0.01f, 10000.0f);
	VP = View*Proj;

}

void TestApp::DestroyAssets() {
	PrimitiveMgr.DestroyPrimitives();
}

void TestApp::OnUpdate() {
	DtTimer.Update();

	OnInput();


	Cubes[0].TranslateAbsolute(Position.x, Position.y, Position.z);
	Cubes[0].RotateXAbsolute(Orientation.x);
	Cubes[0].RotateYAbsolute(Orientation.y);
	Cubes[0].RotateZAbsolute(Orientation.z);
	Cubes[0].ScaleAbsolute(0.25f);
	Cubes[0].Update();
	

	Pigs[0].TranslateAbsolute(Position.x, Position.y, Position.z);
	Pigs[0].RotateXAbsolute(Orientation.x);
	Pigs[0].RotateYAbsolute(Orientation.y);
	Pigs[0].RotateZAbsolute(Orientation.z);
	Pigs[0].ScaleAbsolute(Scaling.x);
	Pigs[0].Update();


	static float freq = DtTimer.GetDTSecs();
	freq += 10.0f*DtTimer.GetDTSecs();

	if (freq > 36.0f)
		freq = 0.0f;

	Cubes[1].TranslateAbsolute(1.5f*exp(0.1f*-freq)*sin(freq), 1.5f*exp(0.1f*-freq)*cos(freq), 0.0f);
	Cubes[1].RotateXRelative(180.0f*DtTimer.GetDTSecs());
	Cubes[1].RotateYRelative(180.0f*DtTimer.GetDTSecs());
	Cubes[1].RotateZRelative(180.0f*DtTimer.GetDTSecs());
	Cubes[1].ScaleAbsolute(0.15f);
	Cubes[1].Update();

	Triangles[0].Update();
	
	OnDraw();
}

void TestApp::OnDraw() {
	pFramework->pVideoDriver->Clear();
	
	//Cubes[0].Draw();
	Cubes[1].Draw();

	Pigs[0].Draw();
//	Triangles[0].Draw();

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

	
}

void TestApp::OnPause() {

}

void TestApp::OnResume() {

}

void TestApp::OnReset() {

}