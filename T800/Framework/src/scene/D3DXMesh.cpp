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

#include <video/BaseDriver.h>
#include <scene/D3DXMesh.h>
#include <iostream>

#include <video/D3DXShader.h>


#define CHANGE_TO_RH 0
#define DEBUG_MODEL 0


extern ComPtr<ID3D11Device>            D3D11Device;
extern ComPtr<ID3D11DeviceContext>     D3D11DeviceContext;


void D3DXMesh::Create(char *filename) {

	std::string fname = std::string(filename);
	if (xFile.LoadXFile(fname)) {
		cout << " Load " << endl;
	}
	else {
		cout << " Failed " << endl;
	}

	HRESULT hr;

	GatherInfo();

	for (std::size_t i = 0; i < xFile.MeshInfo.size(); i++) {
		xFinalGeometry *it = &xFile.MeshInfo[i];
		xMeshGeometry *pActual = &xFile.XMeshDataBase[0]->Geometry[i];
		MeshInfo  *it_MeshInfo = &Info[i];

		D3DXShader *s = dynamic_cast<D3DXShader*>(g_pBaseDriver->GetShaderSig(it_MeshInfo->SubSets[0].Sig));

		D3D11DeviceContext->IASetInputLayout(s->Layout.Get());

		D3D11_BUFFER_DESC bdesc = { 0 };
		bdesc.Usage = D3D11_USAGE_DEFAULT;
		bdesc.ByteWidth = sizeof(D3DXMesh::CBuffer);
		bdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		hr = D3D11Device->CreateBuffer(&bdesc, 0, it_MeshInfo->pd3dConstantBuffer.GetAddressOf());
		if (hr != S_OK) {
			printf("Error Creating Buffer Layout\n");
			return;
		}

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

						it_subsetinfo->DiffuseId = LoadTex(path, material,&it_subsetinfo->DiffuseTex);

					}

					if (mDef->NameParam == "specularMap") {
						std::string path = RemovePath(mDef->CaseString);
#if DEBUG_MODEL
						std::cout << "path[" << path << "]" << std::endl;
#endif
						it_subsetinfo->SpecularId = LoadTex(path, material,&it_subsetinfo->SpecularTex);
					}

					if (mDef->NameParam == "glossMap") {
						std::string path = RemovePath(mDef->CaseString);
#if DEBUG_MODEL
						std::cout << "path[" << path << "]" << std::endl;
#endif
						it_subsetinfo->GlossfId = LoadTex(path, material,&it_subsetinfo->GlossfTex);
					}

					if (mDef->NameParam == "normalMap") {
						std::string path = RemovePath(mDef->CaseString);
#if DEBUG_MODEL
						std::cout << "path[" << path << "]" << std::endl;
#endif
						it_subsetinfo->NormalId = LoadTex(path, material,&it_subsetinfo->NormalTex);;
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

			D3D11_BUFFER_DESC bdesc = { 0 };
			bdesc.ByteWidth = it_subsetinfo->NumTris * 3 * sizeof(USHORT);
			bdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			D3D11_SUBRESOURCE_DATA subData = { tmpIndexex, 0, 0 };

			hr = D3D11Device->CreateBuffer(&bdesc, &subData, &it_subsetinfo->IB);
			if (hr != S_OK) {
				printf("Error Creating Index Buffer\n");
				return;
			}

			delete[] tmpIndexex;
		}



		it_MeshInfo->VertexSize = it->VertexSize;

		bdesc = { 0 };
		bdesc.ByteWidth = pActual->NumVertices*it->VertexSize;
		bdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		D3D11_SUBRESOURCE_DATA subData = { &it->pData[0], 0, 0 };
		hr = D3D11Device->CreateBuffer(&bdesc, &subData, &it_MeshInfo->VB);
		if (hr != S_OK) {
			printf("Error Creating Vertex Buffer\n");
			return;
		}

#if CHANGE_TO_RH
		for (std::size_t a = 0; a < pActual->Triangles.size(); a += 3) {
			unsigned short i0 = pActual->Triangles[a + 0];
			unsigned short i2 = pActual->Triangles[a + 2];
			pActual->Triangles[a + 0] = i2;
			pActual->Triangles[a + 2] = i0;
		}
#endif

		bdesc = { 0 };
		bdesc.ByteWidth = pActual->Triangles.size() * sizeof(unsigned short);
		bdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		subData = { &pActual->Triangles[0], 0, 0 };

		hr = D3D11Device->CreateBuffer(&bdesc, &subData, &it_MeshInfo->IB);
		if (hr != S_OK) {
			printf("Error Creating Index Buffer\n");
			return;
		}
	}

	XMatIdentity(transform);
}

