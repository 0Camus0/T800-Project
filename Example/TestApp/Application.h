#include <Core.h>
#include <PrimitiveManager.h>

class TestApp : public AppBase {
public:
	TestApp() : AppBase() {}
	void CreateAssets();
	void DestroyAssets();

	void OnUpdate(unsigned int dt);
	void OnDraw();
	void OnInput();

	void OnPause();
	void OnResume();

	void OnReset();

	PrimitiveManager PrimitiveMgr;
};