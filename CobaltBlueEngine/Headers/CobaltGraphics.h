#ifndef COBALTBLUEGRAPHICS
#define COBALTBLUEGRAPHICS

#include "CobaltBlueCore.h"
#include "CobaltCamera.h"

#include "Direct3d.h"
#include "ShaderCore.h"
#include "Model3d.h"
#include "CobaltSprite.h"

#include <unordered_map>

class CobaltGraphics
{
public:
  friend class Sprite;
  friend class Model3D;

  bool Initialize(unsigned width, unsigned height, bool fullScreen, HWND window, AntiAlias antiAlias);
  void Shutdown();
  bool Frame(float frameTime);

  CobaltCamera Camera;

protected:
  unordered_map<int, Model3D*> m_modelListings;
  std::vector<Model2D*> m_bitmapListings;
  Direct3D m_DirectX;

private:
  bool Render(float frameTime);
  unsigned m_width;
  unsigned m_height;
  bool m_full;
  HWND m_window;
  ShaderCore m_Shader;
};

#endif