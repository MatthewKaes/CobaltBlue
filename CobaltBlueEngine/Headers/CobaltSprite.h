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
  unsigned Width();
  unsigned Height();
  void Update(ID3D11DeviceContext* context);
  void Render(ID3D11DeviceContext* context);
  int GetIndexCount();
  ID3D11ShaderResourceView* GetTexture();
  Bitmap* Bitmap();

private:
  static const int VertexCount = 4;
  static const int IndexCount = 4;

  struct VertexType
  {
    XMFLOAT3 position;
    XMFLOAT4 color;
    XMFLOAT2 texture;
  };

  void Release();
  void CreateBuffers(ID3D11Device* device);

  ::Bitmap* m_bitmap;
  ID3D11Buffer* m_vertexBuffer;
  ID3D11Buffer* m_indexBuffer;
};

#endif