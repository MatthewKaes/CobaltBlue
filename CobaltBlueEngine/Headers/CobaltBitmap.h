#ifndef COBALTBITMAP
#define COBALTBITMAP

#include "Direct3d.h"
#include "CobaltTexture.h"

class CobaltBitmap
{
public:
  static const int VertexCount = 4;
  static const int IndexCount = 4;

  CobaltBitmap();
  ~CobaltBitmap();

  bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context, LPCWSTR filename);
  void Release();

  void Render(ID3D11DeviceContext* context);
  void Update(ID3D11DeviceContext* context);

  ID3D11ShaderResourceView* GetTexture();
  unsigned Width();
  unsigned Height();

private:
  struct VertexType
  {
    XMFLOAT3 position;
    XMFLOAT4 color;
    XMFLOAT2 texture;
  };

  bool InitializeBuffers(ID3D11Device* device);

  CobaltTexture* m_texture;
  ID3D11Buffer* m_vertexBuffer;
  ID3D11Buffer* m_indexBuffer;
};

#endif