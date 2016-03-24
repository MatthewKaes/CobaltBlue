#ifndef COBALTBLUEENGINE
#define COBALTBLUEENGINE

#include "CobaltBlueCore.h"

class COBALTAPI CobaltEngine {
public:
  CobaltEngine(unsigned graphicsWidth, unsigned graphicsHeight, LPCWSTR appName, bool fullScreen);
  void Run();

  LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
  LPCWSTR m_appName;
  HINSTANCE m_hinstance;
  HWND m_hwnd;
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
static CobaltEngine* EngineHandle = 0;

#endif