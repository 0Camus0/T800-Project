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

#include <video/GLDriver.h>
#include <video/GLTexture.h>
#include <video/GLRT.h>
#include <video/GLShader.h>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <fstream>

#ifdef OS_WINDOWS
#if defined(USING_OPENGL_ES20)
#pragma comment(lib,"libEGL.lib")
#pragma comment(lib,"libGLESv2.lib")
#elif defined (USING_OPENGL_ES30) || defined (USING_OPENGL_ES31)
#pragma comment(lib,"libEGL.lib")
#pragma comment(lib,"libGLESv2.lib")
#elif defined(USING_OPENGL)
#pragma comment(lib,"glew.lib")
#pragma comment(lib,"OpenGL32.Lib")
#endif
#elif defined(OS_LINUX)
#include <GL/freeglut.h>
#endif

namespace t800 {
  extern Device*            T8Device;
  extern DeviceContext*     T8DeviceContext;
  void * GLDeviceContext::GetAPIObject() const
  {
    return nullptr;
  }
  void ** GLDeviceContext::GetAPIObjectReference() const
  {
    return nullptr;
  }
  void GLDeviceContext::release()
  {
    delete this;
  }
  void GLDeviceContext::SetPrimitiveTopology(T8_TOPOLOGY::E topology)
  {
    switch (topology)
    {
    case T8_TOPOLOGY::POINT_LIST:
      internalTopology = GL_POINTS;
      break;
    case T8_TOPOLOGY::LINE_LIST:
      internalTopology = GL_LINES;
      break;
    case T8_TOPOLOGY::LINE_STRIP:
      internalTopology = GL_LINE_STRIP;
      break;
    case T8_TOPOLOGY::TRIANLE_LIST:
      internalTopology = GL_TRIANGLES;
      break;
    case T8_TOPOLOGY::TRIANGLE_STRIP:
      internalTopology = GL_TRIANGLE_STRIP;
      break;
    default:
      internalTopology = GL_TRIANGLES;
      break;
    }
  }
  void GLDeviceContext::DrawIndexed(unsigned vertexCount, unsigned startIndex, unsigned startVertex)
  {
    glDrawElements(internalTopology, vertexCount, internalIBFormat, 0);
  }
  void * GLDevice::GetAPIObject() const
  {
    return nullptr;
  }
  void ** GLDevice::GetAPIObjectReference() const
  {
    return nullptr;
  }
  void GLDevice::release()
  {
    delete this;
  }
  Buffer * GLDevice::CreateBuffer(T8_BUFFER_TYPE::E bufferType, BufferDesc desc, void * initialData)
  {
    Buffer* retBuff;
    switch (bufferType)
    {
    case T8_BUFFER_TYPE::VERTEX:
      retBuff = new GLVertexBuffer;
      break;
    case T8_BUFFER_TYPE::INDEX:
      retBuff = new GLIndexBuffer;
      break;
    case T8_BUFFER_TYPE::CONSTANT:
      retBuff = new GLConstantBuffer;
      break;
    default:
      break;
    }
    retBuff->Create(*this, desc, initialData);
    return retBuff;
  }


  void * GLVertexBuffer::GetAPIObject() const
  {
    return nullptr;
  }

  void ** GLVertexBuffer::GetAPIObjectReference() const
  {
    return nullptr;
  }

