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