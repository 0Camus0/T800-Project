#include <video/GLShader.h>
#include <utils/Utils.h>
#include "video\GLDriver.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
bool GLShader::CreateShaderAPI(std::string src_vs, std::string src_fs, unsigned int sig) {

	ShaderProg = glCreateProgram();

	GLuint vshader_id = createShader(GL_VERTEX_SHADER, (char*)src_vs.c_str());
	GLuint fshader_id = createShader(GL_FRAGMENT_SHADER, (char*)src_fs.c_str());

	glAttachShader(ShaderProg, vshader_id);
	glAttachShader(ShaderProg, fshader_id);

	glLinkProgram(ShaderProg);
	glUseProgram(ShaderProg);

  t800::InputElement ie;
  vertexWidth = 0;
  ie.loc = glGetAttribLocation(ShaderProg, "Vertex");
  ie.type  = T8_CBUFFER_TYPE::VECTOR4;
  locs.push_back(ie);
  ie.loc = glGetAttribLocation(ShaderProg, "Normal");
  locs.push_back(ie);
  ie.loc = glGetAttribLocation(ShaderProg, "Tangent");
  locs.push_back(ie);
  ie.loc = glGetAttribLocation(ShaderProg, "Binormal");
  locs.push_back(ie);
  ie.type = T8_CBUFFER_TYPE::VECTOR2;
  ie.loc = glGetAttribLocation(ShaderProg, "UV");
  locs.push_back(ie);
  ie.loc = glGetAttribLocation(ShaderProg, "UV_Sec");
  locs.push_back(ie);


  ie.type = T8_CBUFFER_TYPE::MATRIX;
  ie.loc = glGetUniformLocation(ShaderProg, "WVP");
  internalUniformsLocs.push_back(ie);
  ie.loc = glGetUniformLocation(ShaderProg, "World");
  internalUniformsLocs.push_back(ie);
  ie.loc = glGetUniformLocation(ShaderProg, "WorldView");
  internalUniformsLocs.push_back(ie);
  ie.loc = glGetUniformLocation(ShaderProg, "WVPInverse");
  internalUniformsLocs.push_back(ie);
  ie.loc = glGetUniformLocation(ShaderProg, "WVPLight");
  internalUniformsLocs.push_back(ie);

  ie.type = T8_CBUFFER_TYPE::VECTOR4;
  ie.loc = glGetUniformLocation(ShaderProg, "LightPositions");
  ie.num = 128;
  internalUniformsLocs.push_back(ie);
  ie.loc = glGetUniformLocation(ShaderProg, "LightColors");
  internalUniformsLocs.push_back(ie);
  ie.num = 1;
  ie.loc = glGetUniformLocation(ShaderProg, "LightPos");
  internalUniformsLocs.push_back(ie);
  ie.loc = glGetUniformLocation(ShaderProg, "LightColor");
  internalUniformsLocs.push_back(ie);
  ie.loc = glGetUniformLocation(ShaderProg, "CameraPosition");
  internalUniformsLocs.push_back(ie);
  ie.loc = glGetUniformLocation(ShaderProg, "CameraInfo");   
  internalUniformsLocs.push_back(ie);
  ie.loc = glGetUniformLocation(ShaderProg, "LightCameraPosition");
  internalUniformsLocs.push_back(ie);
  ie.loc = glGetUniformLocation(ShaderProg, "LightCameraInfo");
  internalUniformsLocs.push_back(ie);
  ie.loc = glGetUniformLocation(ShaderProg, "Ambient");
  internalUniformsLocs.push_back(ie);

	return true;
}

void GLShader::Set(const t800::DeviceContext & deviceContext)
{
  const_cast<t800::DeviceContext*>(&deviceContext)->actualShaderSet = (Shader*)this;
  int stride = reinterpret_cast<const t800::GLDeviceContext*>(&deviceContext)->internalStride;
  glUseProgram(ShaderProg);

  int offset = 0;
  for (auto& it : locs)
  {
    if (it.loc != -1) {
      int size = 0;
      int offsetInc = 0;
      switch (it.type)
      {
      case T8_CBUFFER_TYPE::FLOAT:
        offsetInc = 4;
        size = 1;
        break;
      case T8_CBUFFER_TYPE::VECTOR2:
        offsetInc = 8;
        size = 2;
        break;
      case T8_CBUFFER_TYPE::VECTOR4:
        offsetInc = 16;
        size = 4;
        break;
      }
      glEnableVertexAttribArray(it.loc);
      glVertexAttribPointer(it.loc, size, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(offset));
      offset += offsetInc;
    }
  }
}