  void GLVertexBuffer::Set(const DeviceContext & deviceContext, const unsigned stride, const unsigned offset)
  {
    const_cast<DeviceContext*>(&deviceContext)->actualVertexBuffer = (VertexBuffer*)this;
    reinterpret_cast<GLDeviceContext*>(const_cast<DeviceContext*>(&deviceContext))->internalStride = stride;
    glBindBuffer(GL_ARRAY_BUFFER, APIID);
  }
  void GLVertexBuffer::UpdateFromSystemCopy(const DeviceContext & deviceContext)
  {
    glBindBuffer(GL_ARRAY_BUFFER, APIID);
    glBufferData(GL_ARRAY_BUFFER, descriptor.byteWidth, &sysMemCpy[0], GL_STATIC_DRAW);
  }
  void GLVertexBuffer::UpdateFromBuffer(const DeviceContext & deviceContext, const void * buffer)
  {
    sysMemCpy.clear();
    sysMemCpy.assign((char*)buffer, (char*)buffer + descriptor.byteWidth);
    UpdateFromSystemCopy(deviceContext);
  }
  void GLVertexBuffer::release()
  {
    sysMemCpy.clear();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    delete this;
  }
  void GLVertexBuffer::Create(const Device & device, BufferDesc desc, void * initialData)
  {
    descriptor = desc;
    if (initialData) {
      sysMemCpy.assign((char*)initialData, (char*)initialData + desc.byteWidth);
    }
    glGenBuffers(1, &APIID);
    glBindBuffer(GL_ARRAY_BUFFER, APIID);
    glBufferData(GL_ARRAY_BUFFER, desc.byteWidth, initialData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }


  void * GLIndexBuffer::GetAPIObject() const
  {
    return nullptr;
  }

  void ** GLIndexBuffer::GetAPIObjectReference() const
  {
    return nullptr;
  }

  void GLIndexBuffer::Set(const DeviceContext & deviceContext, const unsigned offset, T8_IB_FORMAR::E format)
  {
    switch (format)
    {
    case T8_IB_FORMAR::R16:
      reinterpret_cast<GLDeviceContext*>(const_cast<DeviceContext*>(&deviceContext))->internalIBFormat = GL_UNSIGNED_SHORT;
      break;
    case T8_IB_FORMAR::R32:
      reinterpret_cast<GLDeviceContext*>(const_cast<DeviceContext*>(&deviceContext))->internalIBFormat = GL_UNSIGNED_INT;
      break;
    default:
      break;
    }
    const_cast<DeviceContext*>(&deviceContext)->actualIndexBuffer = (IndexBuffer*)this;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, APIID);
  }
  void GLIndexBuffer::UpdateFromSystemCopy(const DeviceContext & deviceContext)
  {
    glBindBuffer(GL_ARRAY_BUFFER, APIID);
    glBufferData(GL_ARRAY_BUFFER, descriptor.byteWidth, &sysMemCpy[0], GL_STATIC_DRAW);
  }
  void GLIndexBuffer::UpdateFromBuffer(const DeviceContext & deviceContext, const void * buffer)
  {
    sysMemCpy.clear();
    sysMemCpy.assign((char*)buffer, (char*)buffer + descriptor.byteWidth);
    UpdateFromSystemCopy(deviceContext);
  }
  void GLIndexBuffer::release()
  {
    sysMemCpy.clear();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    delete this;
  }
  void GLIndexBuffer::Create(const Device & device, BufferDesc desc, void * initialData)
  {
    descriptor = desc;
    if (initialData) {
      sysMemCpy.assign((char*)initialData, (char*)initialData + desc.byteWidth);
    }
    glGenBuffers(1, &APIID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, APIID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, desc.byteWidth, initialData, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }


  void * GLConstantBuffer::GetAPIObject() const
  {
    return nullptr;
  }

  void ** GLConstantBuffer::GetAPIObjectReference() const
  {
    return nullptr;
  }

  void GLConstantBuffer::Set(const DeviceContext & deviceContext)
  {
    const_cast<DeviceContext*>(&deviceContext)->actualConstantBuffer = (ConstantBuffer*)this;
    GLShader* sh = reinterpret_cast<GLShader*>(deviceContext.actualShaderSet);

    for (auto &it : sh->internalUniformsLocs) {
      switch (it.type)
      {
      case hyperspace::shader::datatype_::INT_:
        glUniform1i(it.loc, *reinterpret_cast<GLint*>(&sysMemCpy[it.bufferBytePosition]));
        break;
      case hyperspace::shader::datatype_::BOOLEAN_:
        glUniform1i(it.loc, *reinterpret_cast<GLint*>(&sysMemCpy[it.bufferBytePosition]));
        break;
      case hyperspace::shader::datatype_::FLOAT_:
        glUniform1f(it.loc, *reinterpret_cast<GLfloat*>(&sysMemCpy[it.bufferBytePosition]));
        break;
      case hyperspace::shader::datatype_::MAT2_:
        glUniformMatrix2fv(it.loc, it.num, GL_FALSE, reinterpret_cast<GLfloat*>(&sysMemCpy[it.bufferBytePosition]));
        break;
      case hyperspace::shader::datatype_::MAT3_:
        glUniformMatrix3fv(it.loc, it.num, GL_FALSE, reinterpret_cast<GLfloat*>(&sysMemCpy[it.bufferBytePosition]));
        break;
      case hyperspace::shader::datatype_::MAT4_:
        glUniformMatrix4fv(it.loc, it.num, GL_FALSE, reinterpret_cast<GLfloat*>(&sysMemCpy[it.bufferBytePosition]));
        break;
      case hyperspace::shader::datatype_::VECTOR2_:
        glUniform2fv(it.loc, it.num, reinterpret_cast<GLfloat*>(&sysMemCpy[it.bufferBytePosition]));
        break;
      case hyperspace::shader::datatype_::VECTOR3_:
        glUniform3fv(it.loc, it.num, reinterpret_cast<GLfloat*>(&sysMemCpy[it.bufferBytePosition]));
        break;
      case hyperspace::shader::datatype_::VECTOR4_:
        glUniform4fv(it.loc, it.num, reinterpret_cast<GLfloat*>(&sysMemCpy[it.bufferBytePosition]));
        break;
      default:
        break;
      }
    }
  }
  void GLConstantBuffer::UpdateFromSystemCopy(const DeviceContext & deviceContext)
  {
  }
  void GLConstantBuffer::UpdateFromBuffer(const DeviceContext & deviceContext, const void * buffer)
  {
    sysMemCpy.clear();
    sysMemCpy.assign((char*)buffer, (char*)buffer + descriptor.byteWidth);
  }
  void GLConstantBuffer::release()
  {
    sysMemCpy.clear();
  }
  void GLConstantBuffer::Create(const Device & device, BufferDesc desc, void * initialData)
  {
    descriptor = desc;
    if (initialData) {
      sysMemCpy.assign((char*)initialData, (char*)initialData + desc.byteWidth);
    }
  }


#if defined(USING_OPENGL_ES20) || defined(USING_OPENGL_ES30) || defined(USING_OPENGL_ES31)
  void EGLError(const char* c_ptr) {

    EGLint iErr = eglGetError();
    if (iErr != EGL_SUCCESS) {
      std::cout << "EGL CALL: " << c_ptr << " Error Code: " << iErr << std::endl;
    }

  }
  bool OpenNativeDisplay(EGLNativeDisplayType* nativedisp_out)
  {
    *nativedisp_out = (EGLNativeDisplayType)NULL;
    return true;
  }
#endif
  void	GLDriver::InitDriver() {
    T8Device = new t800::GLDevice;
    T8DeviceContext = new t800::GLDeviceContext;

#if (defined(USING_OPENGL_ES20) || defined(USING_OPENGL_ES30) || defined(USING_OPENGL_ES31)) && defined(USING_SDL)
    EGLint numConfigs;

    EGLNativeDisplayType nativeDisplay;

    if (!OpenNativeDisplay(&nativeDisplay)) {
      std::cout << "can't open native display" << std::endl;
    }

    eglDisplay = eglGetDisplay(nativeDisplay);

    EGLError("eglGetDisplay");

    EGLint iMajorVersion, iMinorVersion;

    if (!eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion)) {
      std::cout << "Failed to initialize egl" << std::endl;
    }
    else {
      std::cout << "EGL version " << iMajorVersion << "." << iMinorVersion << std::endl;
    }

    eglBindAPI(EGL_OPENGL_ES_API);

    EGLError("eglBindAPI");

    const EGLint attribs[] = {
      EGL_SURFACE_TYPE,	EGL_WINDOW_BIT,
      EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
      EGL_BLUE_SIZE,		8,
      EGL_GREEN_SIZE,		8,
      EGL_RED_SIZE,		8,
      EGL_DEPTH_SIZE,		24,
      EGL_NONE
    };

    if (!eglChooseConfig(eglDisplay, attribs, &eglConfig, 1, &numConfigs)) {
      std::cout << "Failed to choose config" << std::endl;
    }

    EGLError("eglChooseConfig");

    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);

