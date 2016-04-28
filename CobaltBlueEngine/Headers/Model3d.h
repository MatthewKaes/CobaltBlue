#ifndef COBALTMODEL3D
#define COBALTMODEL3D

#include <D3DX11.h>
#include <D3DX10math.h>
#include "CobaltTexture.h"

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
    D3DXVECTOR3 position;
    D3DXVECTOR4 color;
    D3DXVECTOR2 texture;
  };

  bool InitializeTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, LPWSTR filename, TextureType textureType);
  bool InitializeBuffers(ID3D11Device* device);

  CobaltTexture* m_Texture;
  ID3D11Buffer* m_vertexBuffer;
  ID3D11Buffer* m_indexBuffer;
  int m_vertexCount;
  int m_indexCount;
  unsigned m_id;

  static unsigned ModelID;
};

#endif