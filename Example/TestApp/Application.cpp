#include "Application.h"

void TestApp::InitVars() {
	DtTimer.Init();
	Position	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Orientation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Scaling		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

void TestApp::CreateAssets() {	
	PrimitiveMgr.CreateTriangle();
}

void TestApp::DestroyAssets() {
	PrimitiveMgr.DestroyPrimitives();
}

void TestApp::OnUpdate() {
	DtTimer.Update();

	OnInput();
	
	D3DXMATRIX WorldTranslate,WorldScale,WorldRotate;
	D3DXMatrixTranslation(&WorldTranslate, Position.x, Position.y, Position.z);
	D3DXMatrixScaling(&WorldScale, Scaling.x, Scaling.y, Scaling.z);
	D3DXMatrixRotationZ(&WorldRotate, Orientation.z);
	WorldTransform = WorldScale*WorldRotate*WorldTranslate;

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