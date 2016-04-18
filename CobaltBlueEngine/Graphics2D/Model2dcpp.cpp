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