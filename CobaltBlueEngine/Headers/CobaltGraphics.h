#ifndef COBALTBLUEGRAPHICS
#define COBALTBLUEGRAPHICS

#include "CobaltBlueCore.h"
#include "CobaltCamera.h"

#include "Direct3d.h"
#include "ShaderCore.h"
#include "Model3d.h"

#include <unordered_map>

class CobaltGraphics
{
public:
  friend class Model3D;

  bool Initialize(unsigned width, unsigned height, bool fullScreen, HWND window);
  void Shutdown();
  bool Frame();

  CobaltCamera Camera;

protected:
  unordered_map<int, Model3D*> m_modelListings;
  Direct3D m_DirectX;

private:
  bool Render();
  unsigned m_width;
  unsigned m_height;
  bool m_full;
  HWND m_window;
  ShaderCore m_Shader;
};

#endif