    EGLError("eglCreateWindowSurface");

    EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, ai32ContextAttribs);

    EGLError("eglCreateContext");

    if (eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) == EGL_FALSE) {
      std::cout << "Failed to make current" << std::endl;
      return;
    }

    eglQuerySurface(eglDisplay, eglSurface, EGL_WIDTH, &width);
    eglQuerySurface(eglDisplay, eglSurface, EGL_HEIGHT, &height);
#elif defined(USING_OPENGL)
    GLenum err = glewInit();
    if (GLEW_OK != err) {
      printf("Error: %s\n", glewGetErrorString(err));
    }
    else {
      printf("GLEW OK\n");
    }
    SDL_Surface *sur = SDL_GetVideoSurface();
    width = sur->w;
    height = sur->h;
#endif

    std::string GL_Version = std::string((const char*)glGetString(GL_VERSION));
    std::string GL_Extensions = std::string((const char*)glGetString(GL_EXTENSIONS));

    std::istringstream iss(GL_Extensions);
    std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss},
      std::istream_iterator<std::string>{} };

    ExtensionsTok = tokens;
    Extensions = GL_Extensions;

    std::cout << "GL Version: " << GL_Version << "\n\nExtensions\n\n";

    for (unsigned int i = 0; i < ExtensionsTok.size(); i++) {
      printf("[%s]\n", ExtensionsTok[i].c_str());
    }

    const unsigned char *version = glGetString(GL_SHADING_LANGUAGE_VERSION);
    printf("GLSL Ver: %s \n", version);

    glEnable(GL_DEPTH_TEST);
    glClearDepthf(1.0f);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &CurrentFBO);

