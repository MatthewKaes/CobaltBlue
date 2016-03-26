#include "CobaltGraphics.h"

bool CobaltGraphics::Initialize(unsigned width, unsigned height, bool fullScreen, HWND window)
{
  m_width = width;
  m_height = height;
  m_full = fullScreen;
  m_window = window;

  // Set the initial position of the camera.
  Camera.SetPosition(0.0f, 0.0f, -10.0f);

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
  m_DirectX.BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

  // Generate the view matrix based on the camera's position.
  Camera.Update();

  m_DirectX.EndScene();
  return true;
}
