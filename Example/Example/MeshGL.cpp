#include "MeshGL.h"
#include <iostream>

#define CHANGE_TO_RH 0
#define DEBUG_MODEL 0

void MeshGL::Create(char *filename) {

	std::string fname = std::string(filename);
	if (xFile.LoadXFile(fname)) {
		cout << " Load " << endl;
	}
	else {
		cout << " Failed " << endl;
	}

	for (std::size_t i = 0; i < xFile.MeshInfo.size(); i++) {
		xFinalGeometry *it = &xFile.MeshInfo[i];
		xMeshGeometry *pActual = &xFile.XMeshDataBase[0]->Geometry[i];
	
		char *vsSourceP = file2string("VS_Mesh.glsl");
		char *fsSourceP = file2string("FS_Mesh.glsl");

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

		it->ShaderProg = glCreateProgram();

		GLuint vshader_id = createShader(GL_VERTEX_SHADER, (char*)vstr.c_str());
		GLuint fshader_id = createShader(GL_FRAGMENT_SHADER, (char*)fstr.c_str());

		delete[] vsSourceP;
		delete[] fsSourceP;

		glAttachShader(it->ShaderProg, vshader_id);
		glAttachShader(it->ShaderProg, fshader_id);

		glLinkProgram(it->ShaderProg);
		glUseProgram(it->ShaderProg);

		it->vertexAttribLoc = glGetAttribLocation(it->ShaderProg, "Vertex");
		it->normalAttribLoc = glGetAttribLocation(it->ShaderProg, "Normal");
		it->uvAttribLoc = glGetAttribLocation(it->ShaderProg, "UV");
		it->uvSecAttribLoc = glGetAttribLocation(it->ShaderProg, "UV_Sec");
		it->tangentAttribLoc = glGetAttribLocation(it->ShaderProg, "Tangent");			
		it->binormalAttribLoc = glGetAttribLocation(it->ShaderProg, "Binormal");

		it->matWorldViewProjUniformLoc = glGetUniformLocation(it->ShaderProg, "WVP");
		it->matWorldUniformLoc = glGetUniformLocation(it->ShaderProg, "World");

		int NumMaterials = pActual->MaterialList.Materials.size();
		int NumFaceIndices = pActual->MaterialList.FaceIndices.size();
		
		for (int j = 0; j < NumMaterials; j++) {
			xSubsetInfo *subinfo = &it->Subsets[j];
			xMaterial *material = &pActual->MaterialList.Materials[j];
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
								subinfo->IdTex = ttex->id;
								subinfo->IdTexUniformLoc = glGetUniformLocation(it->ShaderProg, "diffuse");
								found=true;
								break;
							}
						}
						if(!found){
							Texture *tex = new TextureGL;
							int id = tex->LoadTexture((char*)path.c_str());
							if (id != -1) {
							#if DEBUG_MODEL
								std::cout << "Texture Loaded index " << id << std::endl;
							#endif
								Textures.push_back(tex);
								subinfo->IdTex = id;
								subinfo->IdTexUniformLoc = glGetUniformLocation(it->ShaderProg, "diffuse");
							}
							else {
								std::cout << "Texture not Found" << std::endl;
								delete tex;
							}
						}					  
					}
				}
			}
			unsigned short *tmpIndexex = new unsigned short[subinfo->NumVertex];
			int counter = 0;
			bool first = false;
			for (int k = 0; k < NumFaceIndices; k++) {
				if (pActual->MaterialList.FaceIndices[k] == j) {
					unsigned int index = k * 3;
					if (!first) {
						subinfo->TriStart = k;
						subinfo->VertexStart = index;
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

			glGenBuffers(1, &subinfo->Id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subinfo->Id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, subinfo->NumTris * 3 * sizeof(unsigned short), tmpIndexex, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			delete[] tmpIndexex;
		}

		glGenBuffers(1, &it->Id);
		glBindBuffer(GL_ARRAY_BUFFER, it->Id);
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
		glGenBuffers(1, &it->IdIBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, it->IdIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, pActual->Triangles.size() * sizeof(unsigned short), &pActual->Triangles[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	XMatIdentity(transform);
}

void MeshGL::Transform(float *t) {
	transform = t;
}

void MeshGL::Draw(float *t, float *vp) {

	if (t)
		transform = t;

	for (std::size_t i = 0; i <  xFile.MeshInfo.size();  i++) {
		XMATRIX44 VP = XMATRIX44(vp);
		XMATRIX44 WVP = transform*VP;

		xFinalGeometry *it = &xFile.MeshInfo[i];
		xMeshGeometry *pActual = &xFile.XMeshDataBase[0]->Geometry[i];

		glUseProgram(it->ShaderProg);

		glUniformMatrix4fv(it->matWorldUniformLoc, 1, GL_FALSE, &transform.m[0][0]);
		glUniformMatrix4fv(it->matWorldViewProjUniformLoc, 1, GL_FALSE, &WVP.m[0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, it->Id);

		glEnableVertexAttribArray(it->vertexAttribLoc);
		glVertexAttribPointer(it->vertexAttribLoc, 4, GL_FLOAT, GL_FALSE, it->VertexSize, BUFFER_OFFSET(0));
	
		if (it->normalAttribLoc != -1){
			glEnableVertexAttribArray(it->normalAttribLoc);
			glVertexAttribPointer(it->normalAttribLoc, 4, GL_FLOAT, GL_FALSE, it->VertexSize, BUFFER_OFFSET(16));
		}

		if (it->tangentAttribLoc != -1){
			glEnableVertexAttribArray(it->tangentAttribLoc);
			glVertexAttribPointer(it->tangentAttribLoc, 4, GL_FLOAT, GL_FALSE, it->VertexSize, BUFFER_OFFSET(32));
		}

		if (it->binormalAttribLoc != -1){
			glEnableVertexAttribArray(it->binormalAttribLoc);
			glVertexAttribPointer(it->binormalAttribLoc, 4, GL_FLOAT, GL_FALSE, it->VertexSize, BUFFER_OFFSET(48));
		}

		if (it->uvAttribLoc != -1){
			int offset = 32;
			if(pActual->VertexAttributes&xMeshGeometry::HAS_TANGENT){
				offset = 64;
			}
			glEnableVertexAttribArray(it->uvAttribLoc);
			glVertexAttribPointer(it->uvAttribLoc, 2, GL_FLOAT, GL_FALSE, it->VertexSize, BUFFER_OFFSET(offset));
		}

		if (it->uvSecAttribLoc != -1){
			int offset = 40;
			if (pActual->VertexAttributes&xMeshGeometry::HAS_TANGENT) {
				offset = 72;
			}
			glEnableVertexAttribArray(it->uvSecAttribLoc);
			glVertexAttribPointer(it->uvSecAttribLoc, 2, GL_FLOAT, GL_FALSE, it->VertexSize, BUFFER_OFFSET(offset));
		}
	
		for(std::size_t k = 0; k < it->Subsets.size(); k++){
			xSubsetInfo *sbIfo = &it->Subsets[k];
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sbIfo->Id);

			if(sbIfo->IdTex!=-1){
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, sbIfo->IdTex);
				glUniform1i(sbIfo->IdTexUniformLoc, 0);
			}

			glDrawElements(GL_TRIANGLES, sbIfo->NumVertex, GL_UNSIGNED_SHORT, 0);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(it->vertexAttribLoc);
		if (it->normalAttribLoc != -1) {
			glDisableVertexAttribArray(it->normalAttribLoc);
		}
		if (it->tangentAttribLoc != -1) {
			glDisableVertexAttribArray(it->tangentAttribLoc);
		}
		if (it->binormalAttribLoc != -1) {
			glDisableVertexAttribArray(it->binormalAttribLoc);
		}
		if (it->uvAttribLoc != -1) {
			glDisableVertexAttribArray(it->uvAttribLoc);
		}
		if (it->uvSecAttribLoc != -1) {
			glDisableVertexAttribArray(it->uvSecAttribLoc);
		}
		
		glUseProgram(0);
	}

	
}

void MeshGL::Destroy() {
	for (std::size_t i = 0; i < xFile.MeshInfo.size(); i++) {
		xFinalGeometry *it = &xFile.MeshInfo[i];
		glDeleteProgram(it->ShaderProg);
	}
}