#if defined(USING_OPENGL) || defined(USING_OPENGL_ES30) || defined(USING_OPENGL_ES31)
    for (int i = 0; i < 16; i++) {
      DrawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
    }
#endif
  }

  void	GLDriver::CreateSurfaces() {

  }

  void	GLDriver::DestroySurfaces() {

  }

  void	GLDriver::Update() {

  }

  void	GLDriver::DestroyDriver() {
    T8Device->release();
    T8DeviceContext->release();
#if (defined(USING_OPENGL_ES20) || defined(USING_OPENGL_ES30) || defined(USING_OPENGL_ES31)) && defined(OS_WINDOWS)
    eglDestroySurface(eglDisplay, eglSurface);
    eglDestroyContext(eglDisplay, eglContext);
    eglTerminate(eglDisplay);
#endif
  }

  void	GLDriver::SetWindow(void *window) {
#if (defined(USING_OPENGL_ES20) || defined(USING_OPENGL_ES30) || defined(USING_OPENGL_ES31)) && defined(OS_WINDOWS)
    eglWindow = GetActiveWindow();
#endif
  }

  void	GLDriver::SetDimensions(int w, int h) {
    width = w;
    height = h;
  }

  void	GLDriver::Clear() {
    glClearColor(0.5, 0.0, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  }

  void	GLDriver::SwapBuffers() {
#ifdef OS_WINDOWS
#if defined(USING_OPENGL_ES20) || defined(USING_OPENGL_ES30)
    eglSwapBuffers(eglDisplay, eglSurface);
#elif defined(USING_OPENGL)
    SDL_GL_SwapBuffers();
#endif
#elif defined(OS_LINUX)
#ifdef USING_FREEGLUT
    glutSwapBuffers();
#elif defined(USING_WAYLAND_NATIVE)
#endif
#endif

  }

  bool GLDriver::CheckExtension(std::string s) {
    return (Extensions.find(s) != std::string::npos);
  }

  int GLDriver::CreateTexture(std::string path) {
    GLTexture *pTex = new GLTexture;
    if (pTex->LoadTexture((char*)path.c_str())) {
      Textures.push_back(pTex);
      return (Textures.size() - 1);
    }
    else {
      delete pTex;
    }
    return -1;
  }

  Texture* GLDriver::GetTexture(int id) {
    if (id < 0 || id >= (int)Textures.size()) {
      printf("Warning null ptr Textures Idx\n");
      return 0;
    }

    return Textures[id];
  }

  void GLDriver::DestroyTexture() {
    for (unsigned int i = 0; i < Textures.size(); i++) {
      GLTexture *pTex = dynamic_cast<GLTexture*>(Textures[i]);
      delete pTex;
    }
  }

  int  GLDriver::CreateRT(int nrt, int cf, int df, int w, int h, bool genMips) {
    GLRT	*pRT = new GLRT;
    if (w == 0)
      w = width;
    if (h == 0)
      h = height;
    if (pRT->LoadRT(nrt, cf, df, w, h, genMips)) {
      glBindFramebuffer(GL_FRAMEBUFFER, CurrentFBO);
      RTs.push_back(pRT);
      return (RTs.size() - 1);
    }
    else {
      delete pRT;
    }
    return -1;
  }

  void GLDriver::PushRT(int id) {
    if (id < 0 || id >= (int)RTs.size())
      return;

    CurrentRT = id;

    GLRT *pRT = dynamic_cast<GLRT*>(RTs[id]);

    glBindFramebuffer(GL_FRAMEBUFFER, pRT->vFrameBuffers[0]);

#if defined(USING_OPENGL) || defined(USING_OPENGL_ES30) || defined(USING_OPENGL_ES31)
    if (pRT->number_RT != 0) {
      glDrawBuffers(pRT->number_RT, DrawBuffers);
    }
    else {
      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    }
#endif

    glViewport(0, 0, pRT->w, pRT->h);

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void GLDriver::PopRT() {
    glBindFramebuffer(GL_FRAMEBUFFER, CurrentFBO);
    glViewport(0, 0, width, height);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    if (CurrentRT >= 0) {
      if (RTs[CurrentRT]->GenMips) {
        glBindTexture(GL_TEXTURE_2D, RTs[CurrentRT]->vColorTextures[0]->id);
        glGenerateMipmap(GL_TEXTURE_2D);
      }
    }

  }

  void GLDriver::DestroyRT(int id) {
    if (id < 0 || id >= (int)RTs.size())
      return;

    RTs[id]->DestroyRT();
    GLRT *pRT = dynamic_cast<GLRT*>(RTs[id]);
    delete pRT;

  }

  void GLDriver::DestroyRTs() {
    for (unsigned int i = 0; i < RTs.size(); i++) {
      GLRT *pRT = dynamic_cast<GLRT*>(RTs[i]);
      delete pRT;
    }
  }

  Texture* GLDriver::GetRTTexture(int id, int index) {
    if (id < 0 || id >= (int)RTs.size())
      exit(666);

    if (index == DEPTH_ATTACHMENT) {
      return RTs[id]->pDepthTexture;
    }
    else {
      return RTs[id]->vColorTextures[index];
    }
  }

  int GLDriver::CreateShader(std::string src_vs, std::string src_fs, unsigned int sig) {
    if (sig != T8_NO_SIGNATURE) {
      for (unsigned int i = 0; i < m_signatureShaders.size(); i++) {
        if (m_signatureShaders[i]->Sig == sig) {
          return i;
        }
      }
    }
    GLShader* shader = new GLShader();
    if (shader->CreateShader(src_vs, src_fs, sig)) {
      m_signatureShaders.push_back(shader);
      return (m_signatureShaders.size() - 1);
    }
    else {
      delete shader;
    }
    return -1;
  }


  ShaderBase*	GLDriver::GetShaderSig(unsigned int sig) {
    for (unsigned int i = 0; i < m_signatureShaders.size(); i++) {
      if (m_signatureShaders[i]->Sig == sig) {
        return m_signatureShaders[i];
      }
    }
    return 0;
  }

  ShaderBase*	GLDriver::GetShaderIdx(int id) {
    if (id < 0 || id >= (int)m_signatureShaders.size()) {
      printf("Warning null ptr ShaderBase Idx\n");
      return 0;
    }

    return m_signatureShaders[id];
  }

  void GLDriver::DestroyShaders() {
    for (unsigned int i = 0; i < m_signatureShaders.size(); i++) {
      GLShader *pShader = dynamic_cast<GLShader*>(m_signatureShaders[i]);
      delete pShader;
    }
  }
}
