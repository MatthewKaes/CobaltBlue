#include "CobaltBlue.h" 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
  CobaltEngine engine(600, 400, L"Test App", false);
  engine.Run();

  return 0;
}