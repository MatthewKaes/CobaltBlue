#include "CobaltGraphics.h"
#include <algorithm>
bool CobaltGraphics::Initialize(unsigned width, unsigned height, bool fullScreen, HWND window, AntiAlias antiAlias)
{
  m_width = width;
  m_height = height;
  m_full = fullScreen;
  m_window = window;

  // Set the initial position of the camera.
  Camera.SetPosition(0.0f, 0.0f, -10.0f);

  bool result = m_DirectX.Initialize(width, height, true, window, fullScreen, SCREEN_DEPTH, SCREEN_NEAR, antiAlias);
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

bool CobaltGraphics::Frame(float frameTime)
{
  return Render(frameTime);
}

bool CobaltGraphics::Render(float frameTime)
{
  XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

  m_DirectX.BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

  // Generate the view matrix based on the camera's position.
  Camera.Update(frameTime);

  m_DirectX.GetWorldMatrix(worldMatrix);
  Camera.GetView(viewMatrix);
  m_DirectX.GetProjectionMatrix(projectionMatrix);
  m_DirectX.GetOrthoMatrix(orthoMatrix);


  // Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
  for (auto model : m_modelListings)
  {
    model.second->Render(m_DirectX.GetDeviceContext());

    m_Shader.Render(m_DirectX.GetDeviceContext(), model.second->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, model.second->GetTexture());
  }

  
  m_DirectX.SetZBuffer(false);
  worldMatrix = XMMatrixIdentity();
  viewMatrix = XMMatrixIdentity();
  viewMatrix.r[3].m128_f32[2] = 10.0f;

  std::sort(m_bitmapListings.begin(), m_bitmapListings.end(), [](Model2D* a, Model2D* b) { return a->GetZ() < b->GetZ(); });
  for (auto bitmap : m_bitmapListings)
  {
    bitmap->Render(m_DirectX.GetDeviceContext());

    m_Shader.Render(m_DirectX.GetDeviceContext(), bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, bitmap->GetTexture());
  }
  m_DirectX.SetZBuffer(true);

  m_DirectX.EndScene();
  return true;
}
