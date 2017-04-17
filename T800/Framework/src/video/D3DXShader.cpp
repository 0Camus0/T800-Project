#include <video/D3DXShader.h>

extern ComPtr<ID3D11Device>            D3D11Device;
extern ComPtr<ID3D11DeviceContext>     D3D11DeviceContext;

bool D3DXShader::CreateShaderAPI(std::string src_vs, std::string src_fs, unsigned int sig) {
	HRESULT hr = S_OK;
	{
		VS_blob = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;
		hr = D3DCompile(src_vs.c_str(), src_vs.size(), 0, 0, 0, "VS", "vs_5_0", 0, 0, &VS_blob, &errorBlob);
		if (hr != S_OK) {

			if (errorBlob) {
				printf("ErrorBlob shader VS[%s] [%s]\n\n", src_vs.c_str(),(char*)errorBlob->GetBufferPointer());
				return false;
			}

			if (!VS_blob) {
				return false;
			}
		}

		hr = D3D11Device->CreateVertexShader(VS_blob->GetBufferPointer(), VS_blob->GetBufferSize(), 0, &pVS);
		if (hr != S_OK) {
			printf("Error Creating Vertex Shader\n");
			exit(666);
		}
	}

	{
		FS_blob = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;
		hr = D3DCompile(src_fs.c_str(), src_fs.size(), 0, 0, 0, "FS", "ps_5_0", 0, 0, &FS_blob, &errorBlob);
		if (hr != S_OK) {
			if (errorBlob) {
				printf("ErrorBlob shader FS [%s] [%s]\n\n", src_fs.c_str(), (char*)errorBlob->GetBufferPointer());
				return false;
			}

			if (!FS_blob) {
				return false;
			}
		}

		hr = D3D11Device->CreatePixelShader(FS_blob->GetBufferPointer(), FS_blob->GetBufferSize(), 0, &pFS);
		if (hr != S_OK) {
			printf("Error Creating Pixel Shader\n");
			return false;
		}
	}

	int offset = 0;
	D3D11_INPUT_ELEMENT_DESC elementDesc;
	elementDesc.SemanticName = "POSITION";
	elementDesc.SemanticIndex = 0;
	elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elementDesc.InputSlot = 0;
	elementDesc.AlignedByteOffset = offset;
	elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elementDesc.InstanceDataStepRate = 0;
	offset += 16;
	VertexDecl.push_back(elementDesc);

	if (sig&Signature::HAS_NORMALS) {
		elementDesc.SemanticName = "NORMAL";
		elementDesc.SemanticIndex = 0;
		elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = offset;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;
		offset += 16;
		VertexDecl.push_back(elementDesc);
	}

	if (sig&Signature::HAS_TANGENTS) {
		elementDesc.SemanticName = "TANGENT";
		elementDesc.SemanticIndex = 0;
		elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = offset;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;
		offset += 16;
		VertexDecl.push_back(elementDesc);
	}

	if (sig&Signature::HAS_BINORMALS) {
		elementDesc.SemanticName = "BINORMAL";
		elementDesc.SemanticIndex = 0;
		elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = offset;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;
		offset += 16;
		VertexDecl.push_back(elementDesc);
	}

	if (sig&Signature::HAS_TEXCOORDS0) {
		elementDesc.SemanticName = "TEXCOORD";
		elementDesc.SemanticIndex = 0;
		elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = offset;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;
		offset += 8;
		VertexDecl.push_back(elementDesc);
	}

	hr = D3D11Device->CreateInputLayout(&VertexDecl[0], VertexDecl.size(), VS_blob->GetBufferPointer(), VS_blob->GetBufferSize(), &Layout);
	if (hr != S_OK) {
		printf("Error Creating Input Layout\n");
		return false;
	}

	return true;
}