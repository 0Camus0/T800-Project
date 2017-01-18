#ifndef UAD_WIN32FRAMEWORK_H
#define UAD_WIN32FRAMEWORK_H

#include "Core.h"
#include "BaseDriver.h"
#include <memory>

class Win32Framework : public RootFramework {
public:
	Win32Framework(AppBase *pBaseApp) : RootFramework(pBaseApp), m_alive(true) 	{ 
		pBaseApp->SetParentFramework(this);
	}
	void InitGlobalVars();
	void OnCreateApplication();
	void OnDestroyApplication();
	void OnInterruptApplication();
	void OnResumeApplication();
	void UpdateApplication();
	void ProcessInput();
	void ResetApplication();
	~Win32Framework() {	}

	bool	m_alive;
};

#endif