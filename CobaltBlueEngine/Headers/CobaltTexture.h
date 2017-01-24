#ifndef COBALTTEXTURE
#define COBALTTEXTURE

#include "Direct3d.h"

enum class TextureType {
  Dynamic,
  Static,
  StaticMipMap
};

class CobaltTexture
{
public:
  CobaltTexture();
  ~CobaltTexture();

  bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context, LPCWSTR filename, TextureType usage);
  void Release();
  void Update(ID3D11DeviceContext* context);
  unsigned Width();
  unsigned Height();
  ID3D11ShaderResourceView* GetTexture();

private:
  bool LoadTexture(LPCWSTR filename);

  unsigned char* m_textureData;
  unsigned m_width;
  unsigned m_height;
  TextureType m_type;
  ID3D11Texture2D* m_texture;
  ID3D11ShaderResourceView* m_textureView;
};

#endif