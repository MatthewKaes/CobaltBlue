#ifndef COBALTPARALLAX
#define COBALTPARALLAX

#include "Model2d.h"
#include "CobaltBitmap.h"

class Parallax : public Model2D {
public:
  friend class CobaltGraphics;

  Parallax();
  ~Parallax();

  void Create(LPWSTR textureFile, unsigned width, unsigned height);
  void Create(unsigned width, unsigned height, unsigned imgWidth, unsigned imgHeight);
  unsigned ImgWidth();
  unsigned ImgHeight();
  void Update(ID3D11DeviceContext* context);
  void Render(ID3D11DeviceContext* context);
  int GetIndexCount();
  ID3D11ShaderResourceView* GetTexture();
  Bitmap* Bitmap();

  int Ox;
  int Oy;
  unsigned Width;
  unsigned Height;

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

  int m_prevOx;
  int m_prevOy;
  ::Bitmap* m_bitmap;
  ID3D11Buffer* m_vertexBuffer;
  ID3D11Buffer* m_indexBuffer;
};

#endif