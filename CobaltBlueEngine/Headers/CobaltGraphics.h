#ifndef COBALTBLUEGRAPHICS
#define COBALTBLUEGRAPHICS

#include "CobaltBlueCore.h"
#include "CobaltCamera.h"

#include "Direct3d.h"
#include "ShaderCore.h"
#include "Model3d.h"
#include "Model2d.h"

#include <unordered_map>
#include <unordered_set>

class CobaltGraphics
{
public:
  bool Initialize(unsigned width, unsigned height, bool fullScreen, HWND window, AntiAlias antiAlias);
  void Shutdown();
  bool Frame(float frameTime);
  unsigned Width();
  unsigned Height();
  HWND Window();
  bool VSync();
  void SetVSync(bool vsync);

  CobaltCamera Camera;
  Direct3D DirectX;

private:
  bool Render(float frameTime);
  unsigned m_width;
  unsigned m_height;
  bool m_full;
  HWND m_window;
  ShaderCore m_Shader;
};

#endif