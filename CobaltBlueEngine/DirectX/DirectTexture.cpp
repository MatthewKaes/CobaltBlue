#include "DirectTexture.h"
#include <stdio.h>
#include <wincodec.h>

#pragma comment(lib, "windowscodecs.lib")

struct TargaHeader
{
  unsigned char data1[12];
  unsigned short width;
  unsigned short height;
  unsigned char bpp;
  unsigned char data2;
};

DirectTexture::DirectTexture()
{
  m_textureView = 0;
}

DirectTexture::~DirectTexture()
{
  Shutdown();
}

bool DirectTexture::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, LPCWSTR filename, TextureType usage)
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

void DirectTexture::Shutdown()
{
  // Release the texture resource.
  if (m_textureView)
  {
    m_textureView->Release();
    m_textureView = 0;
  }

  return;
}

void DirectTexture::Update(ID3D11DeviceContext* context)
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

ID3D11ShaderResourceView* DirectTexture::GetTexture()
{
  if (m_textureView)
  {
    return m_textureView;
  }
  return 0;
}

bool DirectTexture::LoadTGA(LPCWSTR filename) 
{
  int bpp;
  int imageSize;
  FILE* filePtr;
  unsigned int count;
  TargaHeader targaFileHeader;
  unsigned char* targaImage;

  if (_wfopen_s(&filePtr, filename, L"rb"))
  {
    return false;
  }

  // Read in the file header.
  count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
  if (count != 1)
  {
    return false;
  }

  // Get the important information from the header.
  m_height = (int)targaFileHeader.height;
  m_width = (int)targaFileHeader.width;
  bpp = (int)targaFileHeader.bpp;

  // Check that it is 32 bit and not 24 bit.
  if (bpp != 32)
  {
    return false;
  }

  // Calculate the size of the 32 bit image data.
  imageSize = m_width * m_height * 4;

  // Allocate memory for the targa image data.
  targaImage = new unsigned char[imageSize];
  if (!targaImage)
  {
    return false;
  }

  // Read in the targa image data.
  count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
  if (count != imageSize)
  {
    return false;
  }

  // Close the file.
  if (fclose(filePtr))
  {
    return false;
  }

  // Allocate memory for the texture.
  m_textureData = new unsigned char[imageSize];
  if (!m_textureData)
  {
    return false;
  }

  // Initialize the index into the targa destination data array.
  int index = 0;

  // Initialize the index into the targa image data.
  int k = (m_width * m_height * 4) - (m_width * 4);

  // Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down.
  for (unsigned j = 0; j < m_height; j++)
  {
    for (unsigned i = 0; i< m_width; i++)
    {
      m_textureData[index + 0] = targaImage[k + 2];  // Red.
      m_textureData[index + 1] = targaImage[k + 1];  // Green.
      m_textureData[index + 2] = targaImage[k + 0];  // Blue
      m_textureData[index + 3] = targaImage[k + 3];  // Alpha

      k += 4;
      index += 4;
    }

    // Set the targa image data index back to the preceding row at the beginning of the column since its reading it in upside down.
    k -= (m_width * 8);
  }

  // Release the targa image data now that it was copied into the destination array.
  delete[] targaImage;
  targaImage = 0;

  return true;
}

bool DirectTexture::LoadTexture(LPCWSTR filename)
{
  if (!lstrcmpiW(filename + lstrlenW(filename) - lstrlenW(L".TGA"), L".TGA"))
  {
    return LoadTGA(filename);
  }

  CoInitialize(NULL);
  
  IWICBitmapDecoder* pDecoder = NULL;
  IWICImagingFactory* pFactory = NULL;
  IWICBitmapFrameDecode* bitmapSource = NULL;
  IWICFormatConverter* converter = NULL;

  HRESULT hr = CoCreateInstance(
    CLSID_WICImagingFactory,
    NULL,
    CLSCTX_INPROC_SERVER,
    IID_PPV_ARGS(&pFactory)
    );

  hr = pFactory->CreateDecoderFromFilename(filename, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
  if (FAILED(hr))
  {
    pFactory->Release();
    return false;
  }

  hr = pDecoder->GetFrame(0, &bitmapSource);
  if (FAILED(hr))
  {
    pFactory->Release();
    pDecoder->Release();
    return false;
  }

  pFactory->CreateFormatConverter(&converter);
  converter->Initialize(bitmapSource, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeMedianCut);

  bitmapSource->GetSize(&m_width, &m_height);

  // Calculate the size of the 32 bit image data.
  unsigned imageSize = m_width * m_height * 4;
  m_textureData = new unsigned char[imageSize];

  WICRect sourceRect;
  sourceRect.X = 0;
  sourceRect.Y = 0;
  sourceRect.Width = m_width;
  sourceRect.Height = m_height;
  converter->CopyPixels(&sourceRect, m_width * 4, imageSize, m_textureData);

  bitmapSource->Release();
  pDecoder->Release();
  pFactory->Release();
  converter->Release();

  return true;
}