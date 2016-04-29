#ifndef COBALTANIMATION
#define COBALTANIMATION

#include "CobaltParallax.h"

class Animation : public Model2D {
public:
  void Create(LPWSTR textureFile, unsigned cellsX, unsigned cellsY);
  void Pause();
  void Play(unsigned frameTime);
  unsigned Width();
  unsigned Height();
  void Update();
  Bitmap* Bitmap();

  unsigned X;
  unsigned Y;
  bool YRoll;

private:
  unsigned m_width;
  unsigned m_height;
  unsigned m_cellX;
  unsigned m_cellY;
  unsigned m_cellCountX;
  unsigned m_cellCountY;
  unsigned m_frameCount;
  unsigned m_frameTime;

  Parallax m_parallax;
};

#endif