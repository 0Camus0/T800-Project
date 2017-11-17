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

#include <core/Core.h>

#include <scene/PrimitiveManager.h>
#include <scene/PrimitiveInstance.h>
#include <scene/SceneProp.h>

#include <utils/xMaths.h>
#include <utils/Camera.h>
#include <utils/Timer.h>


enum {
	DRAW_CUBE_SPINNING = 0,
	DRAW_CUBE_BIG,
	DRAW_MESH,
	DRAW_ALL,
};

enum{
	SHADOW_KERNEL = 0,
	BLOOM_KERNEL = 1  
};

enum {
	CHANGE_EXPOSURE = 0,
	CHANGE_BLOOM_FACTOR,
	CHANGE_NUM_LIGHTS,
	CHANGE_ACTIVE_GAUSS_KERNEL,
	CHANGE_GAUSS_KERNEL_SAMPLE_COUNT,
	CHANGE_GAUSS_KERNEL_RADIUS,
	CHANGE_GAUSS_KERNEL_DEVIATION,
	CHANGE_MAX_NUM_OPTIONS
};

class App : public t800::AppBase {
public:
	App() : AppBase() {}
	void InitVars();
	void CreateAssets();
	void DestroyAssets();

	void OnUpdate();
	void OnDraw();
	void OnInput();

	void OnPause();
	void OnResume();

	void OnReset();


	void ChangeSettingsOnPlus();
	void ChangeSettingsOnMinus();
	void printCurrSelection();

	t800::PrimitiveManager PrimitiveMgr;
	t800::PrimitiveInst	Cubes[10];
	t800::PrimitiveInst	Triangles[10];
	t800::PrimitiveInst   Pigs[10];
	t800::PrimitiveInst	QuadInst;

  t800::PrimitiveInst	Quads[10];

	Camera			Cam;
	Camera			LightCam;

	GaussFilter		ShadowFilter;
	GaussFilter		BloomFilter;

	Camera			*ActiveCam;

	XVECTOR3		Position;
	XVECTOR3		Orientation;
	XVECTOR3		Scaling;

	XMATRIX44		View;
	XMATRIX44		Projection;
	XMATRIX44		VP;

	SceneProps		SceneProp;

	bool			FirstFrame;
	Timer			DtTimer;
	float			DtSecs;
	int				SelectedMesh;
	int				RTIndex;
	int				QuadIndex;
	int				GBufferPass;
	int				DeferredPass;
	int				DepthPass;
	int				ShadowAccumPass;
	int				BloomAccumPass;
	int				ExtraHelperPass;
	//int				

	int				EnvMapTexIndex;

	enum {
		NORMAL_CAM1 = 0,
		LIGHT_CAM1,
		MAX_CAMS
	};
	int				CamSelection;
	int				SceneSettingSelection;
	int				ChangeActiveGaussSelection;
};
