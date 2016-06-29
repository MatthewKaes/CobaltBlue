#ifndef COBALTANIMATION
#define COBALTANIMATION

#include "CobaltParallax.h"

class Animation : public Model2D {
public:
  void Create(LPCWSTR textureFile, unsigned cellsX, unsigned cellsY);
  void Pause();
  void Play(unsigned frameTime);
  bool Playing();
  unsigned Width();
  unsigned Height();
  void Update(float frameTime);
  Bitmap* Bitmap();

  bool YRoll;
  int CellX;
  int CellY;

private:
  unsigned m_width;
  unsigned m_height;
  unsigned m_cellCountX;
  unsigned m_cellCountY;
  unsigned m_frameCount;
  unsigned m_frameTime;

  Parallax m_parallax;
};

#endif