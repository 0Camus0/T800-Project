#include "CubeGL.h"


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

	if(normalAttribLoc<0)
		normalAttribLoc=0;

	if(uvAttribLoc<0)
		uvAttribLoc=0;


	matWorldViewProjUniformLoc = glGetUniformLocation(shaderID, "WVP");
	matWorldUniformLoc		   = glGetUniformLocation(shaderID, "World");
	
	// +Y SIDE
	vertices[0] = { -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f };
	vertices[1] = {  1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f };
	vertices[2] = { -1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f };
	vertices[3] = {  1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f };

	// -Y SIDE
	vertices[4] = { -1.0f,  -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
	vertices[5] = {  1.0f,  -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f };
	vertices[6] = { -1.0f,  -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f };
	vertices[7] = {  1.0f,  -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f };

	// +X SIDE
	vertices[8]  = { 1.0f,  1.0f,  1.0f,   1.0f, 0.0f, 0.0f,  0.0f, 1.0f };
	vertices[9]  = { 1.0f,  1.0f, -1.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f };
	vertices[10] = { 1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f };
	vertices[11] = { 1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f };

	// -X SIDE
	vertices[12] = { -1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
	vertices[13] = { -1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	vertices[14] = { -1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f };
	vertices[15] = { -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	// +Z SIDE
	vertices[16] = { -1.0f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f };
	vertices[17] = {  1.0f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f };
	vertices[18] = { -1.0f, -1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f };
	vertices[19] = {  1.0f, -1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f };

	// -Z SIDE
	vertices[20] = { -1.0f,  1.0f, -1.0f,  0.0f, 0.0f, -1.0f, 1.0f, 0.0f };
	vertices[21] = {  1.0f,  1.0f, -1.0f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f };
	vertices[22] = { -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, -1.0f, 1.0f, 1.0f };
	vertices[23] = {  1.0f, -1.0f, -1.0f,  0.0f, 0.0f, -1.0f, 0.0f, 1.0f };

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

void CubeGL::Draw() {

	glUniformMatrix4fv(matWorldUniformLoc, 1, GL_FALSE, &transform.m[0][0]);

	D3DXMATRIX View;
	D3DXVECTOR3 Pos		= D3DXVECTOR3(0.0f,1.0f,5.0f);
	D3DXVECTOR3 Up		= D3DXVECTOR3(0.0f,1.0f,0.0f);
	D3DXVECTOR3 LookAt	= D3DXVECTOR3(0.0001f, 0.0001f, 0.0001f) - Pos;
	D3DXMatrixLookAtRH(&View,&Pos,&LookAt,&Up);
	D3DXMATRIX Proj;
//	D3DXMatrixPerspectiveFovRH(&Proj,D3DXToRadian(45.0f),1280.0f/720.0f,0.1f,1000.0f);
//	D3DXMatrixOrthoRH(&Proj, 1280.0f / 720.0f, 1.0f , 0.1, 100.0f);
	D3DXMATRIX WVP = transform*View;
	glUniformMatrix4fv(matWorldViewProjUniformLoc, 1, GL_FALSE, &WVP.m[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	glEnableVertexAttribArray(vertexAttribLoc);
	glVertexAttribPointer(vertexAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(CVertex), (void*)0);

	if (normalAttribLoc>0) {
		glEnableVertexAttribArray(normalAttribLoc);
		glVertexAttribPointer(normalAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(CVertex), (void*)12);
	}

	if(uvAttribLoc>0){
		glEnableVertexAttribArray(uvAttribLoc);
		glVertexAttribPointer(uvAttribLoc, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex), (void*)24);
	}

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(vertexAttribLoc);
	if (uvAttribLoc>0) {
	glDisableVertexAttribArray(normalAttribLoc);
	}
	if (uvAttribLoc>0) {
	glDisableVertexAttribArray(uvAttribLoc);
	}

}

void CubeGL::Destroy() {
	glDeleteProgram(shaderID);
}