#include <video/windows/D3DXShader.h>


namespace t800 {
  extern Device*            T8Device;
  extern DeviceContext*     T8DeviceContext;

  bool D3DXShader::CreateShaderAPI(std::string src_vs, std::string src_fs, unsigned int sig) {
    ID3D11Device* device = reinterpret_cast<ID3D11Device*>(T8Device->GetAPIObject());
    ID3D11DeviceContext* deviceContext = reinterpret_cast<ID3D11DeviceContext*>(T8DeviceContext->GetAPIObject());
    HRESULT hr = S_OK;
    {
      VS_blob = nullptr;
      ComPtr<ID3DBlob> errorBlob = nullptr;
      hr = D3DCompile(src_vs.c_str(), src_vs.size(), 0, 0, 0, "VS", "vs_5_0", 0, 0, &VS_blob, &errorBlob);
      if (hr != S_OK) {

        if (errorBlob) {
          printf("ErrorBlob shader VS[%s] [%s]\n\n", src_vs.c_str(), (char*)errorBlob->GetBufferPointer());
          return false;
        }

        if (!VS_blob) {
          return false;
        }
      }

      hr = device->CreateVertexShader(VS_blob->GetBufferPointer(), VS_blob->GetBufferSize(), 0, &pVS);
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

      hr = device->CreatePixelShader(FS_blob->GetBufferPointer(), FS_blob->GetBufferSize(), 0, &pFS);
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

    hr = device->CreateInputLayout(&VertexDecl[0], VertexDecl.size(), VS_blob->GetBufferPointer(), VS_blob->GetBufferSize(), &Layout);
    if (hr != S_OK) {
      printf("Error Creating Input Layout\n");
      return false;
    }

    return true;
  }

  void D3DXShader::Set(const DeviceContext & deviceContext)
  {
    reinterpret_cast<ID3D11DeviceContext*>(deviceContext.GetAPIObject())->VSSetShader(pVS.Get(), 0, 0);
    reinterpret_cast<ID3D11DeviceContext*>(deviceContext.GetAPIObject())->PSSetShader(pFS.Get(), 0, 0);
    reinterpret_cast<ID3D11DeviceContext*>(deviceContext.GetAPIObject())->IASetInputLayout(Layout.Get());
  }
}