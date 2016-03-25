#ifndef COBALTBLUEENGINE
#define COBALTBLUEENGINE

#include "CobaltBlueCore.h"
#include "CobaltInput.h"
#include "CobaltGraphics.h"

class COBALTAPI CobaltEngine {
public:
  // Core Functions
  CobaltEngine(unsigned graphicsWidth, unsigned graphicsHeight, LPCWSTR appName, bool fullScreen);
  ~CobaltEngine();
  void Run();
  bool Frame();

  LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

  // API Exposed Objects
  static CobaltInput Input;
  static CobaltGraphics Graphics;
private:
  LPCWSTR m_appName;
  HINSTANCE m_hinstance;
  HWND m_hwnd;
  bool m_fullScreenMode;
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
static CobaltEngine* EngineHandle = 0;
CobaltInput CobaltEngine::Input;
CobaltGraphics CobaltEngine::Graphics;

#endif