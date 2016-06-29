#ifndef COBALTMODEL2D
#define COBALTMODEL2D

#include <D3DX11.h>
#include <D3DX10math.h>

enum class Lighting {
  None,
  Dynamic
};

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
  bool Contains(Point pt);
  int X;
  int Y;
  unsigned Width;
  unsigned Height;
};

class Model2D {
public:
  virtual ~Model2D() { };
  virtual void Update(float frameTime) = 0;
  virtual void Render() { };
  virtual int GetIndexCount() { return 0; };
  virtual D3DXVECTOR2 Dimensions() { return D3DXVECTOR2(); };
  virtual ID3D11ShaderResourceView* GetTexture() { return nullptr; };

  void Flash(Color color, float duration);
  int Z;
  int X;
  int Y;
  Color Tint;
  Lighting Light = Lighting::None;

protected:
  void Frame(float frameTime);

private:
  float m_effectDur;
  float m_totalDur;
  Color m_colorSrc;
  Color m_colorTgt;
};

#endif