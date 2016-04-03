#ifndef COBALTBLUEENGINE
#define COBALTBLUEENGINE

#include "CobaltBlueCore.h"
#include "CobaltInput.h"
#include "CobaltGraphics.h"
#include "CobaltScene.h"

class CobaltEngine {
public:
  // Core Functions
  CobaltEngine(unsigned graphicsWidth, unsigned graphicsHeight, LPCWSTR appName, bool fullScreen, unsigned fps, AntiAlias antiAlias);
  ~CobaltEngine();
  void Run(CobaltScene* entryScene);
  bool Frame();
  void Exit();
  void GotoScene(CobaltScene* nextScene);

  LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

  // API Exposed Objects
  static CobaltInput Input;
  static CobaltGraphics Graphics;
private:
  CobaltScene* m_currentScene;
  CobaltScene* m_nextScene;

  LPCWSTR m_appName;
  HINSTANCE m_hinstance;
  HWND m_hwnd;
  bool m_fullScreenMode;
  bool m_exit;
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

#endif