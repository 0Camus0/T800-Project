#include <Win32Framework.h>
#include "Application.h"


AppBase		  *pApp = 0;
RootFramework *pFrameWork = 0;

int main(){
	pApp = new TestApp;
	pFrameWork = new Win32Framework((AppBase*)pApp);
	pFrameWork->InitGlobalVars();
	pFrameWork->OnCreateApplication();
	pFrameWork->UpdateApplication();
	pFrameWork->OnDestroyApplication();
	delete pFrameWork;
	delete pApp;

    return 0;
}

