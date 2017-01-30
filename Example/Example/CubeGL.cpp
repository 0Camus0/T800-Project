#include "CubeGL.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void CubeGL::Create() {
	shaderID = glCreateProgram();

	char *vsSourceP = file2string("VS.glsl");
	char *fsSourceP = file2string("FS.glsl");

	GLuint vshader_id = createShader(GL_VERTEX_SHADER, vsSourceP);
	GLuint fshader_id = createShader(GL_FRAGMENT_SHADER, fsSourceP);

	glAttachShader(shaderID, vshader_id);
	glAttachShader(shaderID, fshader_id);

	glLinkProgram(shaderID);
	glUseProgram(shaderID);

	vertexAttribLoc = glGetAttribLocation(shaderID, "Vertex");
	normalAttribLoc = glGetAttribLocation(shaderID, "Normal");
	uvAttribLoc     = glGetAttribLocation(shaderID, "UV");

	matWorldViewProjUniformLoc = glGetUniformLocation(shaderID, "WVP");
	matWorldUniformLoc		   = glGetUniformLocation(shaderID, "World");
	
	// +Y SIDE
	vertices[0] = { -1.0f,  1.0f, -1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  0.0f, 1.0f };
	vertices[1] = {  1.0f,  1.0f, -1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f };
	vertices[2] = { -1.0f,  1.0f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  0.0f, 0.0f };
	vertices[3] = {  1.0f,  1.0f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 0.0f };

	// -Y SIDE
	vertices[4] = { -1.0f,  -1.0f, -1.0f, 1.0f,  0.0f, -1.0f, 0.0f, 1.0f,  1.0f, 0.0f };
	vertices[5] = {  1.0f,  -1.0f, -1.0f, 1.0f,  0.0f, -1.0f, 0.0f, 1.0f,  0.0f, 0.0f };
	vertices[6] = { -1.0f,  -1.0f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f, 1.0f,  1.0f, 1.0f };
	vertices[7] = {  1.0f,  -1.0f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f, 1.0f,  0.0f, 1.0f };

	// +X SIDE
	vertices[8]  = { 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 1.0f };
	vertices[9]  = { 1.0f,  1.0f, -1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f };
	vertices[10] = { 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f };
	vertices[11] = { 1.0f, -1.0f, -1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f };

	// -X SIDE
	vertices[12] = { -1.0f,  1.0f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f };
	vertices[13] = { -1.0f,  1.0f, -1.0f, 1.0f,  -1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f };
	vertices[14] = { -1.0f, -1.0f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f };
	vertices[15] = { -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 1.0f };

	// +Z SIDE
	vertices[16] = { -1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f,  0.0f, 1.0f };
	vertices[17] = {  1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f,  1.0f, 1.0f };
	vertices[18] = { -1.0f, -1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f };
	vertices[19] = {  1.0f, -1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f,  1.0f, 0.0f };

	// -Z SIDE
	vertices[20] = { -1.0f,  1.0f, -1.0f, 1.0f,  0.0f, 0.0f, -1.0f, 1.0f,  1.0f, 0.0f };
	vertices[21] = {  1.0f,  1.0f, -1.0f, 1.0f,  0.0f, 0.0f, -1.0f, 1.0f,  0.0f, 0.0f };
	vertices[22] = { -1.0f, -1.0f, -1.0f, 1.0f,  0.0f, 0.0f, -1.0f, 1.0f,  1.0f, 1.0f };
	vertices[23] = {  1.0f, -1.0f, -1.0f, 1.0f,  0.0f, 0.0f, -1.0f, 1.0f,  0.0f, 1.0f };

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(CVertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// +X
	indices[0] = 8;
	indices[1] = 9;
	indices[2] = 10;
	indices[3] = 9;
	indices[4] = 11;
	indices[5] = 10;

	// -X
	indices[6] = 14;
	indices[7] = 13;
	indices[8] = 12;
	indices[9] = 14;
	indices[10] = 15;
	indices[11] = 13;

	// +Y
	indices[12] = 1;
	indices[13] = 2;
	indices[14] = 0;
	indices[15] = 3;
	indices[16] = 2;
	indices[17] = 1;

	// -Y
	indices[18] = 4;
	indices[19] = 6;
	indices[20] = 5;
	indices[21] = 5;
	indices[22] = 6;
	indices[23] = 7;

	// +Z
	indices[24] = 17;
	indices[25] = 18;
	indices[26] = 16;
	indices[27] = 19;
	indices[28] = 18;
	indices[29] = 17;

	// -Z
	indices[30] = 20;
	indices[31] = 22;
	indices[32] = 21;
	indices[33] = 21;
	indices[34] = 22;
	indices[35] = 23;

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned short), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	D3DXMatrixIdentity(&transform);
}

void CubeGL::Transform(float *t) {
	transform = t;
}

void CubeGL::Draw(float *t,float *vp) {
	glUseProgram(shaderID);

	if (t)
		transform = t;

	D3DXMATRIX VP = D3DXMATRIX(vp);
	D3DXMATRIX WVP = transform*VP;

	glUniformMatrix4fv(matWorldUniformLoc, 1, GL_FALSE, &transform.m[0][0]);
	glUniformMatrix4fv(matWorldViewProjUniformLoc, 1, GL_FALSE, &WVP.m[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	glEnableVertexAttribArray(vertexAttribLoc);
	glEnableVertexAttribArray(normalAttribLoc);

	if (uvAttribLoc != -1) 
		glEnableVertexAttribArray(uvAttribLoc);
	
	glVertexAttribPointer(vertexAttribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(0));
	glVertexAttribPointer(normalAttribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(16));
	
	if(uvAttribLoc!=-1)
		glVertexAttribPointer(uvAttribLoc, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(32));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(vertexAttribLoc);
	glDisableVertexAttribArray(normalAttribLoc);
	
	if (uvAttribLoc!=-1) {
	glDisableVertexAttribArray(uvAttribLoc);
	}

	glUseProgram(0);
}

void CubeGL::Destroy() {
	glDeleteProgram(shaderID);
}