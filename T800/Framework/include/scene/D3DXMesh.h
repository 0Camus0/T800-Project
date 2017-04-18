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

#ifndef T800_MESH_D3D_H
#define T800_MESH_D3D_H

#include <Config.h>

#include <video\D3DXTexture.h>
#include <D3Dcompiler.h>

#include <video\BaseDriver.h>

#include <utils\Utils.h>

#include <utils\xMaths.h>
#include <utils\XDataBase.h>
#include <scene\PrimitiveBase.h>



#include <vector>

using namespace xF;

class D3DXMesh : public PrimitiveBase {
public:
	D3DXMesh()  {}

	struct CBuffer {
		XMATRIX44 WVP;
		XMATRIX44 World;
		XMATRIX44 WorldView;
		XVECTOR3  Light0Pos;
		XVECTOR3  Light0Col;
		XVECTOR3  CameraPos;
		XVECTOR3  CameraInfo;
		XVECTOR3  Ambient;
	};


	struct SubSetInfo {	
		unsigned int		Sig;

		ComPtr<ID3D11Buffer>		IB;
		D3DXTexture*					DiffuseTex;
		D3DXTexture*					SpecularTex;
		D3DXTexture*					GlossfTex;
		D3DXTexture*					NormalTex;
		D3DXTexture*					ReflectTex;

		int					DiffuseId;
		int					SpecularId;
		int					GlossfId;
		int					NormalId;
		int					ReflectId;

		unsigned int		VertexStart;
		unsigned int		NumVertex;
		unsigned int		TriStart;
		unsigned int		NumTris;
		unsigned int		VertexSize;
		bool				bAlignedVertex;
	};


	struct MeshInfo {
		unsigned int			 VertexSize;
		unsigned int			 NumVertex;

		ComPtr<ID3D11Buffer>		IB;
		ComPtr<ID3D11Buffer>		VB;
		ComPtr<ID3D11Buffer>        pd3dConstantBuffer;
		D3DXMesh::CBuffer			CnstBuffer;

		std::vector<SubSetInfo>	SubSets;
	};

	void Create() {}
	void Create(char *);
	void Transform(float *t);
	void Draw(float *t, float *vp);
	void Destroy();

	void GatherInfo();
	int  LoadTex(std::string p, xF::xMaterial *mat,D3DXTexture** tex);

	XMATRIX44	transform;
	XDataBase	xFile;
	std::vector<MeshInfo> Info;
};


#endif