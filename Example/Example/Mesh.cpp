#include "Mesh.h"
#include <iostream>

#define CHANGE_TO_RH 0
#define DEBUG_MODEL 0

#ifdef USING_D3D11
extern ComPtr<ID3D11Device>            D3D11Device;
extern ComPtr<ID3D11DeviceContext>     D3D11DeviceContext;
#endif


void Mesh::Create(char *filename) {

	std::string fname = std::string(filename);
	if (xFile.LoadXFile(fname)) {
		cout << " Load " << endl;
	}
	else {
		cout << " Failed " << endl;
	}

#ifdef USING_D3D11
	HRESULT hr;
#endif

	GatherInfo();

	for (std::size_t i = 0; i < xFile.MeshInfo.size(); i++) {
		xFinalGeometry *it = &xFile.MeshInfo[i];
		xMeshGeometry *pActual = &xFile.XMeshDataBase[0]->Geometry[i];
		MeshInfo  *it_MeshInfo = &Info[i];

#ifdef USING_OPENGL_ES
		char *vsSourceP = file2string("Shaders/VS_Mesh.glsl");
		char *fsSourceP = file2string("Shaders/FS_Mesh.glsl");
#elif defined(USING_D3D11)
		char *vsSourceP = file2string("Shaders/VS_Mesh.hlsl");
		char *fsSourceP = file2string("Shaders/FS_Mesh.hlsl");
#endif
		std::string vstr = std::string(vsSourceP);
		std::string fstr = std::string(fsSourceP);
		std::string Defines = "";

		if(pActual->VertexAttributes&xMeshGeometry::HAS_NORMAL)
			Defines += "#define USE_NORMALS\n\n";
		if (pActual->VertexAttributes&xMeshGeometry::HAS_TEXCOORD0)
			Defines += "#define USE_TEXCOORD0\n\n";
		if (pActual->VertexAttributes&xMeshGeometry::HAS_TEXCOORD1)
			Defines += "#define USE_TEXCOORD1\n\n";
		if (pActual->VertexAttributes&xMeshGeometry::HAS_TANGENT)
			Defines += "#define USE_TANGENTS\n\n";
		if (pActual->VertexAttributes&xMeshGeometry::HAS_BINORMAL)
			Defines += "#define USE_BINORMALS\n\n";

		vstr = Defines + vstr;
		fstr = Defines + fstr;
#ifdef USING_OPENGL_ES
		unsigned int ShaderProg = glCreateProgram();
		it_MeshInfo->ShaderProg = ShaderProg;

		GLuint vshader_id = createShader(GL_VERTEX_SHADER, (char*)vstr.c_str());
		GLuint fshader_id = createShader(GL_FRAGMENT_SHADER, (char*)fstr.c_str());

		delete[] vsSourceP;
		delete[] fsSourceP;

		glAttachShader(ShaderProg, vshader_id);
		glAttachShader(ShaderProg, fshader_id);

		glLinkProgram(ShaderProg);
		glUseProgram(ShaderProg);

		it_MeshInfo->vertexAttribLoc = glGetAttribLocation(ShaderProg, "Vertex");
		it_MeshInfo->normalAttribLoc = glGetAttribLocation(ShaderProg, "Normal");
		it_MeshInfo->uvAttribLoc = glGetAttribLocation(ShaderProg, "UV");
		it_MeshInfo->uvSecAttribLoc = glGetAttribLocation(ShaderProg, "UV_Sec");
		it_MeshInfo->tangentAttribLoc = glGetAttribLocation(ShaderProg, "Tangent");
		it_MeshInfo->binormalAttribLoc = glGetAttribLocation(ShaderProg, "Binormal");

		it_MeshInfo->matWorldViewProjUniformLoc = glGetUniformLocation(ShaderProg, "WVP");
		it_MeshInfo->matWorldUniformLoc = glGetUniformLocation(ShaderProg, "World");
#elif defined(USING_D3D11)
		if (!vsSourceP || !fsSourceP)
			exit(32);
		
		{
			it_MeshInfo->VS_blob = nullptr;
			ComPtr<ID3DBlob> errorBlob = nullptr;
			hr = D3DCompile(vstr.c_str(), vstr.size(), 0, 0, 0, "VS", "vs_5_0", 0, 0, &it_MeshInfo->VS_blob, &errorBlob);
			if (hr != S_OK) {

				if (errorBlob) {
					printf("errorBlob shader[%s]", (char*)errorBlob->GetBufferPointer());
					return;
				}

				if (it_MeshInfo->VS_blob) {
					return;
				}
			}

			hr = D3D11Device->CreateVertexShader(it_MeshInfo->VS_blob->GetBufferPointer(), it_MeshInfo->VS_blob->GetBufferSize(), 0, &it_MeshInfo->pVS);
			if (hr != S_OK) {
				printf("Error Creating Vertex Shader\n");
				return;
			}
		}

		{
			it_MeshInfo->FS_blob = nullptr;
			ComPtr<ID3DBlob> errorBlob = nullptr;
			hr = D3DCompile(fstr.c_str(), fstr.size(), 0, 0, 0, "FS", "ps_5_0", 0, 0, &it_MeshInfo->FS_blob, &errorBlob);
			if (hr != S_OK) {
				if (errorBlob) {
					printf("errorBlob shader[%s]", (char*)errorBlob->GetBufferPointer());
					return;
				}

				if (it_MeshInfo->FS_blob) {
					return;
				}
			}

			hr = D3D11Device->CreatePixelShader(it_MeshInfo->FS_blob->GetBufferPointer(), it_MeshInfo->FS_blob->GetBufferSize(), 0, &it_MeshInfo->pFS);
			if (hr != S_OK) {
				printf("Error Creating Pixel Shader\n");
				return;
			}
		}

		int offset = 0;
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName		 = "POSITION";
		elementDesc.SemanticIndex		 = 0;
		elementDesc.Format				 = DXGI_FORMAT_R32G32B32A32_FLOAT;
		elementDesc.InputSlot			 = 0;
		elementDesc.AlignedByteOffset	 = offset;
		elementDesc.InputSlotClass		 = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;
		offset += 16;
		it_MeshInfo->VertexDecl.push_back(elementDesc);

		if (pActual->VertexAttributes&xMeshGeometry::HAS_NORMAL) {
			elementDesc.SemanticName = "NORMAL";
			elementDesc.SemanticIndex = 0;
			elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = offset;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;
			offset += 16;
			it_MeshInfo->VertexDecl.push_back(elementDesc);
		}

		if (pActual->VertexAttributes&xMeshGeometry::HAS_TANGENT) {
			elementDesc.SemanticName = "TANGENT";
			elementDesc.SemanticIndex = 0;
			elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = offset;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;
			offset += 16;
			it_MeshInfo->VertexDecl.push_back(elementDesc);
		}

		if (pActual->VertexAttributes&xMeshGeometry::HAS_BINORMAL) {
			elementDesc.SemanticName = "BINORMAL";
			elementDesc.SemanticIndex = 0;
			elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = offset;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;
			offset += 16;
			it_MeshInfo->VertexDecl.push_back(elementDesc);
		}

		if (pActual->VertexAttributes&xMeshGeometry::HAS_TEXCOORD0) {
			elementDesc.SemanticName = "TEXCOORD";
			elementDesc.SemanticIndex = 0;
			elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = offset;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;
			offset += 8;
			it_MeshInfo->VertexDecl.push_back(elementDesc);
		}

		hr = D3D11Device->CreateInputLayout(&it_MeshInfo->VertexDecl[0], it_MeshInfo->VertexDecl.size(), it_MeshInfo->VS_blob->GetBufferPointer(), it_MeshInfo->VS_blob->GetBufferSize(), &it_MeshInfo->Layout);
		if (hr != S_OK) {
			printf("Error Creating Input Layout\n");
			return;
		}
		D3D11DeviceContext->IASetInputLayout(it_MeshInfo->Layout.Get());

		D3D11_BUFFER_DESC bdesc = { 0 };
		bdesc.Usage = D3D11_USAGE_DEFAULT;
		bdesc.ByteWidth = sizeof(Mesh::CBuffer);
		bdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		hr = D3D11Device->CreateBuffer(&bdesc, 0, it_MeshInfo->pd3dConstantBuffer.GetAddressOf());
		if (hr != S_OK) {
			printf("Error Creating Buffer Layout\n");
			return;
		}
#endif
		int NumMaterials = pActual->MaterialList.Materials.size();
		int NumFaceIndices = pActual->MaterialList.FaceIndices.size();
		
		for (int j = 0; j < NumMaterials; j++) {
			xSubsetInfo *subinfo = &it->Subsets[j];
			xMaterial *material = &pActual->MaterialList.Materials[j];
			SubSetInfo *it_subsetinfo = &it_MeshInfo->SubSets[j];

			for (unsigned int k = 0; k<material->EffectInstance.pDefaults.size(); k++) {
				xEffectDefault *mDef = &material->EffectInstance.pDefaults[k];
				if (mDef->Type == xF::xEFFECTENUM::STDX_STRINGS) {
				#if DEBUG_MODEL
					std::cout << "[" <<mDef->NameParam << "]" << std::endl;
				#endif
					if(mDef->NameParam=="diffuseMap"){
						std::string path = mDef->CaseString;
						int firstSlash = path.find_last_of("\\") + 1;
						int Length = path.size() - firstSlash;
						path = path.substr(firstSlash, Length);
					#if DEBUG_MODEL
						std::cout << "path[" << path << "]" << std::endl;
					#endif
						bool found=false;
						for(std::size_t f=0;f<Textures.size();f++){
							Texture *ttex = Textures[f];
							std::string ttstr = std::string(ttex->optname);
							if(ttstr== path){
#ifdef USING_OPENGL_ES
								it_subsetinfo->IdTex = ttex->id;
								it_subsetinfo->IdTexUniformLoc = glGetUniformLocation(it->ShaderProg, "diffuse");
#elif defined(USING_D3D11)
								it_subsetinfo->DiffTex = ttex;
#endif
								found=true;
								break;
							}
						}
						if(!found){
#ifdef USING_OPENGL_ES
							Texture *tex = new TextureGL;
#elif defined(USING_D3D11)
							Texture *tex = new TextureD3D;
#endif
							unsigned int id = tex->LoadTexture((char*)path.c_str());

							for (unsigned int m = 0; m < material->EffectInstance.pDefaults.size(); m++) {
								xEffectDefault *mDef_2 = &material->EffectInstance.pDefaults[m];
								if(mDef_2->Type== xF::xEFFECTENUM::STDX_DWORDS){
									if (mDef_2->NameParam == "Tiled") {
										int value = mDef_2->CaseDWORD;
										unsigned int params = TEXT_BASIC_PARAMS::MIPMAPS;
										if(value)
											params |= TEXT_BASIC_PARAMS::TILED;
										else
											params |= TEXT_BASIC_PARAMS::CLAMP_TO_EDGE;

										tex->params = params;
										tex->SetTextureParams(id);
									}
								}
							}

							if (id != -1) {
							#if DEBUG_MODEL
								std::cout << "Texture Loaded index " << id << std::endl;
							#endif
								Textures.push_back(tex);
#ifdef USING_OPENGL_ES
								it_subsetinfo->IdTex = id;
								it_subsetinfo->IdTexUniformLoc = glGetUniformLocation(it->ShaderProg, "diffuse");
#elif defined(USING_D3D11)
								it_subsetinfo->DiffTex = tex;
#endif
							}
							else {
								std::cout << "Texture not Found" << std::endl;
								delete tex;
							}
						}					  
					}
				}
			}
			it_subsetinfo->NumTris   = subinfo->NumTris;
			it_subsetinfo->NumVertex = subinfo->NumVertex;			
			unsigned short *tmpIndexex = new unsigned short[it_subsetinfo->NumVertex];
			int counter = 0;
			bool first = false;
			for (int k = 0; k < NumFaceIndices; k++) {
				if (pActual->MaterialList.FaceIndices[k] == j) {
					unsigned int index = k * 3;
					if (!first) {
						it_subsetinfo->TriStart = k;
						it_subsetinfo->VertexStart = index;
						first = true;
					}
					
					#if CHANGE_TO_RH
					tmpIndexex[counter++] = pActual->Triangles[index + 2];
					tmpIndexex[counter++] = pActual->Triangles[index + 1];
					tmpIndexex[counter++] = pActual->Triangles[index];
					#else
					tmpIndexex[counter++] = pActual->Triangles[index];
					tmpIndexex[counter++] = pActual->Triangles[index + 1];
					tmpIndexex[counter++] = pActual->Triangles[index + 2];
					#endif
				}
			}
#ifdef USING_OPENGL_ES
			glGenBuffers(1, &it_subsetinfo->Id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, it_subsetinfo->Id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, it_subsetinfo->NumTris * 3 * sizeof(unsigned short), tmpIndexex, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#elif defined(USING_D3D11)
			D3D11_BUFFER_DESC bdesc = { 0 };
			bdesc.ByteWidth = it_subsetinfo->NumTris * 3 * sizeof(USHORT);
			bdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			D3D11_SUBRESOURCE_DATA subData = { tmpIndexex, 0, 0 };

			hr = D3D11Device->CreateBuffer(&bdesc, &subData, &it_subsetinfo->IB);
			if (hr != S_OK) {
				printf("Error Creating Index Buffer\n");
				return;
			}
#endif
			delete[] tmpIndexex;
		}		

		it_MeshInfo->VertexSize = it->VertexSize;
#ifdef USING_OPENGL_ES
		glGenBuffers(1, &it_MeshInfo->Id);
		glBindBuffer(GL_ARRAY_BUFFER, it_MeshInfo->Id);
		glBufferData(GL_ARRAY_BUFFER, pActual->NumVertices*it->VertexSize, &it->pData[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
#elif defined(USING_D3D11)
		bdesc = { 0 };
		bdesc.ByteWidth = pActual->NumVertices*it->VertexSize;
		bdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		D3D11_SUBRESOURCE_DATA subData = { &it->pData[0], 0, 0 };
		hr = D3D11Device->CreateBuffer(&bdesc, &subData, &it_MeshInfo->VB);
		if (hr != S_OK) {
			printf("Error Creating Vertex Buffer\n");
			return;
		}
#endif

#if CHANGE_TO_RH
		for (std::size_t a = 0; a < pActual->Triangles.size(); a += 3) {
			unsigned short i0 = pActual->Triangles[a + 0];
			unsigned short i2 = pActual->Triangles[a + 2];
			pActual->Triangles[a + 0] = i2;
			pActual->Triangles[a + 2] = i0;
		}	
#endif

#ifdef USING_OPENGL_ES
		glGenBuffers(1, &it_MeshInfo->IdIBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, it_MeshInfo->IdIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, pActual->Triangles.size() * sizeof(unsigned short), &pActual->Triangles[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#elif defined(USING_D3D11)
		bdesc = { 0 };
		bdesc.ByteWidth = pActual->Triangles.size() * sizeof(unsigned short);
		bdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		subData = { &pActual->Triangles[0], 0, 0 };

		hr = D3D11Device->CreateBuffer(&bdesc, &subData, &it_MeshInfo->IB);
		if (hr != S_OK) {
			printf("Error Creating Index Buffer\n");
			return;
		}
#endif
	}

	XMatIdentity(transform);
}

void Mesh::GatherInfo() {
	for (std::size_t i = 0; i < xFile.MeshInfo.size(); i++) {
		xFinalGeometry *it = &xFile.MeshInfo[i];
		xMeshGeometry *pActual = &xFile.XMeshDataBase[0]->Geometry[i];
		MeshInfo tmp;
		int NumMaterials = pActual->MaterialList.Materials.size();
		for (int j = 0; j < NumMaterials; j++) {
			xSubsetInfo *subinfo = &it->Subsets[j];
			SubSetInfo stmp;
			tmp.SubSets.push_back(stmp);
		}	
		Info.push_back(tmp);
	}
}

void Mesh::Transform(float *t) {
	transform = t;
}

void Mesh::Draw(float *t, float *vp) {

	if (t)
		transform = t;

#ifdef USING_OPENGL_ES
	for (std::size_t i = 0; i <  xFile.MeshInfo.size();  i++) {
		XMATRIX44 VP = XMATRIX44(vp);
		XMATRIX44 WVP = transform*VP;

		MeshInfo  *it_MeshInfo = &Info[i];
		xMeshGeometry *pActual = &xFile.XMeshDataBase[0]->Geometry[i];


		glUseProgram(it_MeshInfo->ShaderProg);

		glUniformMatrix4fv(it_MeshInfo->matWorldUniformLoc, 1, GL_FALSE, &transform.m[0][0]);
		glUniformMatrix4fv(it_MeshInfo->matWorldViewProjUniformLoc, 1, GL_FALSE, &WVP.m[0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, it_MeshInfo->Id);

		glEnableVertexAttribArray(it_MeshInfo->vertexAttribLoc);
		glVertexAttribPointer(it_MeshInfo->vertexAttribLoc, 4, GL_FLOAT, GL_FALSE, it_MeshInfo->VertexSize, BUFFER_OFFSET(0));
	
		if (it_MeshInfo->normalAttribLoc != -1){
			glEnableVertexAttribArray(it_MeshInfo->normalAttribLoc);
			glVertexAttribPointer(it_MeshInfo->normalAttribLoc, 4, GL_FLOAT, GL_FALSE, it_MeshInfo->VertexSize, BUFFER_OFFSET(16));
		}

		if (it_MeshInfo->tangentAttribLoc != -1){
			glEnableVertexAttribArray(it_MeshInfo->tangentAttribLoc);
			glVertexAttribPointer(it_MeshInfo->tangentAttribLoc, 4, GL_FLOAT, GL_FALSE, it_MeshInfo->VertexSize, BUFFER_OFFSET(32));
		}

		if (it_MeshInfo->binormalAttribLoc != -1){
			glEnableVertexAttribArray(it_MeshInfo->binormalAttribLoc);
			glVertexAttribPointer(it_MeshInfo->binormalAttribLoc, 4, GL_FLOAT, GL_FALSE, it_MeshInfo->VertexSize, BUFFER_OFFSET(48));
		}

		if (it_MeshInfo->uvAttribLoc != -1){
			int offset = 32;
			if(pActual->VertexAttributes&xMeshGeometry::HAS_TANGENT){
				offset = 64;
			}
			glEnableVertexAttribArray(it_MeshInfo->uvAttribLoc);
			glVertexAttribPointer(it_MeshInfo->uvAttribLoc, 2, GL_FLOAT, GL_FALSE, it_MeshInfo->VertexSize, BUFFER_OFFSET(offset));
		}

		if (it_MeshInfo->uvSecAttribLoc != -1){
			int offset = 40;
			if (pActual->VertexAttributes&xMeshGeometry::HAS_TANGENT) {
				offset = 72;
			}
			glEnableVertexAttribArray(it_MeshInfo->uvSecAttribLoc);
			glVertexAttribPointer(it_MeshInfo->uvSecAttribLoc, 2, GL_FLOAT, GL_FALSE, it_MeshInfo->VertexSize, BUFFER_OFFSET(offset));
		}
	
		for(std::size_t k = 0; k < it_MeshInfo->SubSets.size(); k++){
			//xSubsetInfo *sbIfo = &it->Subsets[k];
			SubSetInfo *sub_info = &it_MeshInfo->SubSets[k];
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sub_info->Id);

			if(sub_info->IdTex!=-1){
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, sub_info->IdTex);
				glUniform1i(sub_info->IdTexUniformLoc, 0);
			}

			glDrawElements(GL_TRIANGLES, sub_info->NumVertex, GL_UNSIGNED_SHORT, 0);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(it_MeshInfo->vertexAttribLoc);
		if (it_MeshInfo->normalAttribLoc != -1) {
			glDisableVertexAttribArray(it_MeshInfo->normalAttribLoc);
		}
		if (it_MeshInfo->tangentAttribLoc != -1) {
			glDisableVertexAttribArray(it_MeshInfo->tangentAttribLoc);
		}
		if (it_MeshInfo->binormalAttribLoc != -1) {
			glDisableVertexAttribArray(it_MeshInfo->binormalAttribLoc);
		}
		if (it_MeshInfo->uvAttribLoc != -1) {
			glDisableVertexAttribArray(it_MeshInfo->uvAttribLoc);
		}
		if (it_MeshInfo->uvSecAttribLoc != -1) {
			glDisableVertexAttribArray(it_MeshInfo->uvSecAttribLoc);
		}
		
		glUseProgram(0);
	}
#elif defined(USING_D3D11)
	for (std::size_t i = 0; i < xFile.MeshInfo.size(); i++) {
		MeshInfo  *it_MeshInfo = &Info[i];
		xMeshGeometry *pActual = &xFile.XMeshDataBase[0]->Geometry[i];

		XMATRIX44 VP = XMATRIX44(vp);
		XMATRIX44 WVP = transform*VP;
		it_MeshInfo->CnstBuffer.WVP = WVP;
		it_MeshInfo->CnstBuffer.World = transform;

		UINT stride = it_MeshInfo->VertexSize;
		UINT offset = 0;
		D3D11DeviceContext->VSSetShader(it_MeshInfo->pVS.Get(), 0, 0);
		D3D11DeviceContext->PSSetShader(it_MeshInfo->pFS.Get(), 0, 0);

		D3D11DeviceContext->IASetVertexBuffers(0, 1, it_MeshInfo->VB.GetAddressOf(), &stride, &offset);

		D3D11DeviceContext->UpdateSubresource(it_MeshInfo->pd3dConstantBuffer.Get(), 0, 0, &it_MeshInfo->CnstBuffer, 0, 0);		
		D3D11DeviceContext->VSSetConstantBuffers(0, 1, it_MeshInfo->pd3dConstantBuffer.GetAddressOf());
		D3D11DeviceContext->PSSetConstantBuffers(0, 1, it_MeshInfo->pd3dConstantBuffer.GetAddressOf());

		for (std::size_t k = 0; k < it_MeshInfo->SubSets.size(); k++) {
			SubSetInfo *it_subsetinfo = &it_MeshInfo->SubSets[k];

			TextureD3D *texd3d = dynamic_cast<TextureD3D*>(it_subsetinfo->DiffTex);
			D3D11DeviceContext->PSSetShaderResources(0, 1, texd3d->pSRVTex.GetAddressOf());
			D3D11DeviceContext->PSSetSamplers(0, 1, texd3d->pSampler.GetAddressOf());

			D3D11DeviceContext->IASetIndexBuffer(it_subsetinfo->IB.Get(), DXGI_FORMAT_R16_UINT, 0);

			D3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			D3D11DeviceContext->DrawIndexed(it_subsetinfo->NumVertex, 0, 0);
		}
	}
#endif
}

void Mesh::Destroy() {
#ifdef USING_OPENGL_ES
	for (std::size_t i = 0; i < xFile.MeshInfo.size(); i++) {
		MeshInfo  *it_MeshInfo = &Info[i];
		glDeleteProgram(it_MeshInfo->ShaderProg);
	}
#endif
}