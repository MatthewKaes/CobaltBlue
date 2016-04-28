#include "CobaltParallax.h"

#include "CobaltEngine.h"

extern CobaltEngine* EngineHandle;

Parallax::Parallax()
{
  m_bitmap = 0;
  m_indexBuffer = 0;
  m_vertexBuffer = 0;
  X = 0;
  Y = 0;
  Z = 0.0f;
  Ox = 0;
  Oy = 0;
}

Parallax::~Parallax()
{
  Release();
}

void Parallax::Create(LPWSTR textureFile, unsigned width, unsigned height)
{
  EngineHandle->Graphics->m_bitmapListings.push_back(this);
  Width = width;
  Height = height;

  // Create a bitmap to use for 2D graphics
  m_bitmap = new ::Bitmap();
  m_bitmap->Create(textureFile);

  CreateBuffers(EngineHandle->Graphics->m_DirectX.GetDevice());
}

void Parallax::Create(unsigned width, unsigned height, unsigned imgWidth, unsigned imgHeight)
{
  EngineHandle->Graphics->m_bitmapListings.push_back(this);
  Width = width;
  Height = height;

  // Create a bitmap to use for 2D graphics
  m_bitmap = new ::Bitmap();
  m_bitmap->Create(
    imgWidth,
    imgHeight);

  CreateBuffers(EngineHandle->Graphics->m_DirectX.GetDevice());
}

void Parallax::Update(ID3D11DeviceContext* context)
{
  m_bitmap->Update(context);

  if (Z > (SCREEN_DEPTH) / 2.0f - 1.0f)
  {
    Z = (SCREEN_DEPTH) / 2.0f - 1.0f;
  }
  else if (Z < -(SCREEN_DEPTH) / 2.0f - 1.0f)
  {
    Z = -(SCREEN_DEPTH) / 2.0f - 1.0f;
  }

  if (m_prevOx == Ox && m_prevOy == Oy)
  {
    return;
  }

  // Calculate the screen coordinates of the left side of the bitmap.
  VertexType vertices[VertexCount];
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  float left = (EngineHandle->Graphics->Width() / 2.0f) * -1;

  // Calculate the screen coordinates of the right side of the bitmap.
  float right = left + (float)Width;

  // Calculate the screen coordinates of the top of the bitmap.
  float top = EngineHandle->Graphics->Height() / 2.0f;

  // Calculate the screen coordinates of the bottom of the bitmap.
  float bottom = top - (float)Height;

  // Load the vertex array with data.
  vertices[0].position = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
  vertices[0].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
  vertices[0].texture = XMFLOAT2(Ox / (float)ImgWidth(), (float)Height / ImgHeight() + Oy / (float)ImgHeight());

  vertices[1].position = XMFLOAT3(left, top, 0.0f);  // Top left.
  vertices[1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
  vertices[1].texture = XMFLOAT2(Ox / (float)ImgWidth(), Oy / (float)ImgHeight());

  vertices[2].position = XMFLOAT3(right, top, 0.0f);  // Top right.
  vertices[2].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
  vertices[2].texture = XMFLOAT2((float)Width / ImgWidth() + Ox / (float)ImgWidth(), Oy / (float)ImgHeight());

  vertices[3].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
  vertices[3].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
  vertices[3].texture = XMFLOAT2((float)Width / ImgWidth() + Ox / (float)ImgWidth(), (float)Height / ImgHeight() + Oy / (float)ImgHeight());

  context->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

  // Get a pointer to the data in the vertex buffer.
  VertexType* verticesPtr = (VertexType*)mappedResource.pData;

  // Copy the data into the vertex buffer.
  memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * VertexCount));

  // Unlock the vertex buffer.
  context->Unmap(m_vertexBuffer, 0);
}

unsigned Parallax::ImgWidth()
{
  return m_bitmap->Width();
}

unsigned Parallax::ImgHeight()
{
  return m_bitmap->Height();
}

void Parallax::Release()
{
  // Remove form the bitmap listing.
  EngineHandle->Graphics->m_bitmapListings.erase(std::find(EngineHandle->Graphics->m_bitmapListings.begin(), EngineHandle->Graphics->m_bitmapListings.end(), this));

  // Release the texture object.
  if (m_bitmap)
  {
    m_bitmap->Release();
    delete m_bitmap;
    m_bitmap = 0;
  }
}

void Parallax::CreateBuffers(ID3D11Device* device)
{
  VertexType vertices[VertexCount];
  unsigned long indices[IndexCount];
  D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
  D3D11_SUBRESOURCE_DATA vertexData, indexData;
  HRESULT result;

  // Calculate the screen coordinates of the left side of the bitmap.
  float left = (EngineHandle->Graphics->Width() / 2.0f) * -1;

  // Calculate the screen coordinates of the right side of the bitmap.
  float right = left + (float)Width;

  // Calculate the screen coordinates of the top of the bitmap.
  float top = EngineHandle->Graphics->Height() / 2.0f;

  // Calculate the screen coordinates of the bottom of the bitmap.
  float bottom = top - (float)Height;

  // Load the vertex array with data.
  vertices[0].position = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
  vertices[0].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
  vertices[0].texture = XMFLOAT2(Ox / (float)ImgWidth(), (float)Height / ImgHeight() + Oy / (float)ImgHeight());

  vertices[1].position = XMFLOAT3(left, top, 0.0f);  // Top left.
  vertices[1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
  vertices[1].texture = XMFLOAT2(Ox / (float)ImgWidth(), Oy / (float)ImgHeight());

  vertices[2].position = XMFLOAT3(right, top, 0.0f);  // Top right.
  vertices[2].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
  vertices[2].texture = XMFLOAT2((float)Width / ImgWidth() + Ox / (float)ImgWidth(), Oy / (float)ImgHeight());

  vertices[3].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
  vertices[3].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
  vertices[3].texture = XMFLOAT2((float)Width / ImgWidth() + Ox / (float)ImgWidth(), (float)Height / ImgHeight() + Oy / (float)ImgHeight());

  // Load the index array with data.
  indices[0] = 1;  // Bottom left.
  indices[1] = 2;  // Top Left.
  indices[2] = 0;  // Bottom right.
  indices[3] = 3;  // Top right.

                   // Set up the description of the static vertex buffer.
  vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  vertexBufferDesc.ByteWidth = sizeof(VertexType) * VertexCount;
  vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  vertexBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
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

void Parallax::Render(ID3D11DeviceContext* context)
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

int Parallax::GetIndexCount()
{
  return IndexCount;
}

ID3D11ShaderResourceView* Parallax::GetTexture()
{
  return m_bitmap->GetTexture();
}

Bitmap* Parallax::Bitmap()
{
  return m_bitmap;
}