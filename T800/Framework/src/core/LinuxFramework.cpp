#include <video/GLDriver.h>
#include <core/LinuxFramework.h>
#include <stdio.h>

#include <sys/time.h>



LinuxFramework *LinuxFramework::thiz = 0;

LinuxFramework::LinuxFramework(AppBase *pBaseApp) : RootFramework(pBaseApp), m_alive(true) 	{
		pBaseApp->SetParentFramework(this);
		LinuxFramework::thiz = this;
}

void LinuxFramework::InitGlobalVars(){

}

extern int g_AvoidInput;


#include <iostream>
#include <string>
#include <vector>


extern std::vector<std::string> g_args;

void LinuxFramework::OnCreateApplication(){
    int req = 0;
    g_AvoidInput = 0;
    putenv( (char *) "__GL_SYNC_TO_VBLANK=1" );
    glutInit(&req,0);
    glutInitContextProfile(GLUT_CORE_PROFILE);
#ifdef USING_OPENGL_ES30
    glutInitContextVersion(3,0);
#elif defined(USING_OPENGL_ES31)
    glutInitContextVersion(3,1);
#elif defined(USING_OPENGL_ES20)
    glutInitContextVersion(2,0);
#endif
    int width = 1280;
	int height = 720;

	for(unsigned int i=0;i<g_args.size();i++){
        if(g_args[i] == "-x"){
           width = atoi( g_args[i+1].c_str() );
        }

        if(g_args[i] == "-y"){
           height = atoi( g_args[i+1].c_str() );
        }
	}

    glutInitWindowSize(width,height);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutCreateWindow("T800");
    glutDisplayFunc(IdleFunction);
    glutIdleFunc(IdleFunction);
    glutMouseFunc(MouseClickFunction);
    glutPassiveMotionFunc(MouseMoveFunction);
    glutReshapeFunc(ResizeWindow);
    glutKeyboardFunc(KeyboardEvent);
    glutKeyboardUpFunc(KeyboardReleaseEvent);

    pVideoDriver = new GLDriver;
    pVideoDriver->SetDimensions(width, height);

    g_pBaseDriver = pVideoDriver;

	pVideoDriver->SetWindow(0);
	pVideoDriver->InitDriver();

	timeval start;
    gettimeofday(&start,0);


	pBaseApp->InitVars();
	pBaseApp->CreateAssets();

	timeval actual;
	gettimeofday(&actual,0);
	double ttaken = double( (actual.tv_sec - start.tv_sec) + (actual.tv_usec - start.tv_usec)/1000000.0);

	printf("Asset Loading took : %f \n",ttaken);

	glutMainLoop();
}

void LinuxFramework::OnDestroyApplication(){

}

void LinuxFramework::OnInterruptApplication(){

}

void LinuxFramework::OnResumeApplication(){

}

void LinuxFramework::UpdateApplication(){
    pBaseApp->OnUpdate();
}

void LinuxFramework::ProcessInput(){

}

void LinuxFramework::ResetApplication(){

}

// STATIC
void LinuxFramework::IdleFunction(){
    thiz->UpdateApplication();
    thiz->pBaseApp->IManager.xDelta = 0;
    thiz->pBaseApp->IManager.yDelta = 0;
}

void LinuxFramework::MouseClickFunction(int button, int state, int x, int y){

}

void LinuxFramework::MouseMoveFunction(int x, int y){
    static int xDelta = 0;
	static int yDelta = 0;

	xDelta = x - xDelta;
	yDelta = y - yDelta;

	thiz->pBaseApp->IManager.xDelta = xDelta;
	thiz->pBaseApp->IManager.yDelta = yDelta;

	xDelta = x;
	yDelta = y;
}

void LinuxFramework::ResizeWindow(int w, int h){

}

void LinuxFramework::KeyboardEvent(unsigned char key, int x, int y){
printf("key %d \n",key);
	thiz->pBaseApp->IManager.KeyStates[0][key] = true;
}

void LinuxFramework::KeyboardReleaseEvent(unsigned char key, int x, int y){
	thiz->pBaseApp->IManager.KeyStates[0][key] = false;
	thiz->pBaseApp->IManager.KeyStates[1][key] = false;
}
