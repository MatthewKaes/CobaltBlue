#define COBALTENGINEAPI
#include "CobaltGraphics.h"

bool CobaltGraphics::Initialize(unsigned width, unsigned height, bool fullScreen, HWND window)
{
  m_width = width;
  m_height = height;
  m_full = fullScreen;
  m_window = window;

  return m_DirectX.Initialize(width, height, true, window, fullScreen, SCREEN_DEPTH, SCREEN_NEAR);
}

void CobaltGraphics::Shutdown()
{
  m_DirectX.Shutdown();
}

bool CobaltGraphics::Frame()
{
  return Render();
}

bool CobaltGraphics::Render()
{
  m_DirectX.BeginScene(0.0f, 1.0f, 1.0f, 1.0f);
  
  m_DirectX.EndScene();
  return true;
}
