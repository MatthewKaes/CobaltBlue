#include "CobaltSprite.h"

#include "CobaltEngine.h"

extern CobaltEngine* EngineHandle;
extern std::vector<Model2D*> g_renderListings;
extern std::unordered_set<Model2D*> g_updateListings;

Sprite::Sprite()
{
  m_bitmap = 0;
  m_indexBuffer = 0;
  m_vertexBuffer = 0;
  X = 0;
  Y = 0;
  Z = 0.0f;
}

Sprite::~Sprite()
{
  Release();
}

void Sprite::Create(LPWSTR textureFile)
{
  g_renderListings.push_back(this);
  g_updateListings.insert(this);

  // Create a bitmap to use for 2D graphics
  m_bitmap = new ::Bitmap();
  m_bitmap->Create(textureFile);

  CreateBuffers(EngineHandle->Graphics->DirectX.GetDevice());
}

void Sprite::Create(unsigned width, unsigned height)
{
  g_renderListings.push_back(this);
  g_updateListings.insert(this);

  // Create a bitmap to use for 2D graphics
  m_bitmap = new ::Bitmap();
  m_bitmap->Create(
    width,
    height);

  CreateBuffers(EngineHandle->Graphics->DirectX.GetDevice());
}

void Sprite::Update()
{
  m_bitmap->Update();

  if (Z > (SCREEN_DEPTH) / 2.0f - 1.0f)
  {
    Z = (SCREEN_DEPTH) / 2.0f - 1.0f;
  }
  else if (Z < -(SCREEN_DEPTH) / 2.0f - 1.0f)
  {
    Z = -(SCREEN_DEPTH) / 2.0f - 1.0f;
  }
}

unsigned Sprite::Width()
{
  return m_bitmap->Width();
}

unsigned Sprite::Height()
{
  return m_bitmap->Height();
}

void Sprite::Release()
{
  // Remove form the bitmap listing.
  g_renderListings.erase(std::find(g_renderListings.begin(), g_renderListings.end(), this));
  g_updateListings.erase(this);

  // Release the texture object.
  if (m_bitmap)
  {
    m_bitmap->Release();
    delete m_bitmap;
    m_bitmap = 0;
  }
}

void Sprite::CreateBuffers(ID3D11Device* device)
{
  VertexType vertices[VertexCount];
  unsigned long indices[IndexCount];
  D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
  D3D11_SUBRESOURCE_DATA vertexData, indexData;
  HRESULT result;

  // Calculate the screen coordinates of the left side of the bitmap.
  float left = (EngineHandle->Graphics->Width() / 2.0f) * -1;

  // Calculate the screen coordinates of the right side of the bitmap.
  float right = left + (float)Width();

  // Calculate the screen coordinates of the top of the bitmap.
  float top = EngineHandle->Graphics->Height() / 2.0f;

  // Calculate the screen coordinates of the bottom of the bitmap.
  float bottom = top - (float)Height();

  // Load the vertex array with data.
  vertices[0].position = D3DXVECTOR3(left, bottom, 0.0f);  // Bottom left.
  vertices[0].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
  vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);

  vertices[1].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
  vertices[1].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
  vertices[1].texture = D3DXVECTOR2(0.0f, 0.0f);

  vertices[2].position = D3DXVECTOR3(right, top, 0.0f);  // Top right.
  vertices[2].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
  vertices[2].texture = D3DXVECTOR2(1.0f, 0.0f);

  vertices[3].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
  vertices[3].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
  vertices[3].texture = D3DXVECTOR2(1.0f, 1.0f);

  // Load the index array with data.
  indices[0] = 1;  // Bottom left.
  indices[1] = 2;  // Top Left.
  indices[2] = 0;  // Bottom right.
  indices[3] = 3;  // Top right.

  // Set up the description of the static vertex buffer.
  vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  vertexBufferDesc.ByteWidth = sizeof(VertexType) * VertexCount;
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
    //return false;
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
    //return false;
  }
}

void Sprite::Render()
{
  ID3D11DeviceContext* context = EngineHandle->Graphics->DirectX.GetDeviceContext();
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

int Sprite::GetIndexCount()
{
  return IndexCount;
}

ID3D11ShaderResourceView* Sprite::GetTexture()
{
  return m_bitmap->GetTexture();
}

Bitmap* Sprite::Bitmap()
{
  return m_bitmap;
}