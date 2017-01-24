#include "TextureRender.h"

TextureRender::TextureRender()
{
  m_renderTargetTexture = 0;
  m_renderTargetView = 0;
  m_shaderResourceView = 0;
}

TextureRender::~TextureRender()
{
  Release();
}

bool TextureRender::Initialize(ID3D11Device* device, int screenWidth, int screenHeight)
{
  D3D11_TEXTURE2D_DESC textureDesc;
  HRESULT result;
  D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
  D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

  // Initialize the render target texture description.
  ZeroMemory(&textureDesc, sizeof(textureDesc));

  // Setup the render target texture description.
  textureDesc.Width = screenWidth;
  textureDesc.Height = screenHeight;
  textureDesc.MipLevels = 1;
  textureDesc.ArraySize = 1;
  textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  textureDesc.SampleDesc.Count = 1;
  textureDesc.Usage = D3D11_USAGE_DEFAULT;
  textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
  textureDesc.CPUAccessFlags = 0;
  textureDesc.MiscFlags = 0;

  // Create the render target texture.
  result = device->CreateTexture2D(&textureDesc, NULL, &m_renderTargetTexture);
  if (FAILED(result))
  {
    return false;
  }

  // Setup the description of the render target view.
  renderTargetViewDesc.Format = textureDesc.Format;
  renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
  renderTargetViewDesc.Texture2D.MipSlice = 0;

  // Create the render target view.
  result = device->CreateRenderTargetView(m_renderTargetTexture, &renderTargetViewDesc, &m_renderTargetView);
  if (FAILED(result))
  {
    return false;
  }

  // Setup the description of the shader resource view.
  shaderResourceViewDesc.Format = textureDesc.Format;
  shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
  shaderResourceViewDesc.Texture2D.MipLevels = 1;

  // Create the shader resource view.
  result = device->CreateShaderResourceView(m_renderTargetTexture, &shaderResourceViewDesc, &m_shaderResourceView);
  if (FAILED(result))
  {
    return false;
  }

  VertexType vertices[VertexCount];
  unsigned long indices[IndexCount];
  D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
  D3D11_SUBRESOURCE_DATA vertexData, indexData;

  // Calculate the screen coordinates of the left side of the bitmap.
  float left = (screenWidth / 2.0f) * -1;

  // Calculate the screen coordinates of the right side of the bitmap.
  float right = left + (float)screenWidth;

  // Calculate the screen coordinates of the top of the bitmap.
  float top = screenHeight / 2.0f;

  // Calculate the screen coordinates of the bottom of the bitmap.
  float bottom = top - (float)screenHeight;

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
  device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

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
  device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);

  return true;
}

void TextureRender::Release()
{
  if (m_shaderResourceView)
  {
    m_shaderResourceView->Release();
    m_shaderResourceView = 0;
  }

  if (m_renderTargetView)
  {
    m_renderTargetView->Release();
    m_renderTargetView = 0;
  }

  if (m_renderTargetTexture)
  {
    m_renderTargetTexture->Release();
    m_renderTargetTexture = 0;
  }
}

void TextureRender::SetRenderTarget(Direct3D* d3d)
{
  // Bind the render target view and depth stencil buffer to the output render pipeline.
  d3d->GetDeviceContext()->OMSetRenderTargets(1, &m_renderTargetView, d3d->GetDepthStencilView());
}

void TextureRender::ClearRenderTarget(Direct3D* d3d, float red, float green, float blue, float alpha)
{
  float color[4];

  // Setup the color to clear the buffer to.
  color[0] = red;
  color[1] = green;
  color[2] = blue;
  color[3] = alpha;

  // Clear the back buffer.
  d3d->GetDeviceContext()->ClearRenderTargetView(m_renderTargetView, color);

  // Clear the depth buffer.
  d3d->GetDeviceContext()->ClearDepthStencilView(d3d->GetDepthStencilView(), D3D10_CLEAR_DEPTH, 1.0f, 0);

  return;
}

void TextureRender::Render(Direct3D* d3d)
{
  unsigned int stride;
  unsigned int offset;

  // Set vertex buffer stride and offset.
  stride = sizeof(VertexType);
  offset = 0;

  // Set the vertex buffer to active in the input assembler so it can be rendered.
  d3d->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

  // Set the index buffer to active in the input assembler so it can be rendered.
  d3d->GetDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

  // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
  d3d->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

ID3D11ShaderResourceView* TextureRender::GetTexture()
{
  return m_shaderResourceView;
}

int TextureRender::GetIndexCount()
{
  return IndexCount;
}