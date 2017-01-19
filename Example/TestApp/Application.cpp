#include "Application.h"


void TestApp::CreateAssets() {
	PrimitiveMgr.CreateTriangle();
}

void TestApp::DestroyAssets() {

}

void TestApp::OnUpdate(unsigned int dt) {

}

void TestApp::OnDraw() {
	pFramework->pVideoDriver->Clear();

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