#ifndef COBALTBLUEENGINE
#define COBALTBLUEENGINE

#include "CobaltBlueCore.h"
#include "CobaltInput.h"
#include "CobaltGraphics.h"
#include "CobaltSynchronizer.h"
#include "CobaltAudio.h"
#include "CobaltCache.h"
#include "CobaltSound.h"
#include "CobaltScene.h"

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

  unsigned FPS();
  unsigned FrameCount();
  float FrameTime();

  LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

  // API Exposed Objects
  LPCWSTR Font;
  CobaltInput* Input;
  CobaltGraphics* Graphics;
  CobaltAudio* Audio;
  CobaltCache* Cache;
  CobaltSound* Sound;

  const unsigned DefaultFPS = 60;
  const LPCWSTR DefaultFont = L"Arial";

private:
  CobaltScene* m_currentScene;
  CobaltScene* m_nextScene;
  CobaltSynchronizer m_sync;

  LPCWSTR m_appName;
  HINSTANCE m_hinstance;
  HWND m_hwnd;
  bool m_exit;

};

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

#endif