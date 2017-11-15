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
#include <iostream>

#include <scene/windows/D3DXMesh.h>
#if defined(USING_GL_COMMON)
#include <video/GLShader.h>
#include <video/GLDriver.h>
#else
#include <video/windows/D3DXShader.h>
#include <video/windows/D3DXDriver.h>
#endif


#define CHANGE_TO_RH 0
#define DEBUG_MODEL 0


extern t800::Device*            D3D11Device;
extern t800::DeviceContext*     D3D11DeviceContext;


void D3DXMesh::Create(char *filename) {
	std::string fname = std::string(filename);
	if (xFile.LoadXFile(fname)) {
		cout << " Load " << endl;
	}
	else {
		cout << " Failed " << endl;
	}

	GatherInfo();

	for (std::size_t i = 0; i < xFile.MeshInfo.size(); i++) {
		xFinalGeometry *it = &xFile.MeshInfo[i];
		xMeshGeometry *pActual = &xFile.XMeshDataBase[0]->Geometry[i];
		MeshInfo  *it_MeshInfo = &Info[i];

		ShaderBase *s = g_pBaseDriver->GetShaderSig(it_MeshInfo->SubSets[0].Sig);

    //s->Set(*D3D11DeviceContext);

    t800::BufferDesc bdesc;
    bdesc.byteWidth = sizeof(D3DXMesh::CBuffer);
    bdesc.usage = T8_BUFFER_USAGE::DEFAULT;
    it_MeshInfo->CB = (t800::ConstantBuffer*)D3D11Device->CreateBuffer(T8_BUFFER_TYPE::CONSTANT, bdesc);
    if (false) {
      printf("Error Creating Constant Buffer\n");
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


      t800::BufferDesc bdesc;
      bdesc.byteWidth = it_subsetinfo->NumTris * 3 * sizeof(unsigned short);
      bdesc.usage = T8_BUFFER_USAGE::DEFAULT;
      it_subsetinfo->IB = (t800::IndexBuffer*)D3D11Device->CreateBuffer(T8_BUFFER_TYPE::INDEX, bdesc, tmpIndexex);
			if (false) {
				printf("Error Creating Index Buffer\n");
				return;
			}

			delete[] tmpIndexex;
		}



		it_MeshInfo->VertexSize = it->VertexSize;

    t800::BufferDesc buffdesc;
    buffdesc.byteWidth = pActual->NumVertices*it->VertexSize;
    buffdesc.usage = T8_BUFFER_USAGE::DEFAULT;
    it_MeshInfo->VB = (t800::VertexBuffer*)D3D11Device->CreateBuffer(T8_BUFFER_TYPE::VERTEX, buffdesc, &it->pData[0]);
    if (false) {
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

    buffdesc.byteWidth = pActual->Triangles.size() * sizeof(unsigned short);
    buffdesc.usage = T8_BUFFER_USAGE::DEFAULT;
    it_MeshInfo->IB = (t800::IndexBuffer*)D3D11Device->CreateBuffer(T8_BUFFER_TYPE::INDEX, buffdesc, &pActual->Triangles[0]);
    if (false) {
      printf("Error Creating Index Buffer\n");
      return;
    }
	}

	XMatIdentity(transform);
}

void D3DXMesh::GatherInfo() {
#if defined(USING_GL_COMMON)
  char *vsSourceP = file2string("Shaders/VS_Mesh.glsl");
  char *fsSourceP = file2string("Shaders/FS_Mesh.glsl");
#else
  char *vsSourceP = file2string("Shaders/VS_Mesh.hlsl");
  char *fsSourceP = file2string("Shaders/FS_Mesh.hlsl");
#endif


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

int	 D3DXMesh::LoadTex(std::string p, xF::xMaterial *mat, Texture** tex) {
	int id = g_pBaseDriver->CreateTexture(p);
	*tex = g_pBaseDriver->GetTexture(id);
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

		unsigned int stride = it_MeshInfo->VertexSize;
    unsigned int offset = 0;

		int Sig = -1;
		ShaderBase *s = 0;
    ShaderBase *last = (ShaderBase*)32;
    it_MeshInfo->VB->Set(*D3D11DeviceContext,stride,offset);

		for (std::size_t k = 0; k < it_MeshInfo->SubSets.size(); k++) {
			bool update = false;
			SubSetInfo *sub_info = &it_MeshInfo->SubSets[k];
      sub_info->IB->Set(*D3D11DeviceContext, 0, T8_IB_FORMAR::R16);

			unsigned int sig = sub_info->Sig;
			sig |= gSig;
			s = g_pBaseDriver->GetShaderSig(sig);

			if(s!=last)
				update=true;

			if(update){
        s->Set(*D3D11DeviceContext);

        it_MeshInfo->CB->UpdateFromBuffer(*D3D11DeviceContext, &it_MeshInfo->CnstBuffer.WVP[0]);
        it_MeshInfo->CB->Set(*D3D11DeviceContext);
			}

      sub_info->DiffuseTex->Set(*D3D11DeviceContext, 0,"DiffuseTex"); //No obtiene el loc
			if (s->Sig&Signature::SPECULAR_MAP){
        sub_info->SpecularTex->Set(*D3D11DeviceContext,1, "SpecularTex");
			}

			if (s->Sig&Signature::GLOSS_MAP) {;
      sub_info->GlossfTex->Set(*D3D11DeviceContext, 2,"GlossTex");
			}

			if (s->Sig&Signature::NORMAL_MAP) {
        sub_info->NormalTex->Set(*D3D11DeviceContext,3,"NormalTex"); 
			}

			if(EnvMap){
        EnvMap->Set(*D3D11DeviceContext, 4,"texEnv");
			}
      sub_info->DiffuseTex->SetSampler(*D3D11DeviceContext);


      D3D11DeviceContext->SetPrimitiveTopology(T8_TOPOLOGY::TRIANLE_LIST);
      D3D11DeviceContext->DrawIndexed(sub_info->NumVertex, 0, 0);
			last = s;
			}
		}
}

void D3DXMesh::Destroy() {
  //release resources
}
