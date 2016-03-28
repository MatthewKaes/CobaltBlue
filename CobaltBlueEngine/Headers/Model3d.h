#ifndef COBALTMODEL3D
#define COBALTMODEL3D

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class Model3D {
public:
  Model3D();
  ~Model3D();
  void Release();
  void Render(ID3D11DeviceContext* context);

  int GetIndexCount();

private:
  struct VertexType
  {
    XMFLOAT3 position;
    XMFLOAT4 color;
  };

  bool InitializeBuffers(ID3D11Device* device);

  ID3D11Buffer *m_vertexBuffer;
  ID3D11Buffer *m_indexBuffer;
  int m_vertexCount;
  int m_indexCount;
  unsigned m_id;

  static unsigned ModelID;
};

#endif