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

#include <scene\Mesh.h>
#include <iostream>

#if defined(USING_OPENGL_ES) || defined(USING_OPENGL)
#include <video/GLShader.h>
#elif defined(USING_D3D11)
#include <video/D3DXShader.h>
#endif

#define CHANGE_TO_RH 0
#define DEBUG_MODEL 0

#ifdef USING_D3D11
extern ComPtr<ID3D11Device>            D3D11Device;
extern ComPtr<ID3D11DeviceContext>     D3D11DeviceContext;
#endif

std::string RemovePath(std::string p) {
	std::string path = p;
	int firstSlash = path.find_last_of("\\") + 1;
	int Length = path.size() - firstSlash;
	path = path.substr(firstSlash, Length);
	return path;
}

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

#if defined(USING_OPENGL_ES)||defined(USING_OPENGL)

#elif defined(USING_D3D11)		
		D3DXShader *s = dynamic_cast<D3DXShader*>(g_pBaseDriver->GetShaderSig(it_MeshInfo->SubSets[0].Sig));

		D3D11DeviceContext->IASetInputLayout(s->Layout.Get());

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
						std::string path = RemovePath(mDef->CaseString);
					#if DEBUG_MODEL
						std::cout << "path[" << path << "]" << std::endl;
					#endif
#if defined(USING_OPENGL_ES)||defined(USING_OPENGL)
						TextureGL *tex = dynamic_cast<TextureGL*>(LoadTex(path, material));
						it_subsetinfo->IdDiffuseTex = tex->id;
#elif defined(USING_D3D11)
						TextureD3D *tex = dynamic_cast<TextureD3D*>(LoadTex(path, material));
						it_subsetinfo->DiffuseTex = tex;
#endif									  
					}

					if (mDef->NameParam == "specularMap") {
						std::string path = RemovePath(mDef->CaseString);
#if DEBUG_MODEL
						std::cout << "path[" << path << "]" << std::endl;
#endif
#if defined(USING_OPENGL_ES)||defined(USING_OPENGL)
						TextureGL *tex = dynamic_cast<TextureGL*>(LoadTex(path, material));
						it_subsetinfo->IdSpecularTex = tex->id;
#elif defined(USING_D3D11)
						TextureD3D *tex = dynamic_cast<TextureD3D*>(LoadTex(path, material));
						it_subsetinfo->SpecularTex = tex;
#endif									  
					}

					if (mDef->NameParam == "glossMap") {
						std::string path = RemovePath(mDef->CaseString);
#if DEBUG_MODEL
						std::cout << "path[" << path << "]" << std::endl;
#endif
#if defined(USING_OPENGL_ES)||defined(USING_OPENGL)
						TextureGL *tex = dynamic_cast<TextureGL*>(LoadTex(path, material));
						it_subsetinfo->IdGlossTex = tex->id;
#elif defined(USING_D3D11)
						TextureD3D *tex = dynamic_cast<TextureD3D*>(LoadTex(path, material));
						it_subsetinfo->GlossfTex = tex;
#endif									  
					}

					if (mDef->NameParam == "normalMap") {
						std::string path = RemovePath(mDef->CaseString);
#if DEBUG_MODEL
						std::cout << "path[" << path << "]" << std::endl;
#endif
#if defined(USING_OPENGL_ES)||defined(USING_OPENGL)
						TextureGL *tex = dynamic_cast<TextureGL*>(LoadTex(path, material));
						it_subsetinfo->IdNormalTex = tex->id;
#elif defined(USING_D3D11)
						TextureD3D *tex = dynamic_cast<TextureD3D*>(LoadTex(path, material));
						it_subsetinfo->NormalTex = tex;
#endif									  
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
#if defined(USING_OPENGL_ES)||defined(USING_OPENGL)
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
#if defined(USING_OPENGL_ES)||defined(USING_OPENGL)
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

#if defined(USING_OPENGL_ES)||defined(USING_OPENGL)
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
#if defined(USING_OPENGL_ES)||defined(USING_OPENGL)
	char *vsSourceP = file2string("Shaders/VS_Mesh.glsl");
	char *fsSourceP = file2string("Shaders/FS_Mesh.glsl");
#elif defined(USING_D3D11)
	char *vsSourceP = file2string("Shaders/VS_Mesh.hlsl");
	char *fsSourceP = file2string("Shaders/FS_Mesh.hlsl");
#endif
	

	for (std::size_t i = 0; i < xFile.MeshInfo.size(); i++) {
		xFinalGeometry *it = &xFile.MeshInfo[i];
		xMeshGeometry *pActual = &xFile.XMeshDataBase[0]->Geometry[i];
		int Sig = 0;

		if (pActual->VertexAttributes&xMeshGeometry::HAS_NORMAL)
			Sig |= Signature::HAS_NORMALS;
		if (pActual->VertexAttributes&xMeshGeometry::HAS_TEXCOORD0)
			Sig |= Signature::HAS_TEXCOORDS0;
		if (pActual->VertexAttributes&xMeshGeometry::HAS_TEXCOORD1)
			Sig |= Signature::HAS_TEXCOORDS1;
		if (pActual->VertexAttributes&xMeshGeometry::HAS_TANGENT)
			Sig |= Signature::HAS_TANGENTS;
		if (pActual->VertexAttributes&xMeshGeometry::HAS_BINORMAL)
			Sig |= Signature::HAS_BINORMALS;

		MeshInfo tmp;
		int NumMaterials = pActual->MaterialList.Materials.size();
		for (int j = 0; j < NumMaterials; j++) {
			int CurrSig = Sig;
			xSubsetInfo *subinfo = &it->Subsets[j];
			xMaterial *material = &pActual->MaterialList.Materials[j];
			SubSetInfo stmp;
			
			std::string vstr = std::string(vsSourceP);
			std::string fstr = std::string(fsSourceP);
			std::string Defines = "";

			for (unsigned int k = 0; k < material->EffectInstance.pDefaults.size(); k++) {
				xEffectDefault *mDef = &material->EffectInstance.pDefaults[k];
				if (mDef->Type == xF::xEFFECTENUM::STDX_STRINGS) {
					if (mDef->NameParam == "diffuseMap") {
						CurrSig |= Signature::DIFFUSE_MAP;
					}

					if (mDef->NameParam == "specularMap") {
						CurrSig |= Signature::SPECULAR_MAP;
					}

					if (mDef->NameParam == "glossMap") {
						CurrSig |= Signature::GLOSS_MAP;
					}

					if (mDef->NameParam == "normalMap") {
						CurrSig |= Signature::NORMAL_MAP;
					}
				}

				if (mDef->Type == xF::xEFFECTENUM::STDX_DWORDS) {
					if (mDef->NameParam == "NoLighting") {
						if (mDef->CaseDWORD == 1) {
							CurrSig |= Signature::NO_LIGHT_AT_ALL;
						}
					}
				}				
			}

			g_pBaseDriver->CreateShader(vstr, fstr, CurrSig);
			stmp.Sig = CurrSig;
			tmp.SubSets.push_back(stmp);
		}		
		
		Info.push_back(tmp);
	}


	free(vsSourceP);
	free(fsSourceP);
}

