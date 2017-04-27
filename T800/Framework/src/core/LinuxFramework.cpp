#include <video/GLDriver.h>
#include <core/LinuxFramework.h>

LinuxFramework *LinuxFramework::thiz = 0;

LinuxFramework::LinuxFramework(AppBase *pBaseApp) : RootFramework(pBaseApp), m_alive(true) 	{
		pBaseApp->SetParentFramework(this);
		LinuxFramework::thiz = this;
}

void LinuxFramework::InitGlobalVars(){

}

void LinuxFramework::OnCreateApplication(){
    int req = 0;
    glutInit(&req,0);
    glutInitContextProfile(GLUT_CORE_PROFILE);
#ifdef USING_OPENGL_ES30
    glutInitContextVersion(3,0);
#elif defined(USING_OPENGL_ES31)
    glutInitContextVersion(3,1);
#elif defined(USING_OPENGL_ES20)
    glutInitContextVersion(2,0);
#endif
    int width = 640;
	int height = 360;

    glutInitWindowSize(width,height);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutCreateWindow("T800");
    glutDisplayFunc(IdleFunction);
    glutIdleFunc(IdleFunction);
    glutMouseFunc(MouseClickFunction);
    glutMotionFunc(MouseMoveFunction);
    glutReshapeFunc(ResizeWindow);
    glutKeyboardFunc(KeyboardEvent);
    glutKeyboardUpFunc(KeyboardReleaseEvent);

    pVideoDriver = new GLDriver;
    pVideoDriver->SetDimensions(width, height);

    g_pBaseDriver = pVideoDriver;

	pVideoDriver->SetWindow(0);
	pVideoDriver->InitDriver();

	pBaseApp->InitVars();
	pBaseApp->CreateAssets();

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
}

void LinuxFramework::MouseClickFunction(int button, int state, int x, int y){

}

void LinuxFramework::MouseMoveFunction(int x, int y){

}

void LinuxFramework::ResizeWindow(int w, int h){

}

void LinuxFramework::KeyboardEvent(unsigned char key, int x, int y){

}

void LinuxFramework::KeyboardReleaseEvent(unsigned char key, int x, int y){

}
