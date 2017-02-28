
#include "Triangle.h"

#ifdef USING_D3D11
extern ComPtr<ID3D11Device>            D3D11Device;
extern ComPtr<ID3D11DeviceContext>     D3D11DeviceContext;
#endif

void Trangle::Create() {
#ifdef USING_OPENGL_ES
	shaderID = glCreateProgram();

	char *vsSourceP = file2string("Shaders/VS_tri.glsl");
	char *fsSourceP = file2string("Shaders/FS_tri.glsl");

	GLuint vshader_id = createShader(GL_VERTEX_SHADER, vsSourceP);
	GLuint fshader_id = createShader(GL_FRAGMENT_SHADER, fsSourceP);

	delete [] vsSourceP;
	delete [] fsSourceP;

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

	indices[0] = 2;
	indices[1] = 1;
	indices[2] = 0;
	indices[3] = 3;
	indices[4] = 2;
	indices[5] = 0;

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
#elif defined(USING_D3D11)
	char *vsSourceP = file2string("Shaders/VS_tri.hlsl");
	char *fsSourceP = file2string("Shaders/FS_tri.hlsl");

	if (!vsSourceP || !fsSourceP)
		exit(32);

	HRESULT hr;
	{
		VS_blob = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;
		hr = D3DCompile(vsSourceP, (UINT)strlen(vsSourceP), 0, 0, 0, "VS", "vs_5_0", 0, 0, &VS_blob, &errorBlob);
		if (hr != S_OK) {

			if (errorBlob) {
				printf("errorBlob shader[%s]", (char*)errorBlob->GetBufferPointer());
				return;
			}

			if (VS_blob) {
				return;
			}
		}

		hr = D3D11Device->CreateVertexShader(VS_blob->GetBufferPointer(), VS_blob->GetBufferSize(), 0, &pVS);
		if (hr != S_OK) {
			printf("Error Creating Vertex Shader\n");
			return;
		}
	}

	{
		FS_blob = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;
		hr = D3DCompile(fsSourceP, (UINT)strlen(fsSourceP), 0, 0, 0, "FS", "ps_5_0", 0, 0, &FS_blob, &errorBlob);
		if (hr != S_OK) {
			if (errorBlob) {
				printf("errorBlob shader[%s]", (char*)errorBlob->GetBufferPointer());
				return;
			}

			if (FS_blob) {
				return;
			}
		}

		hr = D3D11Device->CreatePixelShader(FS_blob->GetBufferPointer(), FS_blob->GetBufferSize(), 0, &pFS);
		if (hr != S_OK) {
			printf("Error Creating Pixel Shader\n");
			return;
		}
	}

	vertices[0] = { -0.5f,  0.5f, 0.0f , 0.0f, 0.0f, 1.0f };
	vertices[1] = { -0.5f, -0.5f, 0.0f , 0.0f, 1.0f, 0.0f };
	vertices[2] = { 0.5f, -0.5f, 0.0f , 1.0f, 0.0f, 1.0f };
	vertices[3] = { 0.5f,  0.5f, 0.0f , 1.0f, 0.0f, 0.0f };

	indices[0] = 2;
	indices[1] = 1;
	indices[2] = 0;
	indices[3] = 3;
	indices[4] = 2;
	indices[5] = 0;

	D3D11DeviceContext->VSSetShader(pVS.Get(), 0, 0);
	D3D11DeviceContext->PSSetShader(pFS.Get(), 0, 0);

	D3D11_INPUT_ELEMENT_DESC vertexDeclaration[] = {
		{ "POSITION" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL"   , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	hr = D3D11Device->CreateInputLayout(vertexDeclaration, ARRAYSIZE(vertexDeclaration), VS_blob->GetBufferPointer(), VS_blob->GetBufferSize(), &Layout);
	if (hr != S_OK) {
		printf("Error Creating Input Layout\n");
		return;
	}
	D3D11DeviceContext->IASetInputLayout(Layout.Get());

	D3D11_BUFFER_DESC bdesc = { 0 };
	bdesc.Usage = D3D11_USAGE_DEFAULT;
	bdesc.ByteWidth = sizeof(Trangle::CBuffer);
	bdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	hr = D3D11Device->CreateBuffer(&bdesc, 0, pd3dConstantBuffer.GetAddressOf());
	if (hr != S_OK) {
		printf("Error Creating Buffer Layout\n");
		return;
	}

	D3D11DeviceContext->VSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());
	D3D11DeviceContext->PSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());


	bdesc = { 0 };
	bdesc.ByteWidth = sizeof(triVertex) * 4;
	bdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA subData = { vertices, 0, 0 };

	hr = D3D11Device->CreateBuffer(&bdesc, &subData, &VB);
	if (hr != S_OK) {
		printf("Error Creating Vertex Buffer\n");
		return;
	}

	bdesc = { 0 };
	bdesc.ByteWidth = 6 * sizeof(USHORT);
	bdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	subData = { indices, 0, 0 };

	hr = D3D11Device->CreateBuffer(&bdesc, &subData, &IB);
	if (hr != S_OK) {
		printf("Error Creating Index Buffer\n");
		return;
	}
#endif
		XMatIdentity(transform);
}

void Trangle::Transform(float *t) {
	transform = t;
}

void Trangle::Draw(float *t,float *vp) {
	
	
	if (t)
		transform = t;
#ifdef USING_OPENGL_ES
	glUseProgram(shaderID);
	glUniformMatrix4fv(matUniformLoc, 1, GL_FALSE, &transform.m[0][0]);

#ifdef USE_VBO
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IB);

	glEnableVertexAttribArray(vertexAttribLoc);
	glEnableVertexAttribArray(colorAttribLoc);

	glVertexAttribPointer(vertexAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(triVertex), (void*)0);
	glVertexAttribPointer(colorAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(triVertex), (void*)12);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
	glDisableVertexAttribArray(vertexAttribLoc);
	glDisableVertexAttribArray(colorAttribLoc);

	glUseProgram(0);
#elif defined(USING_D3D11)

	XMATRIX44 VP = XMATRIX44(vp);
	XMATRIX44 WVP = transform*VP;
	CnstBuffer.WVP = WVP;
	CnstBuffer.World = transform;

	UINT stride = sizeof(triVertex);
	UINT offset = 0;
	D3D11DeviceContext->VSSetShader(pVS.Get(), 0, 0);
	D3D11DeviceContext->PSSetShader(pFS.Get(), 0, 0);

	D3D11DeviceContext->IASetInputLayout(Layout.Get());

	D3D11DeviceContext->UpdateSubresource(pd3dConstantBuffer.Get(), 0, 0, &CnstBuffer, 0, 0);

	D3D11DeviceContext->VSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());
	D3D11DeviceContext->PSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());

	D3D11DeviceContext->IASetVertexBuffers(0, 1, VB.GetAddressOf(), &stride, &offset);
	D3D11DeviceContext->IASetIndexBuffer(IB.Get(), DXGI_FORMAT_R16_UINT, 0);

	D3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D11DeviceContext->DrawIndexed(6, 0, 0);
#endif
}

void Trangle::Destroy() {
#ifdef USING_OPENGL_ES
	glDeleteProgram(shaderID);
#endif
}