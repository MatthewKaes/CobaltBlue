#ifndef COBALTWINDOW
#define COBALTWINDOW

#include "CobaltSprite.h"

class Window : public Model2D {
public:
  ~Window();
  void Create(LPCWSTR textureFile);
  void Create(Bitmap* src);
  unsigned Width();
  unsigned Height();
  void Update(float frameTime);
  void Hide(bool hide);
  Bitmap* Foreground();
  Bitmap* Background();

private:
  unsigned m_width;
  unsigned m_height;

  Sprite m_foreground;
  Sprite m_background;
};

#endif