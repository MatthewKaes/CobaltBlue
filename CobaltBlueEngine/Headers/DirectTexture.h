////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

#include "Direct3d.h"

enum class TextureType {
  Dynamic,
  Static,
  StaticMipMap
};

class DirectTexture
{
public:
  DirectTexture();
  ~DirectTexture();

  bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context, LPCWSTR filename, TextureType usage);
  void Shutdown();
  void Update(ID3D11DeviceContext* context);

  ID3D11ShaderResourceView* GetTexture();

private:
  bool LoadTGA(LPCWSTR filename);
  bool LoadTexture(LPCWSTR filename);

  unsigned char* m_textureData;
  unsigned m_width;
  unsigned m_height;
  TextureType m_type;
  ID3D11Texture2D* m_texture;
  ID3D11ShaderResourceView* m_textureView;
};

#endif