#include "CobaltTexture.h"
#include "CobaltEngine.h"

extern CobaltEngine* EngineHandle;

CobaltTexture::CobaltTexture()
{
  m_texture = 0;
  m_textureView = 0;
}

CobaltTexture::~CobaltTexture()
{
  Shutdown();
}

bool CobaltTexture::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, LPCWSTR filename, TextureType usage)
{
  bool result;
  D3D11_TEXTURE2D_DESC textureDesc;
  HRESULT hResult;
  unsigned int rowPitch;
  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

  // Load the targa image data into memory.
  result = LoadTexture(filename);
  if (!result)
  {
    return false;
  }

  // Set if the texture is dynamic.
  m_type = usage;

  // Setup the description of the texture.
  textureDesc.Height = m_height;
  textureDesc.Width = m_width;
  textureDesc.MipLevels = m_type == TextureType::StaticMipMap ? 0 : 1;
  textureDesc.ArraySize = 1;
  textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  textureDesc.SampleDesc.Count = 1;
  textureDesc.SampleDesc.Quality = 0;
  textureDesc.Usage = m_type == TextureType::Dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
  textureDesc.BindFlags = m_type == TextureType::Dynamic ? D3D11_BIND_SHADER_RESOURCE : D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
  textureDesc.CPUAccessFlags = m_type == TextureType::Dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
  textureDesc.MiscFlags = m_type == TextureType::StaticMipMap ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

  // Create the empty texture.
  hResult = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
  if (FAILED(hResult))
  {
    return false;
  }

  // Set the row pitch of the targa image data.
  rowPitch = (m_width * 4) * sizeof(unsigned char);

  // Copy the targa image data into the texture.
  if (m_type == TextureType::Dynamic)
  {
    // Update the dynamic texture.
    Update(context);
  }
  else
  {
    context->UpdateSubresource(m_texture, 0, NULL, m_textureData, rowPitch, 0);
  }

  // Setup the shader resource view description.
  srvDesc.Format = textureDesc.Format;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MostDetailedMip = 0;
  srvDesc.Texture2D.MipLevels = -1;

  // Create the shader resource view for the texture.
  hResult = device->CreateShaderResourceView(m_texture, 0, &m_textureView);
  if (FAILED(hResult))
  {
    return false;
  }

  // Generate mipmaps for this texture.
  if (m_type == TextureType::StaticMipMap)
  {
    context->GenerateMips(m_textureView);
  }

  // Release the targa image data now that the image data has been loaded into the texture.
  delete[] m_textureData;
  m_textureData = 0;

  return true;
}

void CobaltTexture::Shutdown()
{
  // Release the texture resource.
  if (m_textureView)
  {
    m_textureView->Release();
    m_textureView = 0;
  }

  return;
}

void CobaltTexture::Update(ID3D11DeviceContext* context)
{
  if (m_type != TextureType::Dynamic)
  {
    return;
  }

  D3D11_MAPPED_SUBRESOURCE mappedResource;
  context->Map(m_texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

  BYTE* buffer = m_textureData;
  BYTE* mappedData = reinterpret_cast<BYTE*>(mappedResource.pData);
  for (UINT i = 0; i < m_height; i++)
  {
    memcpy(mappedData, buffer, m_width * 4);
    mappedData += mappedResource.RowPitch;
    buffer += m_width * 4;
  }

  context->Unmap(m_texture, 0);
}

unsigned CobaltTexture::Width()
{
  return m_width;
}

unsigned CobaltTexture::Height()
{
  return m_height;
}

ID3D11ShaderResourceView* CobaltTexture::GetTexture()
{
  if (m_textureView)
  {
    return m_textureView;
  }
  return 0;
}

bool CobaltTexture::LoadTexture(LPCWSTR filename)
{
  const CachedImage* image = EngineHandle->Cache->GetImage(filename);
  m_textureData = image->GetImage();
  if (!m_textureData)
  {
    return false;
  }

  m_width = image->Width();
  m_height = image->Height();

  return true;
}