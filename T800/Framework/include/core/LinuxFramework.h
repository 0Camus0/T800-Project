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

#ifndef LINUXFRAMEWORK_H_INCLUDED
#define LINUXFRAMEWORK_H_INCLUDED


#include <Config.h>

#include <core/Core.h>
#include <video/BaseDriver.h>

#include <GL/freeglut.h>

#include <memory>

class LinuxFramework : public RootFramework {
public:
	LinuxFramework(AppBase *pBaseApp);
	void InitGlobalVars();
	void OnCreateApplication();
	void OnDestroyApplication();
	void OnInterruptApplication();
	void OnResumeApplication();
	void UpdateApplication();
	void ProcessInput();
	void ResetApplication();
	~LinuxFramework() {	}

	static void IdleFunction();
    static void MouseClickFunction(int button, int state, int x, int y);
    static void MouseMoveFunction(int x, int y);
    static void ResizeWindow(int w, int h);
    static void KeyboardEvent(unsigned char key, int x, int y);
    static void KeyboardReleaseEvent(unsigned char key, int x, int y);

	bool	m_alive;

	static LinuxFramework* thiz;
};


#endif // LINUXFRAMEWORK_H_INCLUDED
