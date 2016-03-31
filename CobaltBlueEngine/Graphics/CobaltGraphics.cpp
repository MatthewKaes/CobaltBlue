#include "CobaltGraphics.h"

bool CobaltGraphics::Initialize(unsigned width, unsigned height, bool fullScreen, unsigned fps, HWND window, AntiAlias antiAlias)
{
  m_width = width;
  m_height = height;
  m_full = fullScreen;
  m_window = window;

  // Set the initial position of the camera.
  Camera.SetPosition(0.0f, 0.0f, -10.0f);

  bool result = m_DirectX.Initialize(width, height, true, window, fullScreen, SCREEN_DEPTH, SCREEN_NEAR, fps, antiAlias);
  if (!result)
  {
	  return false;
  }

  m_Shader.Initialize(m_DirectX.GetDevice(), window);

  return true;
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
  XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

  m_DirectX.BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

  // Generate the view matrix based on the camera's position.
  Camera.Update();

  m_DirectX.GetWorldMatrix(worldMatrix);
  Camera.GetView(viewMatrix);
  m_DirectX.GetProjectionMatrix(projectionMatrix);


  // Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
  for (auto model : m_modelListings)
  {
    model.second->Render(m_DirectX.GetDeviceContext());

    m_Shader.Render(m_DirectX.GetDeviceContext(), model.second->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
  }

  m_DirectX.EndScene();
  return true;
}
