#ifndef COBALTBLUEGRAPHICS
#define COBALTBLUEGRAPHICS

#include "CobaltBlueCore.h"
#include "Direct3d.h"

class CobaltGraphics
{
public:
  bool Initialize(unsigned width, unsigned height, bool fullScreen, HWND window);
  void Shutdown();
  bool Frame();

private:
  bool Render();
  unsigned m_width;
  unsigned m_height;
  bool m_full;
  HWND m_window;
  Direct3D m_DirectX;
};

#endif