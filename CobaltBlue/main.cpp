#include "CobaltBlue.h" 

#pragma comment(lib, "CobaltBlueEngine.lib")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
  CobaltEngine engine(640, 480, L"Test App", false);
  engine.Run();

  return 0;
}