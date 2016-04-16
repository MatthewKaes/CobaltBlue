#ifndef COBALTMODEL2D
#define COBALTMODEL2D

#include <d3d11.h>

class Model2D {
public:
  virtual ~Model2D() { };
  virtual void Render(ID3D11DeviceContext* context) = 0;
  virtual int GetIndexCount() = 0;
  virtual ID3D11ShaderResourceView* GetTexture() = 0;
  virtual int GetZ() = 0;
};

#endif