Texture* Mesh::LoadTex(std::string p, xF::xMaterial *mat) {

	for (std::size_t f = 0; f < Textures.size(); f++) {
		Texture *ttex = Textures[f];
		std::string ttstr = std::string(ttex->optname);
		if (ttstr == p) {
			return ttex;
		}
	}

#if defined(USING_OPENGL_ES)||defined(USING_OPENGL)
		Texture *tex = new TextureGL;
#elif defined(USING_D3D11)
		Texture *tex = new TextureD3D;
#endif
		unsigned int id = tex->LoadTexture((char*)p.c_str());

		bool tiled = false;
		for (unsigned int m = 0; m < mat->EffectInstance.pDefaults.size(); m++) {
			xEffectDefault *mDef_2 = &mat->EffectInstance.pDefaults[m];
			if (mDef_2->Type == xF::xEFFECTENUM::STDX_DWORDS) {
				if (mDef_2->NameParam == "Tiled") {					
					if (mDef_2->CaseDWORD == 1) {
						tiled = true;
					}				
					break;
				}				
			}
		}

		unsigned int params = TEXT_BASIC_PARAMS::MIPMAPS;

		if (tiled)
			params |= TEXT_BASIC_PARAMS::TILED;
		else
			params |= TEXT_BASIC_PARAMS::CLAMP_TO_EDGE;

		tex->params = params;
		tex->SetTextureParams(id);

		if (id != -1) {
#if DEBUG_MODEL
			std::cout << "Texture Loaded index " << id << std::endl;
#endif
			Textures.push_back(tex);
			return tex;
		}
		else {
			std::cout << "Texture not Found" << std::endl;		
		}
	
		return 0;
}

void Mesh::Transform(float *t) {
	transform = t;
}

void Mesh::Draw(float *t, float *vp) {

	if (t)
		transform = t;

#ifdef USING_OPENGL
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,  GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, drawBuffers);
#endif

