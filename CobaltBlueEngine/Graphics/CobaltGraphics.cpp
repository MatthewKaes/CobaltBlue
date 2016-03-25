#define COBALTENGINEAPI
#include "CobaltGraphics.h"

bool CobaltGraphics::Initialize(unsigned width, unsigned height, bool fullScreen, HWND window)
{
  m_width = width;
  m_height = height;
  m_full = fullScreen;
  m_window = window;

  return true;
}

void CobaltGraphics::Shutdown()
{

}

bool CobaltGraphics::Frame()
{
  return Render();
}

bool CobaltGraphics::Render()
{
  return true;
}