void D3DXMesh::GatherInfo() {

	char *vsSourceP = file2string("Shaders/VS_Mesh.hlsl");
	char *fsSourceP = file2string("Shaders/FS_Mesh.hlsl");

	std::string vstr = std::string(vsSourceP);
	std::string fstr = std::string(fsSourceP);

	free(vsSourceP);
	free(fsSourceP);

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

			CurrSig |= Signature::GBUFF_PASS;
			g_pBaseDriver->CreateShader(vstr, fstr, CurrSig);
			CurrSig ^= Signature::GBUFF_PASS;

			CurrSig |= Signature::SHADOW_MAP_PASS;
			g_pBaseDriver->CreateShader(vstr, fstr, CurrSig);
			
		}

		Info.push_back(tmp);
	}
}

int	 D3DXMesh::LoadTex(std::string p, xF::xMaterial *mat, D3DXTexture** tex) {
	int id = g_pBaseDriver->CreateTexture(p);
	*tex = dynamic_cast<D3DXTexture*>(g_pBaseDriver->GetTexture(id));
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

	(*tex)->params = params;
	(*tex)->SetTextureParams();

	if (id != -1) {
#if DEBUG_MODEL
		std::cout << "Texture Loaded index " << id << std::endl;
#endif
	}else {
		std::cout << "Texture [" << p << "] not Found" << std::endl;
	}

	return id;
}

void D3DXMesh::Transform(float *t) {
	transform = t;
}

void D3DXMesh::Draw(float *t, float *vp) {

	if (t)
		transform = t;

	Camera *pActualCamera = pScProp->pCameras[0];

		for (std::size_t i = 0; i < xFile.MeshInfo.size(); i++) {
			MeshInfo  *it_MeshInfo = &Info[i];
			xMeshGeometry *pActual = &xFile.XMeshDataBase[0]->Geometry[i];

			XMATRIX44 VP = pActualCamera->VP;
			XMATRIX44 WVP = transform*VP;
			XMATRIX44 WorldView = transform*pActualCamera->View;
			XVECTOR3 infoCam = XVECTOR3(pActualCamera->NPlane, pActualCamera->FPlane, pActualCamera->Fov, 1.0f);

			it_MeshInfo->CnstBuffer.WVP = WVP;
			it_MeshInfo->CnstBuffer.World = transform;
			it_MeshInfo->CnstBuffer.WorldView = WorldView;
			it_MeshInfo->CnstBuffer.Light0Pos = pScProp->Lights[0].Position;
			it_MeshInfo->CnstBuffer.Light0Col = pScProp->Lights[0].Color;
			it_MeshInfo->CnstBuffer.CameraPos = pActualCamera->Eye;
			it_MeshInfo->CnstBuffer.Ambient = pScProp->AmbientColor;
			it_MeshInfo->CnstBuffer.CameraInfo = infoCam;

			UINT stride = it_MeshInfo->VertexSize;
			UINT offset = 0;

			int Sig = -1;
			D3DXShader *s = 0;
			D3DXShader *last = (D3DXShader*)32;
			D3D11DeviceContext->IASetVertexBuffers(0, 1, it_MeshInfo->VB.GetAddressOf(), &stride, &offset);

			for (std::size_t k = 0; k < it_MeshInfo->SubSets.size(); k++) {
				bool update = false;
				SubSetInfo *sub_info = &it_MeshInfo->SubSets[k];

				unsigned int sig = sub_info->Sig;
				sig |= gSig;
				s = dynamic_cast<D3DXShader*>(g_pBaseDriver->GetShaderSig(sig));

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

				D3D11DeviceContext->PSSetShaderResources(0, 1, sub_info->DiffuseTex->pSRVTex.GetAddressOf());

				if (s->Sig&Signature::SPECULAR_MAP){
					D3D11DeviceContext->PSSetShaderResources(1, 1, sub_info->SpecularTex->pSRVTex.GetAddressOf());
				}

				if (s->Sig&Signature::GLOSS_MAP) {;
					D3D11DeviceContext->PSSetShaderResources(2, 1, sub_info->GlossfTex->pSRVTex.GetAddressOf());
				}

				if (s->Sig&Signature::NORMAL_MAP) {
					D3D11DeviceContext->PSSetShaderResources(3, 1, sub_info->NormalTex->pSRVTex.GetAddressOf());
				}

				D3D11DeviceContext->PSSetSamplers(0, 1, sub_info->DiffuseTex->pSampler.GetAddressOf());

				D3D11DeviceContext->IASetIndexBuffer(sub_info->IB.Get(), DXGI_FORMAT_R16_UINT, 0);

				D3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				D3D11DeviceContext->DrawIndexed(sub_info->NumVertex, 0, 0);
				last = s;
				}
			}

}

void D3DXMesh::Destroy() {

}
