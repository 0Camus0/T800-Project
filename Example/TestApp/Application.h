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

	void UpdateVP();

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

	Timer			DtTimer;
	int				SelectedMesh;
};