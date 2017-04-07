#include <video/GLShader.h>
#include <utils/Utils.h>

bool GLShader::CreateShaderAPI(std::string src_vs, std::string src_fs, unsigned int sig) {

	ShaderProg = glCreateProgram();

	GLuint vshader_id = createShader(GL_VERTEX_SHADER, (char*)src_vs.c_str());
	GLuint fshader_id = createShader(GL_FRAGMENT_SHADER, (char*)src_fs.c_str());

	glAttachShader(ShaderProg, vshader_id);
	glAttachShader(ShaderProg, fshader_id);

	glLinkProgram(ShaderProg);
	glUseProgram(ShaderProg);

	vertexAttribLoc = glGetAttribLocation(ShaderProg, "Vertex");
	normalAttribLoc = glGetAttribLocation(ShaderProg, "Normal");
	uvAttribLoc = glGetAttribLocation(ShaderProg, "UV");
	uvSecAttribLoc = glGetAttribLocation(ShaderProg, "UV_Sec");
	tangentAttribLoc = glGetAttribLocation(ShaderProg, "Tangent");
	binormalAttribLoc = glGetAttribLocation(ShaderProg, "Binormal");

	matWorldViewProjUniformLoc = glGetUniformLocation(ShaderProg, "WVP");
	matWorldUniformLoc = glGetUniformLocation(ShaderProg, "World");

	Light0Pos_Loc = glGetUniformLocation(ShaderProg, "LightPos");
	Light0Color_Loc = glGetUniformLocation(ShaderProg, "LightColor");
	CameraPos_Loc = glGetUniformLocation(ShaderProg, "CameraPosition");
	Ambient_loc = glGetUniformLocation(ShaderProg, "Ambient");

	DiffuseTex_loc = glGetUniformLocation(ShaderProg, "DiffuseTex");
	SpecularTex_loc = glGetUniformLocation(ShaderProg, "SpecularTex");
	GlossTex_loc = glGetUniformLocation(ShaderProg, "GlossTex");
	NormalTex_loc = glGetUniformLocation(ShaderProg, "NormalTex");
	ReflectTex_loc = glGetUniformLocation(ShaderProg, "ReflectTex");
}
