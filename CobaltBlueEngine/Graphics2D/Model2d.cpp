#include "Model2d.h"

Color::Color()
{
  Red = 255;
  Green = 255;
  Blue = 255;
  Alpha = 255;
}

Color::Color(BYTE red, BYTE green, BYTE blue)
{
  Red = red;
  Green = green;
  Blue = blue;
  Alpha = 255;
}

Color::Color(BYTE red, BYTE green, BYTE blue, BYTE alpha)
{
  Red = red;
  Green = green;
  Blue = blue;
  Alpha = alpha;
}

Point::Point()
{
  X = 0;
  Y = 0;
}

Point::Point(int x, int y)
{
  X = x;
  Y = y;
}

Point::Point(PointReal pr)
{
  X = (int)pr.X;
  Y = (int)pr.Y;
}

bool Point::operator==(const Point& rhs)
{
  return X == rhs.X && Y == rhs.Y;
}

bool Point::operator!=(const Point& rhs)
{
  return !operator==(rhs);
}

Point Point::operator*(int scale)
{
  return Point(X * scale, Y * scale);
}

PointReal::PointReal()
{
  X = 0.0f;
  Y = 0.0f;
}

PointReal::PointReal(float x, float y)
{
  X = x;
  Y = y;
}

PointReal PointReal::operator*(float scale)
{
  return PointReal(X * scale, Y * scale);
}

Rect::Rect()
{
  X = 0;
  Y = 0;
  Width = 0;
  Height = 0;
}

Rect::Rect(int x, int y, unsigned width, unsigned height)
{
  X = x;
  Y = y;
  Width = width;
  Height = height;
}

bool Rect::Contains(Point pt)
{
  return pt.X >= X && pt.X < X + (int)Width && pt.Y >= Y && pt.Y < Y + (int)Height;
}

void Model2D::Flash(Color color, float duration)
{
  m_colorSrc = Tint;
  m_colorTgt = color;
  m_totalDur = 0.0f;
  m_effectDur = duration;
}

void Model2D::Frame(float frameTime)
{
  if (!m_effectDur)
    return;

  m_totalDur += frameTime;

  if (m_totalDur >= m_effectDur)
  {
    m_effectDur = 0.0f;
    Tint = m_colorTgt;
  }
  else
  {
    float frac = m_totalDur / m_effectDur;
    Tint.Red = (BYTE)(m_colorSrc.Red * (1.0f - frac) + m_colorTgt.Red * frac);
    Tint.Green = (BYTE)(m_colorSrc.Green * (1.0f - frac) + m_colorTgt.Green * frac);
    Tint.Blue = (BYTE)(m_colorSrc.Blue * (1.0f - frac) + m_colorTgt.Blue * frac);
    Tint.Alpha = (BYTE)(m_colorSrc.Alpha * (1.0f - frac) + m_colorTgt.Alpha * frac);
  }
}