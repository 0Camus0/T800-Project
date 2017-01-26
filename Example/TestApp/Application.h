#include <Core.h>
#include <PrimitiveManager.h>

#include <d3dx9math.h>
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

	D3DXVECTOR3		Position;
	D3DXVECTOR3		Orientation;
	D3DXVECTOR3		Scaling;
	D3DXMATRIX		WorldTransform;

	Timer			DtTimer;
};