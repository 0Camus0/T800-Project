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

#include <Config.h>
#ifdef OS_LINUX
    #include <core/LinuxFramework.h>
#elif defined(OS_WINDOWS)
	#include <core/Win32Framework.h>
#endif

#include "Application.h"

#define CIL_DDS_DEBUG 1

#if CIL_DDS_DEBUG
#include <utils/cil.h>
#endif

#include <iostream>
#include <string>
#include <vector>


std::vector<std::string> g_args;

AppBase		  *pApp = 0;
RootFramework *pFrameWork = 0;

int main(int arg,char ** args){
#if CIL_DDS_DEBUG
	std::string fpa = "Textures/CubeMap_Granja_RAW.dds";
	int x, y;
	unsigned int props, bsize;
	unsigned char m;
	unsigned char *buf = cil_load(fpa.c_str(), &x, &y, &m, &props, &bsize);
#endif
    for(int i=0;i<arg;i++){
        g_args.push_back( std::string( args[i] ) );
    }

	pApp = new App;
#ifdef OS_LINUX
    pFrameWork = new LinuxFramework((AppBase*)pApp);
    pFrameWork->InitGlobalVars();
	pFrameWork->OnCreateApplication();
#elif defined(OS_WINDOWS)
	pFrameWork = new Win32Framework((AppBase*)pApp);
	pFrameWork->InitGlobalVars();
	pFrameWork->OnCreateApplication();
	pFrameWork->UpdateApplication();
	pFrameWork->OnDestroyApplication();
#endif

	delete pFrameWork;
	delete pApp;

    return 0;
}

