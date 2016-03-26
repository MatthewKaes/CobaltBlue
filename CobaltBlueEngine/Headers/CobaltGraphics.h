#ifndef COBALTBLUEGRAPHICS
#define COBALTBLUEGRAPHICS

#include "CobaltBlueCore.h"
#include "CobaltCamera.h"

class CobaltGraphics
{
public:
  bool Initialize(unsigned width, unsigned height, bool fullScreen, HWND window);
  void Shutdown();
  bool Frame();

  CobaltCamera Camera;

private:
  bool Render();
  unsigned m_width;
  unsigned m_height;
  bool m_full;
  HWND m_window;
  Direct3D m_DirectX;
};

#endif