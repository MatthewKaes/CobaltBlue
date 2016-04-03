#include "CobaltBlue.h" 

#pragma comment(lib, "CobaltBlueEngine.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#include <experimental/filesystem>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
  CobaltEngine engine(480, 480, L"Test App", false, 60, AntiAlias::MSAA4x);

  // Create a test model
  Model3D triangle(L"Textures\\stone.tga", TextureType::StaticMipMap);

  engine.Run();

  return 0;
}