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

unsigned CobaltGraphics::Width()
{
  return m_width;
}

unsigned CobaltGraphics::Height()
{
  return m_height;
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

    m_Shader.Render3D(
      m_DirectX.GetDeviceContext(), 
      model.second->GetIndexCount(), 
      worldMatrix, 
      viewMatrix, 
      projectionMatrix, 
      model.second->GetTexture());
  }

  worldMatrix = XMMatrixIdentity();
  viewMatrix = XMMatrixIdentity();
  viewMatrix.r[3].m128_f32[2] = (SCREEN_DEPTH) / 2.0f;

  m_DirectX.SetZBuffer(false);
  std::sort(m_bitmapListings.begin(), m_bitmapListings.end(), [](Model2D* a, Model2D* b) { return a->Z < b->Z; });
  for (auto model : m_bitmapListings)
  {
    model->Update(m_DirectX.GetDeviceContext());

    model->Render(m_DirectX.GetDeviceContext());

    m_Shader.Render2D(
      m_DirectX.GetDeviceContext(), 
      model->GetIndexCount(), 
      worldMatrix, viewMatrix, 
      orthoMatrix,
      XMVectorSet((float)model->X, (float)model->Y, model->Z, 0.0f),
      XMVectorSet(model->Tint.Red / 255.0f, model->Tint.Blue / 255.0f, model->Tint.Green / 255.0f, model->Tint.Alpha / 255.0f),
      model->GetTexture());
  }
  m_DirectX.SetZBuffer(true);

  m_DirectX.EndScene();
  return true;
}
