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

	matWorldViewUniformLoc = glGetUniformLocation(ShaderProg, "WorldView");
	matWVPInverseUniformLoc = glGetUniformLocation(ShaderProg, "WVPInverse");
	matWVPLightLoc = glGetUniformLocation(ShaderProg, "WVPLight");


	LightPositions_Loc = glGetUniformLocation(ShaderProg, "LightPositions");
	LightColors_Loc = glGetUniformLocation(ShaderProg, "LightColors");
	Light0Pos_Loc = glGetUniformLocation(ShaderProg, "LightPos");
	Light0Color_Loc = glGetUniformLocation(ShaderProg, "LightColor");
	CameraPos_Loc = glGetUniformLocation(ShaderProg, "CameraPosition");
	CameraInfo_Loc = glGetUniformLocation(ShaderProg, "CameraInfo");
	LightCameraPos_Loc = glGetUniformLocation(ShaderProg, "LightCameraPosition");
	LightCameraInfo_Loc = glGetUniformLocation(ShaderProg, "LightCameraInfo");
	Ambient_loc = glGetUniformLocation(ShaderProg, "Ambient");

	DiffuseTex_loc = glGetUniformLocation(ShaderProg, "DiffuseTex");
	SpecularTex_loc = glGetUniformLocation(ShaderProg, "SpecularTex");
	GlossTex_loc = glGetUniformLocation(ShaderProg, "GlossTex");
	NormalTex_loc = glGetUniformLocation(ShaderProg, "NormalTex");
	ReflectTex_loc = glGetUniformLocation(ShaderProg, "ReflectTex");

	tex0_loc = glGetUniformLocation(ShaderProg, "tex0");
	tex1_loc = glGetUniformLocation(ShaderProg, "tex1");
	tex2_loc = glGetUniformLocation(ShaderProg, "tex2");
	tex3_loc = glGetUniformLocation(ShaderProg, "tex3");
	tex4_loc = glGetUniformLocation(ShaderProg, "tex4");
	tex5_loc = glGetUniformLocation(ShaderProg, "tex5");
	tex6_loc = glGetUniformLocation(ShaderProg, "tex6");
	tex7_loc = glGetUniformLocation(ShaderProg, "tex7");

	texEnv_loc = glGetUniformLocation(ShaderProg, "texEnv");

	return true;
}
