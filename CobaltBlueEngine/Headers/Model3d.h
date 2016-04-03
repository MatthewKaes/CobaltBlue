#ifndef COBALTMODEL3D
#define COBALTMODEL3D

#include <d3d11.h>
#include <directxmath.h>
#include "DirectTexture.h"
using namespace DirectX;

class Model3D {
public:
  Model3D();
  ~Model3D();
  void Create(LPWSTR textureFile, TextureType textureType);
  void Release();
  void Render(ID3D11DeviceContext* context);

  int GetIndexCount();
  ID3D11ShaderResourceView* GetTexture();

private:
  struct VertexType
  {
    XMFLOAT3 position;
    XMFLOAT4 color;
    XMFLOAT2 texture;
  };

  bool InitializeTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, LPWSTR filename, TextureType textureType);
  bool InitializeBuffers(ID3D11Device* device);

  DirectTexture* m_Texture;
  ID3D11Buffer* m_vertexBuffer;
  ID3D11Buffer* m_indexBuffer;
  int m_vertexCount;
  int m_indexCount;
  unsigned m_id;

  static unsigned ModelID;
};

#endif