#include "CobaltSprite.h"

#include "CobaltEngine.h"

extern CobaltEngine* EngineHandle;

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
  Release();
}

void Sprite::Create(LPWSTR textureFile)
{
  m_z = 0;
  EngineHandle->Graphics->m_bitmapListings.push_back(this);

  // Create a bitmap to use for 2D graphics
  m_Bitmap = new CobaltBitmap();
  m_Bitmap->Initialize(
    EngineHandle->Graphics->m_DirectX.GetDevice(),
    EngineHandle->Graphics->m_DirectX.GetDeviceContext(),
    textureFile);
}

void Sprite::Create(unsigned width, unsigned height)
{
  m_z = 0;
  EngineHandle->Graphics->m_bitmapListings.push_back(this);

  // Create a bitmap to use for 2D graphics
  m_Bitmap = new CobaltBitmap();
  //m_Bitmap->Initialize(
  //  EngineHandle->Graphics->m_DirectX.GetDevice(),
  //  EngineHandle->Graphics->m_DirectX.GetDeviceContext(),
  //  width,
  //  height);
}

int Sprite::GetZ()
{
  return m_z;
}

void Sprite::Release()
{
  // Remove form the bitmap listing.
  EngineHandle->Graphics->m_bitmapListings.erase(std::find(EngineHandle->Graphics->m_bitmapListings.begin(), EngineHandle->Graphics->m_bitmapListings.end(), this));

  // Release the texture object.
  if (m_Bitmap)
  {
    m_Bitmap->Release();
    delete m_Bitmap;
    m_Bitmap = 0;
  }

}

void Sprite::Render(ID3D11DeviceContext* context)
{
  m_Bitmap->Render(context);
}

int Sprite::GetIndexCount()
{
  return m_Bitmap->IndexCount;
}

ID3D11ShaderResourceView* Sprite::GetTexture()
{
  return m_Bitmap->GetTexture();
}