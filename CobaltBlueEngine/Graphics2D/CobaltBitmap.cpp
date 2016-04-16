#include "CobaltBitmap.h"
#include "CobaltEngine.h"

extern CobaltEngine* EngineHandle;

CobaltBitmap::CobaltBitmap()
{
  m_texture = 0;
  m_indexBuffer = 0;
  m_vertexBuffer = 0;
}

CobaltBitmap::~CobaltBitmap()
{
  Release();
}

bool CobaltBitmap::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, LPCWSTR filename)
{
  m_texture = new CobaltTexture();
  m_texture->Initialize(device, context, filename, TextureType::Dynamic);
  return InitializeBuffers(device);
}

void CobaltBitmap::Release()
{
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

  // Release the vertex buffer.
  if (m_texture)
  {
    m_texture->Shutdown();
    m_texture = 0;
  }
}

void CobaltBitmap::Render(ID3D11DeviceContext* context)
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

void CobaltBitmap::Update(ID3D11DeviceContext* context)
{

}

ID3D11ShaderResourceView* CobaltBitmap::GetTexture()
{
  return m_texture->GetTexture();
}

unsigned CobaltBitmap::Width()
{
  return m_texture->Width();
}

unsigned CobaltBitmap::Height()
{
  return m_texture->Height();
}

bool CobaltBitmap::InitializeBuffers(ID3D11Device* device)
{
  VertexType vertices[VertexCount];
  unsigned long indices[IndexCount];
  D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
  D3D11_SUBRESOURCE_DATA vertexData, indexData;
  HRESULT result;

  // Calculate the screen coordinates of the left side of the bitmap.
  float left = (float)((1366 / 2) * -1) + (float)100;

  // Calculate the screen coordinates of the right side of the bitmap.
  float right = left + (float)Width();

  // Calculate the screen coordinates of the top of the bitmap.
  float top = (float)(768 / 2) - (float)100;

  // Calculate the screen coordinates of the bottom of the bitmap.
  float bottom = top - (float)Height();

  // Load the vertex array with data.
  vertices[0].position = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
  vertices[0].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
  vertices[0].texture = XMFLOAT2(0.0f, 1.0f);

  vertices[1].position = XMFLOAT3(left, top, 0.0f);  // Top left.
  vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
  vertices[1].texture = XMFLOAT2(0.0f, 0.0f);

  vertices[2].position = XMFLOAT3(right, top, 0.0f);  // Top right.
  vertices[2].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
  vertices[2].texture = XMFLOAT2(1.0f, 0.0f);

  vertices[3].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
  vertices[3].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
  vertices[3].texture = XMFLOAT2(1.0f, 1.0f);

  // Load the index array with data.
  indices[0] = 1;  // Bottom left.
  indices[1] = 2;  // Top Left.
  indices[2] = 0;  // Bottom right.
  indices[3] = 3;  // Top right.

  // Set up the description of the static vertex buffer.
  vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  vertexBufferDesc.ByteWidth = sizeof(VertexType) * VertexCount;
  vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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
  indexBufferDesc.ByteWidth = sizeof(unsigned long) * IndexCount;
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

  return true;
}
