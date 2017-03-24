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

#include <scene\Triangle.h>

// Global D3D11 Main Objects
#ifdef USING_D3D11
extern ComPtr<ID3D11Device>            D3D11Device;
extern ComPtr<ID3D11DeviceContext>     D3D11DeviceContext;
#endif

void Trangle::Create() {
#ifdef USING_OPENGL_ES
	//	Create the Program, return the ID of the created program
	shaderID = glCreateProgram();

	char *vsSourceP = file2string("Shaders/VS_tri.glsl");
	char *fsSourceP = file2string("Shaders/FS_tri.glsl");

	// Compile the shader from the char* source
	GLuint vshader_id = createShader(GL_VERTEX_SHADER, vsSourceP);
	GLuint fshader_id = createShader(GL_FRAGMENT_SHADER, fsSourceP);

	// Once the shader programs are created we can safely free the buffers
	free(vsSourceP);
	free(fsSourceP);

	// We attach the id of the vertex and pixel shader to this shader program
	glAttachShader(shaderID, vshader_id);
	glAttachShader(shaderID, fshader_id);

	// Link the object
	glLinkProgram(shaderID);
	// We use the program now, it is bounded now
	glUseProgram(shaderID);

	// Get Vertex Attributes from the Shader
	vertexAttribLoc = glGetAttribLocation(shaderID, "MyVertex");
	colorAttribLoc  = glGetAttribLocation(shaderID, "MyColor");
	
	// Get the Uniform location, that would be the global variables in the shader
	matUniformLoc = glGetUniformLocation(shaderID, "MyMatrix");
#ifdef USE_VBO
	vertices[0] = { -0.5f,  0.5f, 0.0f , 0.0f, 0.0f, 1.0f };
	vertices[1] = { -0.5f, -0.5f, 0.0f , 0.0f, 1.0f, 0.0f };
	vertices[2] = {  0.5f, -0.5f, 0.0f , 1.0f, 0.0f, 1.0f };
	vertices[3] = {  0.5f,  0.5f, 0.0f , 1.0f, 0.0f, 0.0f };
	// Generate the Vertex buffer, it returns an Id
	glGenBuffers(1, &VB);
	// We bind the current buffer to the one with the Id we got
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	// We populate the buffer with the vertex data, it needs the size in bytes and the pointer to the first element
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(triVertex), &vertices[0], GL_STATIC_DRAW);
	// We bind to the default 
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	indices[0] = 2;
	indices[1] = 1;
	indices[2] = 0;
	indices[3] = 3;
	indices[4] = 2;
	indices[5] = 0;

	// Same as the VB
	glGenBuffers(1, &IB);
	// Same but this time we specify that it will be INDEX buffer, rather than VERTEX buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	// Same, size of the buffer and the pointer
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

	// No valid pointer, exit..
	if (!vsSourceP || !fsSourceP)
		exit(32);

	HRESULT hr;
	{
		VS_blob = nullptr; // VS_blob would contain the binary compiled vertex shader program
		ComPtr<ID3DBlob> errorBlob = nullptr; // In case of error, this blob would contain the compilation errors
		// We compile the source, the entry point is VS in our vertex shader, and we are using shader model 5 (d3d11)
		hr = D3DCompile(vsSourceP, (UINT)strlen(vsSourceP), 0, 0, 0, "VS", "vs_5_0", 0, 0, &VS_blob, &errorBlob);
		if (hr != S_OK) { // some error

			if (errorBlob) { // print the error if the blob is valid
				printf("errorBlob shader[%s]", (char*)errorBlob->GetBufferPointer());
				return;
			}
			// No binary data, return.
			if (VS_blob) {
				return;
			}
		}
		// With the binary blob now we create the Vertex Shader Object
		hr = D3D11Device->CreateVertexShader(VS_blob->GetBufferPointer(), VS_blob->GetBufferSize(), 0, &pVS);
		if (hr != S_OK) {
			printf("Error Creating Vertex Shader\n");
			return;
		}
	}
	// Same for the Pixel Shader, just change the entry point, blob, etc, same exact method
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

	free(vsSourceP);
	free(fsSourceP);

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

	// We use the context to set the current Vertex and Pixel programs to be bound
	D3D11DeviceContext->VSSetShader(pVS.Get(), 0, 0);
	D3D11DeviceContext->PSSetShader(pFS.Get(), 0, 0);

	// We need to have a vertex declaration to instruct d3d11 on how we are constructing our vertex data,
	// On this case we use 3 float position and 3 float normal, the normal start at offset 12
	D3D11_INPUT_ELEMENT_DESC vertexDeclaration[] = {
		{ "POSITION" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL"   , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// We create the actual input layout based on the description we just set
	hr = D3D11Device->CreateInputLayout(vertexDeclaration, ARRAYSIZE(vertexDeclaration), VS_blob->GetBufferPointer(), VS_blob->GetBufferSize(), &Layout);
	if (hr != S_OK) {
		printf("Error Creating Input Layout\n");
		return;
	}

	// We Bound the input layout
	D3D11DeviceContext->IASetInputLayout(Layout.Get());

	// Constant buffer would be the created in base on our CBuffer, we fill the descriptor with the correct size of the structure
	// and also we set that the use of this buffer would be constant buffer
	D3D11_BUFFER_DESC bdesc = { 0 };
	bdesc.Usage = D3D11_USAGE_DEFAULT;
	bdesc.ByteWidth = sizeof(Trangle::CBuffer);
	bdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// Create the actual buffer: Note that we use the Device to create resources
	hr = D3D11Device->CreateBuffer(&bdesc, 0, pd3dConstantBuffer.GetAddressOf());
	if (hr != S_OK) {
		printf("Error Creating Buffer Layout\n");
		return;
	}

	// Set the constant buffer to the shader programs: Note that we use the Device Context to manage the resources
	D3D11DeviceContext->VSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());
	D3D11DeviceContext->PSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());

	// We fill the descriptor, this time we set that will be used for vertex data, that is, vertex buffer
	bdesc = { 0 };
	bdesc.ByteWidth = sizeof(triVertex) * 4;
	bdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// Resource sub data is just a structure friendly to set our pointer to the vertex data
	D3D11_SUBRESOURCE_DATA subData = { vertices, 0, 0 };

	// As we did with the constant buffer, we create the actual vertex buffer
	hr = D3D11Device->CreateBuffer(&bdesc, &subData, &VB);
	if (hr != S_OK) {
		printf("Error Creating Vertex Buffer\n");
		return;
	}

	// Same for the index buffer
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

