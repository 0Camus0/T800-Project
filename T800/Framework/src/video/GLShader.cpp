#include <video/GLShader.h>
#include <utils/Utils.h>
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
  ie.loc = glGetAttribLocation(ShaderProg, "Vertex");
  ie.type  = T8_CBUFFER_TYPE::VECTOR4;
  locs.push_back(ie);
  ie.loc = glGetAttribLocation(ShaderProg, "Normal");
  locs.push_back(ie);
  ie.loc = glGetAttribLocation(ShaderProg, "UV");
  ie.type = T8_CBUFFER_TYPE::VECTOR2;
  locs.push_back(ie);
  ie.loc = glGetAttribLocation(ShaderProg, "UV_Sec");
  locs.push_back(ie);
  ie.loc = glGetAttribLocation(ShaderProg, "Tangent");
  ie.type = T8_CBUFFER_TYPE::VECTOR4;
  locs.push_back(ie);
  ie.loc = glGetAttribLocation(ShaderProg, "Binormal");
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
  internalUniformsLocs.push_back(ie);
  ie.loc = glGetUniformLocation(ShaderProg, "LightColors");
  internalUniformsLocs.push_back(ie);
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


  //ie.type = T8_CBUFFER_TYPE::TEXTURE;

  //ie.loc = glGetUniformLocation(ShaderProg, "DiffuseTex");
  //internalUniformsLocs.push_back(ie);
  //ie.loc = glGetUniformLocation(ShaderProg, "SpecularTex");
  //internalUniformsLocs.push_back(ie);
  //ie.loc = glGetUniformLocation(ShaderProg, "GlossTex");
  //internalUniformsLocs.push_back(ie);
  //ie.loc = glGetUniformLocation(ShaderProg, "NormalTex");
  //internalUniformsLocs.push_back(ie);
  //ie.loc = glGetUniformLocation(ShaderProg, "ReflectTex");
  //internalUniformsLocs.push_back(ie);

  //locs["tex0"] = glGetUniformLocation(ShaderProg, "tex0");
  //locs["tex1"] = glGetUniformLocation(ShaderProg, "tex1");
  //locs["tex2"] = glGetUniformLocation(ShaderProg, "tex2");
  //locs["tex3"] = glGetUniformLocation(ShaderProg, "tex3");
  //locs["tex4"] = glGetUniformLocation(ShaderProg, "tex4");
  //locs["tex5"] = glGetUniformLocation(ShaderProg, "tex5");
  //locs["tex6"] = glGetUniformLocation(ShaderProg, "tex6");
  //locs["tex7"] = glGetUniformLocation(ShaderProg, "tex7");

  //locs["texEnv"] = glGetUniformLocation(ShaderProg, "texEnv");

	return true;
}

void GLShader::Set(const t800::DeviceContext & deviceContext)
{
  const_cast<t800::DeviceContext*>(&deviceContext)->actualShaderSet = (Shader*)this;
  glUseProgram(ShaderProg);

  int offset = 0;
  for (auto& it : locs)
  {
    if (it.loc != -1) {
      int size = 0;
      switch (it.type)
      {
      case T8_CBUFFER_TYPE::FLOAT:
        offset += 4;
        size = 1;
        break;
      case T8_CBUFFER_TYPE::VECTOR2:
        offset += 8;
        size = 2;
        break;
      case T8_CBUFFER_TYPE::VECTOR4:
        offset += 16;
        size = 4;
        break;
      }
      glEnableVertexAttribArray(it.loc);
      glVertexAttribPointer(it.loc, size, GL_FLOAT, GL_FALSE, deviceContext.actualVertexBuffer->descriptor.byteWidth, BUFFER_OFFSET(offset));
    }
  }
}
