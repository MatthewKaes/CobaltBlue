#ifndef COBALTSPRITE
#define COBALTSPRITE

#include <directxmath.h>
#include "Model2d.h"
#include "CobaltBitmap.h"
using namespace DirectX;

class Sprite : public Model2D {
public:
  friend class CobaltGraphics;

  Sprite();
  ~Sprite();

  void Create(LPWSTR textureFile);
  void Create(unsigned width, unsigned height);
  void Render(ID3D11DeviceContext* context);
  int GetIndexCount();
  ID3D11ShaderResourceView* GetTexture();
  int GetZ();

private:
  void Release();

  CobaltBitmap* m_Bitmap;
  int m_z;
};

#endif