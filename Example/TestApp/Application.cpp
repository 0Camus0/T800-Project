#include "Application.h"

void TestApp::InitVars() {
	DtTimer.Init();
	Position = XVECTOR3(0.0f, 0.0f, 0.0f);
	Orientation = XVECTOR3(0.0f, 0.0f, 0.0f);
	Scaling = XVECTOR3(1.0f, 1.0f, 1.0f);
	SelectedMesh = 0;

}

void TestApp::CreateAssets() {
	PrimitiveMgr.SetVP(&VP);
	int indexCube = PrimitiveMgr.CreateCube();
	Cubes[0].CreateInstance(PrimitiveMgr.GetPrimitive(indexCube), &VP);
	/*Cubes[1].CreateInstance(PrimitiveMgr.GetPrimitive(indexCube), &VP);
	int indexTriangle = PrimitiveMgr.CreateTriangle();
	Triangles[0].CreateInstance(PrimitiveMgr.GetPrimitive(indexTriangle), &VP);
	int indexCerdo = PrimitiveMgr.CreateMesh("NuCroc.X");
	Pigs[0].CreateInstance(PrimitiveMgr.GetPrimitive(indexCerdo), &VP);

	Cubes[0].SetVisible(false);
	Cubes[1].SetVisible(true);
	Pigs[0].SetVisible(false);

	*/
	UpdateVP();
}

void TestApp::UpdateVP() {
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

	PrimitiveInst *Sel = &Cubes[0];
	Sel->TranslateAbsolute(Position.x, Position.y, Position.z);
	Sel->RotateXAbsolute(Orientation.x);
	Sel->RotateYAbsolute(Orientation.y);
	Sel->RotateZAbsolute(Orientation.z);
	Sel->ScaleAbsolute(Orientation.z);
	Sel->ScaleAbsolute(Scaling.x);
	Sel->Update();
/*
	PrimitiveInst *Sel = &Cubes[0];
	switch (SelectedMesh){
		case DRAW_CUBE_BIG:{
			Sel = &Cubes[0];
		}break;
		case DRAW_MESH:{
			Sel = &Pigs[0];
		}break;
	}

	Sel->TranslateAbsolute(Position.x, Position.y, Position.z);
	Sel->RotateXAbsolute(Orientation.x);
	Sel->RotateYAbsolute(Orientation.y);
	Sel->RotateZAbsolute(Orientation.z);
	Sel->ScaleAbsolute(Orientation.z);
	Sel->ScaleAbsolute(Scaling.x);
	Sel->Update();

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
	*/
	OnDraw();
}

void TestApp::OnDraw() {
	pFramework->pVideoDriver->Clear();
	
	Cubes[0].Draw();
	/*Cubes[1].Draw();
	Pigs[0].Draw();

	*/
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
/*
	if (IManager.PressedOnceKey(SDLK_SPACE)) {
		SelectedMesh++;
		if (SelectedMesh > DRAW_ALL)
			SelectedMesh = 0;

		switch (SelectedMesh) {
			case DRAW_CUBE_SPINNING:{
				Cubes[0].SetVisible(false);
				Cubes[1].SetVisible(true);
				Pigs[0].SetVisible(false);
			}break;
			case DRAW_CUBE_BIG:{
				Cubes[0].SetVisible(true);
				Cubes[1].SetVisible(false);
				Pigs[0].SetVisible(false);
				Position = XVECTOR3(0.0f, 0.0f, 0.0f);
				Orientation = XVECTOR3(0.0f, 0.0f, 0.0f);
				Scaling = XVECTOR3(1.0f, 1.0f, 1.0f);
			}break;
			case DRAW_MESH:{
				Cubes[0].SetVisible(false);
				Cubes[1].SetVisible(false);
				Pigs[0].SetVisible(true);
				Position = XVECTOR3(0.0f, -2.0f, 0.0f);
				Orientation = XVECTOR3(0.0f, 0.0f, 0.0f);
				Scaling = XVECTOR3(0.045f, 0.045f, 0.045f);
			}break;
			case DRAW_ALL:{
				Cubes[0].SetVisible(true);
				Cubes[1].SetVisible(true);
				Pigs[0].SetVisible(true);
			}break;
		}
	}

	*/
	
}

void TestApp::OnPause() {

}

void TestApp::OnResume() {

}

void TestApp::OnReset() {

}