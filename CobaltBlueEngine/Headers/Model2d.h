#ifndef COBALTMODEL2D
#define COBALTMODEL2D

#include <D3DX11.h>
#include <D3DX10math.h>

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

class Point {
public:
  Point();
  Point(int x, int y);
  int X;
  int Y;
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