#ifndef COBALTSPRITE
#define COBALTSPRITE

#include "Model2d.h"
#include "CobaltBitmap.h"

class Sprite : public Model2D {
public:
  Sprite();
  ~Sprite();

  void Create(LPCWSTR textureFile);
  void Create(unsigned width, unsigned height);
  void Create(Bitmap* src);
  unsigned Width();
  unsigned Height();
  void Update(float frameTime);
  void Render();
  int GetIndexCount();
  ID3D11ShaderResourceView* GetTexture();
  D3DXVECTOR2 Dimensions();
  Bitmap* Bitmap();

private:
  static const int VertexCount = 4;
  static const int IndexCount = 4;

  struct VertexType
  {
    D3DXVECTOR3 position;
    D3DXVECTOR4 color;
    D3DXVECTOR2 texture;
  };

  void Release();
  void CreateBuffers(ID3D11Device* device);

  bool m_own;
  ::Bitmap* m_bitmap;
  ID3D11Buffer* m_vertexBuffer;
  ID3D11Buffer* m_indexBuffer;
};

#endif