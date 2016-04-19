#ifndef COBALTMODEL2D
#define COBALTMODEL2D

#include <d3d11.h>

class Color {
public:
  Color();
  Color(BYTE Red, BYTE Green, BYTE Blue);
  Color(BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha);
  BYTE Red;
  BYTE Green;
  BYTE Blue;
  BYTE Alpha;
};

class Rect {
public:
  Rect();
  Rect(int x, int y, unsigned width, unsigned height);
  int X;
  int Y;
  unsigned Width;
  unsigned Height;
};


class Model2D {
public:
  virtual ~Model2D() { };
  virtual void Update(ID3D11DeviceContext* context) = 0;
  virtual void Render(ID3D11DeviceContext* context) = 0;
  virtual int GetIndexCount() = 0;
  virtual ID3D11ShaderResourceView* GetTexture() = 0;

  float Z;
  int X;
  int Y;
  Color Tint;
};

#endif