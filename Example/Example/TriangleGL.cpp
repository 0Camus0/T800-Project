
#include "TriangleGL.h"

#include <stdio.h>

void checkcompilederrors(GLuint shader, GLenum type) {
	GLint bShaderCompiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &bShaderCompiled);
	if (!bShaderCompiled) {
		int i32InfoLogLength, i32CharsWritten;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &i32InfoLogLength);

		char* pszInfoLog = new char[i32InfoLogLength];
		glGetShaderInfoLog(shader, i32InfoLogLength, &i32CharsWritten, pszInfoLog);
		char* pszMsg = new char[i32InfoLogLength + 256];
		if (type == GL_FRAGMENT_SHADER){
			sprintf(pszMsg, "Failed to compile pixel shader: %s", pszInfoLog);
		}
		else if (type == GL_VERTEX_SHADER){ 
			sprintf(pszMsg, "Failed to compile vertex shader: %s", pszInfoLog);
		}
		else{
			sprintf(pszMsg, "Failed to compile wtf shader: %s", pszInfoLog);
		}
		printf("%s", pszMsg);
		delete[] pszMsg;
		delete[] pszInfoLog;
	}
}

GLuint createShader(GLenum type, char* pSource) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, (const char**)&pSource, NULL);
	glCompileShader(shader);
	checkcompilederrors(shader, type);
	return shader;
}

char *file2string(const char *path){
	FILE *fd;
	long len, r;
	char *str;
	if (!(fd = fopen(path, "r"))){
		fprintf(stderr, "Can't open file '%s'\n", path);
		return NULL;
	}
	fseek(fd, 0, SEEK_END);
	len = ftell(fd);
	fseek(fd, 0, SEEK_SET);
	str = (char*)malloc(len * sizeof(char));
	r = fread(str, sizeof(char), len, fd);
	str[r] = '\0';
	fclose(fd);
	return str;
}

void TrangleGL::Create() {
	shaderID = glCreateProgram();

	char *vsSourceP = file2string("VS.glsl");
	char *fsSourceP = file2string("FS.glsl");

	GLuint vshader_id = createShader(GL_VERTEX_SHADER, vsSourceP);
	GLuint fshader_id = createShader(GL_FRAGMENT_SHADER, fsSourceP);

	glAttachShader(shaderID, vshader_id);
	glAttachShader(shaderID, fshader_id);

	glLinkProgram(shaderID);
	glUseProgram(shaderID);

	vertexAttribLoc = glGetAttribLocation(shaderID, "MyVertex");
	colorAttribLoc  = glGetAttribLocation(shaderID, "MyColor");
	
	matUniformLoc = glGetUniformLocation(shaderID, "MyMatrix");
#ifdef USE_VBO
	vertices[0] = { -0.5f,  0.5f, 0.0f , 0.0f, 0.0f, 1.0f };
	vertices[1] = { -0.5f, -0.5f, 0.0f , 0.0f, 1.0f, 0.0f };
	vertices[2] = {  0.5f, -0.5f, 0.0f , 1.0f, 0.0f, 1.0f };
	vertices[3] = {  0.5f,  0.5f, 0.0f , 1.0f, 0.0f, 0.0f };

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(triVertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned short), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#else
	#ifdef USE_ARRAY_OF_STRUCTS
		vertices[0] = { -0.5f,  0.5f, 0.0f , 0.0f, 0.0f, 1.0f };
		vertices[1] = { -0.5f, -0.5f, 0.0f , 0.0f, 1.0f, 0.0f };
		vertices[2] = {  0.5f, -0.5f, 0.0f , 1.0f, 0.0f, 0.0f };

		vertices[3] = { -0.5f, 0.5f, 0.0f , 0.0f, 0.0f, 1.0f };
		vertices[4] = {  0.5f,-0.5f, 0.0f , 1.0f, 0.0f, 0.0f };
		vertices[5] = {  0.5f, 0.5f, 0.0f , 1.0f, 0.0f, 0.0f };
	#else
		positions[0] = { -0.5f,  0.5f, 0.0f };
		positions[1] = { -0.5f, -0.5f, 0.0f };
		positions[2] = {  0.5f, -0.5f, 0.0f };
	
		positions[3] = { -0.5f,  0.5f, 0.0f };
		positions[4] = {  0.5f, -0.5f, 0.0f };
		positions[5] = {  0.5f,  0.5f, 0.0f };

		colors[0] = { 0.0f, 0.0f, 1.0f };
		colors[1] = { 0.0f, 1.0f, 0.0f };
		colors[2] = { 1.0f, 0.0f, 0.0f };

		colors[3] = { 0.0f, 0.0f, 1.0f };
		colors[4] = { 1.0f, 0.0f, 0.0f };
		colors[5] = { 1.0f, 0.0f, 0.0f };
	#endif
#endif
	D3DXMatrixIdentity(&transform);
}

void TrangleGL::Transform(float *t) {
	transform = t;
}

void TrangleGL::Draw() {
	
	glUniformMatrix4fv(matUniformLoc, 1, GL_FALSE, &transform.m[0][0]);

#ifdef USE_VBO
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IB);

	glEnableVertexAttribArray(vertexAttribLoc);
	glEnableVertexAttribArray(colorAttribLoc);

	glVertexAttribPointer(vertexAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(triVertex), (void*)0);
	glVertexAttribPointer(colorAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(triVertex), (void*)12);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
#else
#ifdef USE_ARRAY_OF_STRUCTS
	glEnableVertexAttribArray(vertexAttribLoc);
	float *pos = (float*)&vertices[0].x;
	glVertexAttribPointer(vertexAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(triVertex), pos);

	glEnableVertexAttribArray(colorAttribLoc);
	float *color = (float*)&vertices[0].r;
	glVertexAttribPointer(colorAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(triVertex), color);
#else
	glEnableVertexAttribArray(vertexAttribLoc);
	glVertexAttribPointer(vertexAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(triVertex), positions);
	glEnableVertexAttribArray(colorAttribLoc);
	glVertexAttribPointer(colorAttribLoc, 3, GL_FLOAT, GL_FALSE,  sizeof(triVertex), colors);
#endif
	glUniformMatrix4fv(matUniformLoc, 1, GL_FALSE, &transform.m[0][0]);

	glDrawArrays(GL_TRIANGLES,0,6);
#endif
}

void TrangleGL::Destroy() {
	glDeleteProgram(shaderID);
}