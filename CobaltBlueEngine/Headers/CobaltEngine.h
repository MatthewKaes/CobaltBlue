#ifndef COBALTBLUEENGINE
#define COBALTBLUEENGINE

#include "CobaltBlueCore.h"
#include "CobaltInput.h"
#include "CobaltGraphics.h"
#include "CobaltScene.h"
#include "CobaltSynchronizer.h"
#include "CobaltAudio.h"
#include "CobaltCache.h"

#define DEFAULTFPS 60

class CobaltEngine {
public:
  // Core Functions
  CobaltEngine(unsigned graphicsWidth, unsigned graphicsHeight, LPCWSTR appName, bool fullScreen, AntiAlias antiAlias);
  ~CobaltEngine();
  void SetFPS(unsigned fps);
  void Run(CobaltScene* entryScene);
  bool Frame(float frameTime);
  void Exit();
  void GotoScene(CobaltScene* nextScene);

  LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

  // API Exposed Objects
  CobaltInput* Input;
  CobaltGraphics* Graphics;
  CobaltAudio* Audio;
  CobaltCache* Cache;
private:
  CobaltScene* m_currentScene;
  CobaltScene* m_nextScene;
  CobaltSynchronizer m_sync;

  LPCWSTR m_appName;
  HINSTANCE m_hinstance;
  HWND m_hwnd;
  bool m_fullScreenMode;
  bool m_exit;
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

#endif