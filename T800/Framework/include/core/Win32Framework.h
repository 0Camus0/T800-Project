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

#ifndef UAD_WIN32FRAMEWORK_H
#define UAD_WIN32FRAMEWORK_H

#include <Config.h>

#include <core\Core.h>
#include <video\BaseDriver.h>


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