#include "CobaltGraphics.h"
#include <algorithm>

unordered_map<int, Model3D*> g_modelListings;
std::vector<Model2D*> g_renderListings;
std::unordered_set<Model2D*> g_updateListings;

bool CobaltGraphics::Initialize(unsigned width, unsigned height, bool fullScreen, HWND window, AntiAlias antiAlias)
{
  m_width = width;
  m_height = height;
  m_full = fullScreen;
  m_window = window;

  // Set the initial position of the camera.
  Camera.SetPosition(0.0f, 0.0f, -10.0f);

  bool result = DirectX.Initialize(width, height, true, window, fullScreen, SCREEN_DEPTH, SCREEN_NEAR, antiAlias);
  if (!result)
  {
	  return false;
  }

  m_Shader.Initialize(DirectX.GetDevice(), window);

  return true;
}

void CobaltGraphics::Shutdown()
{
  DirectX.Shutdown();
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

bool CobaltGraphics::VSync()
{
  return DirectX.VSync();
}

void CobaltGraphics::SetVSync(bool vsync)
{
  DirectX.SetVSync(vsync);
}

bool CobaltGraphics::Render(float frameTime)
{
  D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

  DirectX.BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

  // Generate the view matrix based on the camera's position.
  Camera.Update(frameTime);

  DirectX.GetWorldMatrix(worldMatrix);
  Camera.GetView(viewMatrix);
  DirectX.GetProjectionMatrix(projectionMatrix);
  DirectX.GetOrthoMatrix(orthoMatrix);

  // Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
  for (auto model : g_modelListings)
  {
    model.second->Render(DirectX.GetDeviceContext());

    m_Shader.Render3D(
      DirectX.GetDeviceContext(), 
      model.second->GetIndexCount(), 
      worldMatrix, 
      viewMatrix, 
      projectionMatrix, 
      model.second->GetTexture());
  }

  D3DXMatrixIdentity(&worldMatrix);
  D3DXMatrixIdentity(&viewMatrix);
  viewMatrix._43 = (SCREEN_DEPTH) / 2.0f;

  DirectX.SetZBuffer(false);

  for (auto model : g_updateListings)
  {
    model->Update();
  }

  std::sort(g_renderListings.begin(), g_renderListings.end(), [](Model2D* a, Model2D* b) { return a->Z < b->Z; });
  for (auto model : g_renderListings)
  {
    model->Update();

    model->Render();

    m_Shader.Render2D(
      DirectX.GetDeviceContext(), 
      model->GetIndexCount(), 
      worldMatrix, viewMatrix, 
      orthoMatrix,
      D3DXVECTOR4((float)model->X, (float)model->Y, model->Z, 0.0f),
      D3DXVECTOR4(model->Tint.Red / 255.0f, model->Tint.Blue / 255.0f, model->Tint.Green / 255.0f, model->Tint.Alpha / 255.0f),
      model->GetTexture());
  }
  DirectX.SetZBuffer(true);

  DirectX.EndScene();
  return true;
}
