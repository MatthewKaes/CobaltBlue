#include "CobaltCache.h"
#include <wincodec.h>

#pragma comment(lib, "windowscodecs.lib")

CachedImage::CachedImage(LPCWSTR filename)
{
  m_imageData = nullptr;

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
  }

  hr = pDecoder->GetFrame(0, &bitmapSource);
  if (FAILED(hr))
  {
    pFactory->Release();
    pDecoder->Release();
  }

  pFactory->CreateFormatConverter(&converter);
  converter->Initialize(bitmapSource, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeMedianCut);

  bitmapSource->GetSize(&m_width, &m_height);

  // Calculate the size of the 32 bit image data.
  unsigned imageSize = m_width * m_height * 4;
  m_imageData = new unsigned char[imageSize];

  WICRect sourceRect;
  sourceRect.X = 0;
  sourceRect.Y = 0;
  sourceRect.Width = m_width;
  sourceRect.Height = m_height;
  converter->CopyPixels(&sourceRect, m_width * 4, imageSize, m_imageData);

  bitmapSource->Release();
  pDecoder->Release();
  pFactory->Release();
  converter->Release();
}

CachedImage::~CachedImage()
{
  delete m_imageData;
}

unsigned char* CachedImage::GetImage() const
{
  unsigned char* imageData = new unsigned char[m_width * m_height * 4];
  memcpy(imageData, m_imageData, m_width * m_height * 4);

  return imageData;
}

unsigned CachedImage::Width() const
{
  return m_width;
}

unsigned CachedImage::Height() const
{
  return m_height;
}

unsigned char* CachedImage::RawData()
{
  return m_imageData;
}

CobaltCache::~CobaltCache()
{
  for (auto itr : m_cacheMap)
  {
    delete itr.second;
  }
}

const CachedImage* CobaltCache::GetImage(LPCWSTR filename)
{
  if (m_cacheMap.find(filename) != m_cacheMap.end())
  {
    return m_cacheMap[filename];
  }
  else
  {
    CachedImage* image = new CachedImage(filename);

    if (image->RawData())
    {
      m_cacheMap[filename] = image;
    }

    return image;
  }
}
