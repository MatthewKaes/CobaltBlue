#ifndef COBALTMODEL2D
#define COBALTMODEL2D

#include <D3DX11.h>
#include <D3DX10math.h>

enum class Lighting {
  None,
  Source,
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

class RealColor {
public:
  RealColor();
  RealColor(Color color);
  RealColor(float Red, float Green, float Blue);
  RealColor(float Red, float Green, float Blue, float Alpha);
  float Red;
  float Green;
  float Blue;
  float Alpha;
};

class PointReal {
public:
  PointReal();
  PointReal(float x, float y);
  PointReal operator*(float scale);
  float X;
  float Y;
};

class Point {
public:
  Point();
  Point(int x, int y);
  Point(PointReal pr);
  bool operator==(const Point& rhs);
  bool operator!=(const Point& rhs);
  Point operator*(int scale);
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
  double Z;
  int X;
  int Y;
  float Zoom = 1.0f;
  Color Tint;
  Lighting Light = Lighting::None;
  bool Hide = false;

protected:
  void Frame(float frameTime);

private:
  float m_effectDur;
  float m_totalDur;
  Color m_colorSrc;
  Color m_colorTgt;
};

#endif