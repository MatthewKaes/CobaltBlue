#ifndef COBALTPARALLAX
#define COBALTPARALLAX

#include "Model2d.h"
#include "CobaltBitmap.h"

class Parallax : public Model2D {
public:
  Parallax();
  ~Parallax();

  void Create(LPCWSTR textureFile, unsigned width, unsigned height);
  void Create(unsigned width, unsigned height, unsigned imgWidth, unsigned imgHeight);
  void Create(Bitmap* src, unsigned width, unsigned height);
  unsigned ImgWidth();
  unsigned ImgHeight();
  void Update(float frameTime);
  void Render();
  int GetIndexCount();
  ID3D11ShaderResourceView* GetTexture();
  D3DXVECTOR2 Dimensions();
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

  int m_own;
  int m_prevOx;
  int m_prevOy;
  int m_prevWidth;
  int m_prevHeight;
  ::Bitmap* m_bitmap;
  ID3D11Buffer* m_vertexBuffer;
  ID3D11Buffer* m_indexBuffer;
};

#endif