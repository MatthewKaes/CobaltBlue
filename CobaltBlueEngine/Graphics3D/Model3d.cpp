#include "Model3d.h"
#include "CobaltEngine.h"

unsigned Model3D::ModelID = 0;
extern CobaltEngine* EngineHandle;

Model3D::Model3D()
{
}

Model3D::~Model3D()
{
  Release();
}

void Model3D::Create(LPWSTR textureFile, TextureType textureType)
{
  // Add to the rendering queue.
  m_id = Model3D::ModelID++;
  EngineHandle->Graphics->m_modelListings.insert(std::make_pair(m_id, this));

  // Setup the object.
  InitializeTexture(EngineHandle->Graphics->m_DirectX.GetDevice(), EngineHandle->Graphics->m_DirectX.GetDeviceContext(), textureFile, textureType);
  InitializeBuffers(EngineHandle->Graphics->m_DirectX.GetDevice());
}

void Model3D::Release()
{
  EngineHandle->Graphics->m_modelListings.erase(m_id);

  // Release the index buffer.
  if (m_indexBuffer)
  {
    m_indexBuffer->Release();
    m_indexBuffer = 0;
  }

  // Release the vertex buffer.
  if (m_vertexBuffer)
  {
    m_vertexBuffer->Release();
    m_vertexBuffer = 0;
  }	
  
  // Release the texture object.
  if (m_Texture)
  {
    m_Texture->Shutdown();
    delete m_Texture;
    m_Texture = 0;
  }

}

void Model3D::Render(ID3D11DeviceContext* context)
{
  unsigned int stride;
  unsigned int offset;

  // Set vertex buffer stride and offset.
  stride = sizeof(VertexType);
  offset = 0;

  // Set the vertex buffer to active in the input assembler so it can be rendered.
  context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

  // Set the index buffer to active in the input assembler so it can be rendered.
  context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

  // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
  context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

int Model3D::GetIndexCount()
{
  return m_indexCount;
}

ID3D11ShaderResourceView* Model3D::GetTexture()
{
  return m_Texture->GetTexture();
}

bool Model3D::InitializeTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, LPWSTR filename, TextureType textureType)
{
  // Create the texture object.
  m_Texture = new CobaltTexture;

  return  m_Texture->Initialize(device, deviceContext, filename, textureType);
}

bool Model3D::InitializeBuffers(ID3D11Device* device)
{
  VertexType* vertices;
  unsigned long* indices;
  D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
  D3D11_SUBRESOURCE_DATA vertexData, indexData;
  HRESULT result;

  // Set the number of vertices in the vertex array.
  m_vertexCount = 4;

  // Set the number of indices in the index array.
  m_indexCount = 4;

  // Create the vertex array.
  vertices = new VertexType[m_vertexCount];

  // Create the index array.
  indices = new unsigned long[m_indexCount];

  // Load the vertex array with data.
  vertices[0].position = XMFLOAT3(-2.0f, -2.0f, 0.0f);  // Bottom left.
  vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
  vertices[0].texture = XMFLOAT2(0.0f, 1.0f);

  vertices[1].position = XMFLOAT3(-2.0f, 2.0f, 0.0f);  // Top left.
  vertices[1].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
  vertices[1].texture = XMFLOAT2(0.0f, 0.0f);

  vertices[2].position = XMFLOAT3(2.0f, 2.0f, 0.0f);  // Top right.
  vertices[2].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
  vertices[2].texture = XMFLOAT2(1.0f, 0.0f);

  vertices[3].position = XMFLOAT3(2.0f, -2.0f, 0.0f);  // Bottom right.
  vertices[3].color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
  vertices[3].texture = XMFLOAT2(1.0f, 1.0f);

  // Load the index array with data.
  indices[0] = 1;  // Bottom left.
  indices[1] = 2;  // Top Left.
  indices[2] = 0;  // Bottom right.
  indices[3] = 3;  // Top right.

  // Set up the description of the static vertex buffer.
  vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
  vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDesc.CPUAccessFlags = 0;
  vertexBufferDesc.MiscFlags = 0;
  vertexBufferDesc.StructureByteStride = 0;

  // Give the subresource structure a pointer to the vertex data.
  vertexData.pSysMem = vertices;
  vertexData.SysMemPitch = 0;
  vertexData.SysMemSlicePitch = 0;

  // Now create the vertex buffer.
  result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
  if (FAILED(result))
  {
    return false;
  }

  // Set up the description of the static index buffer.
  indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
  indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  indexBufferDesc.CPUAccessFlags = 0;
  indexBufferDesc.MiscFlags = 0;
  indexBufferDesc.StructureByteStride = 0;

  // Give the subresource structure a pointer to the index data.
  indexData.pSysMem = indices;
  indexData.SysMemPitch = 0;
  indexData.SysMemSlicePitch = 0;

  // Create the index buffer.
  result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
  if (FAILED(result))
  {
    return false;
  }

  // Release the arrays now that the vertex and index buffers have been created and loaded.
  delete[] vertices;
  delete[] indices;

  return true;
}