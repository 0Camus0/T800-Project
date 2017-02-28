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

#include <Core.h>
#include <PrimitiveManager.h>
#include <PrimitiveInstance.h>

#include <xMaths.h>
#include <xCamera.h>

#include <Timer.h>


enum {
	DRAW_CUBE_SPINNING = 0,
	DRAW_CUBE_BIG,
	DRAW_MESH,
	DRAW_ALL,
};

class TestApp : public AppBase {
public:
	TestApp() : AppBase() {}
	void InitVars();
	void CreateAssets();
	void DestroyAssets();

	void OnUpdate();
	void OnDraw();
	void OnInput();

	void OnPause();
	void OnResume();

	void OnReset();

	PrimitiveManager PrimitiveMgr;
	PrimitiveInst	Cubes[10];
	PrimitiveInst	Triangles[10];
	PrimitiveInst   Pigs[10];

	xCamera			Cam;

	XVECTOR3		Position;
	XVECTOR3		Orientation;
	XVECTOR3		Scaling;

	XMATRIX44		View;
	XMATRIX44		Projection;
	XMATRIX44		VP;
	
	bool			FirstFrame;
	Timer			DtTimer;
	float			DtSecs;
	int				SelectedMesh;
};