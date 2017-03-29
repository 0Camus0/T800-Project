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

#include <core\Win32Framework.h>
#include "Application.h"


AppBase		  *pApp = 0;
RootFramework *pFrameWork = 0;

int main(){
	pApp = new App;
	pFrameWork = new Win32Framework((AppBase*)pApp);
	pFrameWork->InitGlobalVars();
	pFrameWork->OnCreateApplication();
	pFrameWork->UpdateApplication();
	pFrameWork->OnDestroyApplication();
	delete pFrameWork;
	delete pApp;

    return 0;
}

