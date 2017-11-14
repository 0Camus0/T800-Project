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

/*#include <video/BaseDriver.h>
#include <video/GLShader.h>
#include <video/GLRT.h>
#include <scene/GLMesh.h>
#include <utils/Utils.h>




#define CHANGE_TO_RH 0
#define DEBUG_MODEL 0


void GLMesh::Create(char *filename) {

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

		unsigned int NumMaterials = pActual->MaterialList.Materials.size();
		unsigned int NumFaceIndices = pActual->MaterialList.FaceIndices.size();

		for (unsigned int j = 0; j < NumMaterials; j++) {
			xSubsetInfo *subinfo = &it->Subsets[j];
			xMaterial *material = &pActual->MaterialList.Materials[j];
			SubSetInfo *it_subsetinfo = &it_MeshInfo->SubSets[j];

			for (unsigned int k = 0; k < material->EffectInstance.pDefaults.size(); k++) {
				xEffectDefault *mDef = &material->EffectInstance.pDefaults[k];
				if (mDef->Type == xF::xEFFECTENUM::STDX_STRINGS) {
#if DEBUG_MODEL
					std::cout << "[" << mDef->NameParam << "]" << std::endl;
#endif
					if (mDef->NameParam == "diffuseMap") {
						std::string path = RemovePath(mDef->CaseString);
#if DEBUG_MODEL
						std::cout << "path[" << path << "]" << std::endl;
#endif
						it_subsetinfo->DiffuseId = LoadTex(path, material);
						it_subsetinfo->IdDiffuseTex = g_pBaseDriver->GetTexture(it_subsetinfo->DiffuseId)->id;
					}

					if (mDef->NameParam == "specularMap") {
						std::string path = RemovePath(mDef->CaseString);
#if DEBUG_MODEL
						std::cout << "path[" << path << "]" << std::endl;
#endif
						it_subsetinfo->SpecularId = LoadTex(path, material);
						it_subsetinfo->IdSpecularTex = g_pBaseDriver->GetTexture(it_subsetinfo->SpecularId)->id;
					}

					if (mDef->NameParam == "glossMap") {
						std::string path = RemovePath(mDef->CaseString);
#if DEBUG_MODEL
						std::cout << "path[" << path << "]" << std::endl;
#endif
						it_subsetinfo->GlossfId = LoadTex(path, material);
						it_subsetinfo->IdGlossTex = g_pBaseDriver->GetTexture(it_subsetinfo->GlossfId)->id;
					}

					if (mDef->NameParam == "normalMap") {
						std::string path = RemovePath(mDef->CaseString);
#if DEBUG_MODEL
						std::cout << "path[" << path << "]" << std::endl;
#endif
						it_subsetinfo->NormalId = LoadTex(path, material);
						it_subsetinfo->IdNormalTex = g_pBaseDriver->GetTexture(it_subsetinfo->NormalId)->id;
					}
				}
			}

			it_subsetinfo->NumTris = subinfo->NumTris;
			it_subsetinfo->NumVertex = subinfo->NumVertex;
			unsigned short *tmpIndexex = new unsigned short[it_subsetinfo->NumVertex];
			int counter = 0;
			bool first = false;
			for (unsigned int k = 0; k < NumFaceIndices; k++) {
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

			glGenBuffers(1, &it_subsetinfo->Id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, it_subsetinfo->Id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, it_subsetinfo->NumTris * 3 * sizeof(unsigned short), tmpIndexex, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			delete[] tmpIndexex;
		}



		it_MeshInfo->VertexSize = it->VertexSize;

		glGenBuffers(1, &it_MeshInfo->Id);
		glBindBuffer(GL_ARRAY_BUFFER, it_MeshInfo->Id);
		glBufferData(GL_ARRAY_BUFFER, pActual->NumVertices*it->VertexSize, &it->pData[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


#if CHANGE_TO_RH
		for (std::size_t a = 0; a < pActual->Triangles.size(); a += 3) {
			unsigned short i0 = pActual->Triangles[a + 0];
			unsigned short i2 = pActual->Triangles[a + 2];
			pActual->Triangles[a + 0] = i2;
			pActual->Triangles[a + 2] = i0;
		}
#endif

		glGenBuffers(1, &it_MeshInfo->IdIBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, it_MeshInfo->IdIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, pActual->Triangles.size() * sizeof(unsigned short), &pActual->Triangles[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}

	XMatIdentity(transform);
}

void GLMesh::GatherInfo() {

	char *vsSourceP = file2string("Shaders/VS_Mesh.glsl");
	char *fsSourceP = file2string("Shaders/FS_Mesh.glsl");

	std::string vstr = std::string(vsSourceP);
	std::string fstr = std::string(fsSourceP);

	free(vsSourceP);
	free(fsSourceP);

	for (std::size_t i = 0; i < xFile.MeshInfo.size(); i++) {
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

int	 GLMesh::LoadTex(std::string p, xF::xMaterial *mat) {
	int id = g_pBaseDriver->CreateTexture(p);
	Texture* tex = g_pBaseDriver->GetTexture(id);
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
	tex->SetTextureParams();

	if (id != -1) {
#if DEBUG_MODEL
		std::cout << "Texture Loaded index " << id << std::endl;
#endif
	}
	else {
		std::cout << "Texture [" << p << "] not Found" << std::endl;
	}

	return id;
}

void GLMesh::Transform(float *t) {
	transform = t;
}

void GLMesh::Draw(float *t, float *vp) {

	if (t)
		transform = t;

	Camera *pActualCamera = pScProp->pCameras[0];

	for (std::size_t i = 0; i < xFile.MeshInfo.size(); i++) {
		XMATRIX44 VP = pActualCamera->VP;
		XMATRIX44 WVP = transform*VP;
		XMATRIX44 WorldView = transform*pActualCamera->View;
		XVECTOR3 infoCam = XVECTOR3(pActualCamera->NPlane, pActualCamera->FPlane, pActualCamera->Fov, 1.0f);
		XVECTOR3 camPos = pActualCamera->Eye;

		MeshInfo  *it_MeshInfo = &Info[i];
		xMeshGeometry *pActual = &xFile.XMeshDataBase[0]->Geometry[i];

		GLShader *s = 0;
		GLShader *last = (GLShader*)32;
		for (std::size_t k = 0; k < it_MeshInfo->SubSets.size(); k++) {
			SubSetInfo *sub_info = &it_MeshInfo->SubSets[k];
			bool update = false;

			unsigned int sig = sub_info->Sig;
			sig |= gSig;
			s = dynamic_cast<GLShader*>(g_pBaseDriver->GetShaderSig(sig));

			if (s != last)
				update = true;

			if (update) {
				glUseProgram(s->ShaderProg);

				glUniformMatrix4fv(s->matWorldUniformLoc, 1, GL_FALSE, &transform.m[0][0]);
				glUniformMatrix4fv(s->matWorldViewProjUniformLoc, 1, GL_FALSE, &WVP.m[0][0]);
				glUniformMatrix4fv(s->matWorldViewUniformLoc, 1, GL_FALSE, &WorldView.m[0][0]);


				if (s->Light0Pos_Loc != -1) {
					glUniform4fv(s->Light0Pos_Loc, 1, &pScProp->Lights[0].Position.v[0]);
				}

				if (s->Light0Color_Loc != -1) {
					glUniform4fv(s->Light0Color_Loc, 1, &pScProp->Lights[0].Color.v[0]);
				}

				if (s->CameraPos_Loc != -1) {
					glUniform4fv(s->CameraPos_Loc, 1, &camPos.v[0]);
				}

				if (s->CameraInfo_Loc != -1) {
					glUniform4fv(s->CameraInfo_Loc, 1, &infoCam.v[0]);
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
			if (sub_info->IdDiffuseTex != -1) {
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
}

void GLMesh::Destroy() {
#ifdef USING_GL_COMMON

#endif
}*/
