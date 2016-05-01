#ifndef COBALTANIMATION
#define COBALTANIMATION

#include "CobaltParallax.h"

class Animation : public Model2D {
public:
  void Create(LPWSTR textureFile, unsigned cellsX, unsigned cellsY);
  void Pause();
  void Play(unsigned frameTime);
  void SelectCell(unsigned X, unsigned Y);
  unsigned Width();
  unsigned Height();
  void Update(float frameTime);
  Bitmap* Bitmap();

  unsigned X;
  unsigned Y;
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