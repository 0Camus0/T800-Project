#ifndef UAD_WIN32FRAMEWORK_H
#define UAD_WIN32FRAMEWORK_H

#include "Core.h"
#include "BaseDriver.h"
#include <memory>

class Win32Framework : public RootFramework {
public:
	Win32Framework(AppBase *pBaseApp) : RootFramework(pBaseApp) {}
	void InitGlobalVars();
	void OnCreateApplication();
	void OnDestroyApplication();
	void OnInterruptApplication();
	void OnResumeApplication();
	void UpdateApplication();
	void ProcessInput();
	void ResetApplication();
	~Win32Framework() {	}
};

#endif