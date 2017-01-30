#include "Application.h"

void TestApp::InitVars() {
	DtTimer.Init();
	Position	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Orientation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Scaling		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

void TestApp::CreateAssets() {	
	PrimitiveMgr.CreateCube();
//	PrimitiveMgr.CreateTriangle();
}

void TestApp::DestroyAssets() {
	PrimitiveMgr.DestroyPrimitives();
}

void TestApp::OnUpdate() {
	DtTimer.Update();

	OnInput();

	D3DXMATRIX WorldTranslate,WorldScale,WorldRotation,WorldRotateX, WorldRotateY, WorldRotateZ;
	D3DXMatrixTranslation(&WorldTranslate, Position.x, Position.y, Position.z);
	D3DXMatrixScaling(&WorldScale, Scaling.x, Scaling.y, Scaling.z);
	D3DXMatrixRotationX(&WorldRotateX, Orientation.x);
	D3DXMatrixRotationY(&WorldRotateY, Orientation.y);
	D3DXMatrixRotationZ(&WorldRotateZ, Orientation.z);
	WorldRotation= WorldRotateZ*WorldRotateY*WorldRotateX;
	WorldTransform = WorldScale*WorldRotation*WorldTranslate;

	PrimitiveMgr.TransformPrimitive(0, &WorldTransform.m[0][0]);

	OnDraw();
}

void TestApp::OnDraw() {
	pFramework->pVideoDriver->Clear();
	PrimitiveMgr.DrawPrimitives();
	pFramework->pVideoDriver->SwapBuffers();
}

void TestApp::OnInput() {
	
	if (IManager.PressedKey(SDLK_UP)) {
		Position.y += 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_DOWN)) {
		Position.y -= 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_LEFT)) {
		Position.x -= 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_RIGHT)) {
		Position.x += 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_z)) {
		Position.z -= 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_x)) {
		Position.z += 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP_PLUS)) {
		Scaling.x += 1.0f*DtTimer.GetDTSecs();
		Scaling.y += 1.0f*DtTimer.GetDTSecs();
		Scaling.z += 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP_MINUS)) {
		Scaling.x -= 1.0f*DtTimer.GetDTSecs();
		Scaling.y -= 1.0f*DtTimer.GetDTSecs();
		Scaling.z -= 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP5)) {
		Orientation.x -= 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP6)) {
		Orientation.x += 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP2)) {
		Orientation.y -= 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP3)) {
		Orientation.y += 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP0)) {
		Orientation.z -= 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP_PERIOD)) {
		Orientation.z += 1.0f*DtTimer.GetDTSecs();
	}

	
}

void TestApp::OnPause() {

}

void TestApp::OnResume() {

}

void TestApp::OnReset() {

}