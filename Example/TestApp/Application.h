#include <Core.h>
#include <PrimitiveManager.h>
#include <PrimitiveInstance.h>

#include <xMaths.h>
#include <Timer.h>

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

	XVECTOR3		Position;
	XVECTOR3		Orientation;
	XVECTOR3		Scaling;

	XMATRIX44		View;
	XMATRIX44		Projection;
	XMATRIX44		VP;

	Timer			DtTimer;
};