void Trangle::Draw(float *t, float *vp) {


	if (t)
		transform = t;
#ifdef USING_OPENGL_ES
// We instruct GLES that we will use the program we created bounded to this primitive, rather than another primitive's program
	glUseProgram(shaderID);
	// We send the current matrix to the shader using the uniform location we've got
	glUniformMatrix4fv(matUniformLoc, 1, GL_FALSE, &transform.m[0][0]);

#ifdef USE_VBO
	// We bound the vertex buffer, so GLES know that it will work with this vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	// Same for the Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IB);

	// Enable on attributes using the locations bound with the shaders
	glEnableVertexAttribArray(vertexAttribLoc);
	glEnableVertexAttribArray(colorAttribLoc);

	// Set the attributes, since we are using vertex buffers, we only send the stride and the offset
	// stride = size of the vertex in bytes
	// offset = where it start the current element in bytes, since position have 3 floats that would be 12 bytes for the color attribute
	glVertexAttribPointer(vertexAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(triVertex), (void*)0);
	glVertexAttribPointer(colorAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(triVertex), (void*)12);

	// We draw the triangles, we send the number of indices
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	// Bound to the default the buffers so we don't do any operation on the buffers
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
	// We desable the attrbutes since we are done with them for this frame
	glDisableVertexAttribArray(vertexAttribLoc);
	glDisableVertexAttribArray(colorAttribLoc);

	// Bound to the default program
	glUseProgram(0);
#elif defined(USING_D3D11)

	XMATRIX44 VP = XMATRIX44(vp);
	XMATRIX44 WVP = transform*VP;
	CnstBuffer.WVP = WVP;
	CnstBuffer.World = transform;

	UINT stride = sizeof(triVertex); 
	UINT offset = 0;
	// We bound to use the vertex and pixel shader of this primitive
	D3D11DeviceContext->VSSetShader(pVS.Get(), 0, 0);
	D3D11DeviceContext->PSSetShader(pFS.Get(), 0, 0);
	// Set the input layout to let the shader program know what kind of vertex data we have
	D3D11DeviceContext->IASetInputLayout(Layout.Get());
	// We update the constant buffer with the current matrices
	D3D11DeviceContext->UpdateSubresource(pd3dConstantBuffer.Get(), 0, 0, &CnstBuffer, 0, 0);
	// Once updated the constant buffer we send them to the shader programs
	D3D11DeviceContext->VSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());
	D3D11DeviceContext->PSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());
	// We let d3d that we are using our vertex and index buffers, they require the stride and offset
	D3D11DeviceContext->IASetVertexBuffers(0, 1, VB.GetAddressOf(), &stride, &offset);
	// Same for the index buffer
	D3D11DeviceContext->IASetIndexBuffer(IB.Get(), DXGI_FORMAT_R16_UINT, 0);
	// Instruct to use triangle list
	D3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Draw the primitive sending the number of indices
	D3D11DeviceContext->DrawIndexed(6, 0, 0);
#endif
}

void Trangle::Destroy() {
#ifdef USING_OPENGL_ES
	glDeleteProgram(shaderID);
#endif
}