#if defined(USING_OPENGL_ES)||defined(USING_OPENGL)
	for (std::size_t i = 0; i <  xFile.MeshInfo.size();  i++) {
		XMATRIX44 VP = XMATRIX44(vp);
		XMATRIX44 WVP = transform*VP;

		MeshInfo  *it_MeshInfo = &Info[i];
		xMeshGeometry *pActual = &xFile.XMeshDataBase[0]->Geometry[i];

		GLShader *s = 0;
		GLShader *last = (GLShader*)32;
		for(std::size_t k = 0; k < it_MeshInfo->SubSets.size(); k++){
			SubSetInfo *sub_info = &it_MeshInfo->SubSets[k];
			bool update = false;
			s = dynamic_cast<GLShader*>(g_pBaseDriver->GetShaderSig(sub_info->Sig));

			if (s != last)
				update = true;

			if (update) {
				glUseProgram(s->ShaderProg);

				glUniformMatrix4fv(s->matWorldUniformLoc, 1, GL_FALSE, &transform.m[0][0]);
				glUniformMatrix4fv(s->matWorldViewProjUniformLoc, 1, GL_FALSE, &WVP.m[0][0]);

				if (s->Light0Pos_Loc != -1) {
					glUniform4fv(s->Light0Pos_Loc, 1, &pScProp->Lights[0].Position.v[0]);
				}

				if (s->Light0Color_Loc != -1) {
					glUniform4fv(s->Light0Color_Loc, 1, &pScProp->Lights[0].Color.v[0]);
				}

				if (s->CameraPos_Loc != -1) {
					glUniform4fv(s->CameraPos_Loc, 1, &pScProp->pCameras[0]->Eye.v[0]);
				}

				if (s->Ambient_loc != -1) {
					glUniform4fv(s->Ambient_loc, 1, &pScProp->AmbientColor.v[0]);
				}

				glBindBuffer(GL_ARRAY_BUFFER, it_MeshInfo->Id);

				glEnableVertexAttribArray(s->vertexAttribLoc);
				glVertexAttribPointer(s->vertexAttribLoc, 4, GL_FLOAT, GL_FALSE, it_MeshInfo->VertexSize, BUFFER_OFFSET(0));

				if (s->normalAttribLoc != -1) {
					glEnableVertexAttribArray(s->normalAttribLoc);
					glVertexAttribPointer(s->normalAttribLoc, 4, GL_FLOAT, GL_FALSE, it_MeshInfo->VertexSize, BUFFER_OFFSET(16));
				}

				if (s->tangentAttribLoc != -1) {
					glEnableVertexAttribArray(s->tangentAttribLoc);
					glVertexAttribPointer(s->tangentAttribLoc, 4, GL_FLOAT, GL_FALSE, it_MeshInfo->VertexSize, BUFFER_OFFSET(32));
				}

				if (s->binormalAttribLoc != -1) {
					glEnableVertexAttribArray(s->binormalAttribLoc);
					glVertexAttribPointer(s->binormalAttribLoc, 4, GL_FLOAT, GL_FALSE, it_MeshInfo->VertexSize, BUFFER_OFFSET(48));
				}

				if (s->uvAttribLoc != -1) {
					int offset = 32;
					if (pActual->VertexAttributes&xMeshGeometry::HAS_TANGENT) {
						offset = 64;
					}
					glEnableVertexAttribArray(s->uvAttribLoc);
					glVertexAttribPointer(s->uvAttribLoc, 2, GL_FLOAT, GL_FALSE, it_MeshInfo->VertexSize, BUFFER_OFFSET(offset));
				}

				if (s->uvSecAttribLoc != -1) {
					int offset = 40;
					if (pActual->VertexAttributes&xMeshGeometry::HAS_TANGENT) {
						offset = 72;
					}
					glEnableVertexAttribArray(s->uvSecAttribLoc);
					glVertexAttribPointer(s->uvSecAttribLoc, 2, GL_FLOAT, GL_FALSE, it_MeshInfo->VertexSize, BUFFER_OFFSET(offset));
				}
				
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sub_info->Id);
			
			int slot_active = GL_TEXTURE0;
			int index = 0;
			if(sub_info->IdDiffuseTex!=-1){
				glActiveTexture(slot_active);
				glBindTexture(GL_TEXTURE_2D, sub_info->IdDiffuseTex);
				glUniform1i(s->DiffuseTex_loc, index);
				slot_active++;
				index++;
			}
			
			if (sub_info->IdSpecularTex != -1) {
				glActiveTexture(slot_active);
				glBindTexture(GL_TEXTURE_2D, sub_info->IdSpecularTex);
				glUniform1i(s->SpecularTex_loc, index);
				slot_active++;
				index++;
			}

			if (sub_info->IdGlossTex != -1) {
				glActiveTexture(slot_active);
				glBindTexture(GL_TEXTURE_2D, sub_info->IdGlossTex);
				glUniform1i(s->GlossTex_loc, index);
				slot_active++;
				index++;
			}

			if (sub_info->IdNormalTex != -1) {
				glActiveTexture(slot_active);
				glBindTexture(GL_TEXTURE_2D, sub_info->IdNormalTex);
				glUniform1i(s->NormalTex_loc, index);
				slot_active++;
				index++;
			}
			
			glDrawElements(GL_TRIANGLES, sub_info->NumVertex, GL_UNSIGNED_SHORT, 0);

			last = s;
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	
	}
#elif defined(USING_D3D11)

		for (std::size_t i = 0; i < xFile.MeshInfo.size(); i++) {
			MeshInfo  *it_MeshInfo = &Info[i];
			xMeshGeometry *pActual = &xFile.XMeshDataBase[0]->Geometry[i];
			XMATRIX44 VP = XMATRIX44(vp);
			XMATRIX44 WVP = transform*VP;

			it_MeshInfo->CnstBuffer.WVP = WVP;
			it_MeshInfo->CnstBuffer.World = transform;
			it_MeshInfo->CnstBuffer.Light0Pos = pScProp->Lights[0].Position;
			it_MeshInfo->CnstBuffer.Light0Col = pScProp->Lights[0].Color;
			it_MeshInfo->CnstBuffer.CameraPos = pScProp->pCameras[0]->Eye;
			it_MeshInfo->CnstBuffer.Ambient = pScProp->AmbientColor;

			UINT stride = it_MeshInfo->VertexSize;
			UINT offset = 0;

			int Sig = -1;
			D3DXShader *s = 0;
			D3DXShader *last = (D3DXShader*)32;
			D3D11DeviceContext->IASetVertexBuffers(0, 1, it_MeshInfo->VB.GetAddressOf(), &stride, &offset);

			for (std::size_t k = 0; k < it_MeshInfo->SubSets.size(); k++) {
				bool update = false;
				SubSetInfo *sub_info = &it_MeshInfo->SubSets[k];
				
				s = dynamic_cast<D3DXShader*>(g_pBaseDriver->GetShaderSig(sub_info->Sig));

				if(s!=last)
					update=true;
				
				if(update){
				D3D11DeviceContext->VSSetShader(s->pVS.Get(), 0, 0);
				D3D11DeviceContext->PSSetShader(s->pFS.Get(), 0, 0);
				D3D11DeviceContext->IASetInputLayout(s->Layout.Get());

				D3D11DeviceContext->UpdateSubresource(it_MeshInfo->pd3dConstantBuffer.Get(), 0, 0, &it_MeshInfo->CnstBuffer, 0, 0);
				D3D11DeviceContext->VSSetConstantBuffers(0, 1, it_MeshInfo->pd3dConstantBuffer.GetAddressOf());
				D3D11DeviceContext->PSSetConstantBuffers(0, 1, it_MeshInfo->pd3dConstantBuffer.GetAddressOf());
				}


				TextureD3D *texd3d = dynamic_cast<TextureD3D*>(sub_info->DiffuseTex);
				D3D11DeviceContext->PSSetShaderResources(0, 1, texd3d->pSRVTex.GetAddressOf());

				if (s->Sig&Signature::SPECULAR_MAP){
					texd3d = dynamic_cast<TextureD3D*>(sub_info->SpecularTex);
					D3D11DeviceContext->PSSetShaderResources(1, 1, texd3d->pSRVTex.GetAddressOf());
				}

				if (s->Sig&Signature::GLOSS_MAP) {
					texd3d = dynamic_cast<TextureD3D*>(sub_info->GlossfTex);
					D3D11DeviceContext->PSSetShaderResources(2, 1, texd3d->pSRVTex.GetAddressOf());
				}

				if (s->Sig&Signature::NORMAL_MAP) {
					texd3d = dynamic_cast<TextureD3D*>(sub_info->NormalTex);
					D3D11DeviceContext->PSSetShaderResources(3, 1, texd3d->pSRVTex.GetAddressOf());
				}

				D3D11DeviceContext->PSSetSamplers(0, 1, texd3d->pSampler.GetAddressOf());

				D3D11DeviceContext->IASetIndexBuffer(sub_info->IB.Get(), DXGI_FORMAT_R16_UINT, 0);

				D3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				D3D11DeviceContext->DrawIndexed(sub_info->NumVertex, 0, 0);
				last = s;
				}
			}
#endif
}

void Mesh::Destroy() {
#if defined(USING_OPENGL_ES)||defined(USING_OPENGL)
	for (std::size_t i = 0; i < xFile.MeshInfo.size(); i++) {
		MeshInfo  *it_MeshInfo = &Info[i];
		glDeleteProgram(it_MeshInfo->ShaderProg);
	}
#endif
}