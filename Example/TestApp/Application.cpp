#include "Application.h"


void TestApp::CreateAssets() {
	PrimitiveMgr.CreateTriangle();
}

void TestApp::DestroyAssets() {
	PrimitiveMgr.DestroyPrimitives();
}

void TestApp::OnUpdate(unsigned int dt) {

}

void TestApp::OnDraw() {
	pFramework->pVideoDriver->Clear();
	PrimitiveMgr.DrawPrimitives();
	pFramework->pVideoDriver->SwapBuffers();
}

void TestApp::OnInput() {

}

void TestApp::OnPause() {

}

void TestApp::OnResume() {

}

void TestApp::OnReset() {

}