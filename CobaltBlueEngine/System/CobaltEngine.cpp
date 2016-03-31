#include "CobaltEngine.h"

static CobaltEngine* EngineHandle = 0;
CobaltInput CobaltEngine::Input;
CobaltGraphics CobaltEngine::Graphics;

CobaltEngine::CobaltEngine(unsigned graphicsWidth, unsigned graphicsHeight, LPCWSTR appName, bool fullScreen, unsigned fps, AntiAlias antiAlias)
{
  WNDCLASSEX wc;
  DEVMODE dmScreenSettings;
  int posX, posY;

  // Get the instance of this application.
  m_hinstance = GetModuleHandle(NULL);

  // Give the application a name.
  m_appName = appName;

  // Setup the windows class with default settings.
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc.lpfnWndProc = WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = m_hinstance;
  wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
  wc.hIconSm = wc.hIcon;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = m_appName;
  wc.cbSize = sizeof(WNDCLASSEX);

  // Register the window class.
  RegisterClassEx(&wc);

  // Determine the resolution of the clients desktop screen.
  int screenWidth = GetSystemMetrics(SM_CXSCREEN);
  int screenHeight = GetSystemMetrics(SM_CYSCREEN);

  // Setup the screen settings depending on whether it is running in full screen or in windowed mode.
  m_fullScreenMode = fullScreen;
  if (m_fullScreenMode)
  {
    // If full screen set the screen to maximum size of the users desktop and 32bit.
    memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
    dmScreenSettings.dmSize = sizeof(dmScreenSettings);
    dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
    dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
    dmScreenSettings.dmBitsPerPel = 32;
    dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    // Change the display settings to full screen.
    ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

    // Set the position of the window to the top left corner.
    posX = posY = 0;
  }
  else
  {
    screenWidth = graphicsWidth;
    screenHeight = graphicsHeight;

    // Place the window in the middle of the screen.
    posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
    posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
  }

  // Create the window with the screen settings and get the handle to it.
  m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, 
    m_appName, 
    m_appName,
    (WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_POPUP),
    posX, 
    posY, 
    screenWidth, 
    screenHeight, 
    NULL, 
    NULL, 
    m_hinstance, 
    NULL);

  // Bring the window up on the screen and set it as main focus.
  ShowWindow(m_hwnd, SW_SHOW);
  SetForegroundWindow(m_hwnd);
  SetFocus(m_hwnd);

  // Hide the mouse cursor.
  if (fullScreen)
  {
    ShowCursor(false);
  }

  Graphics.Initialize(screenWidth, screenHeight, fullScreen, fps, m_hwnd, antiAlias);

  return;
}

CobaltEngine::~CobaltEngine()
{
  ShowCursor(true);

  // Fix the display settings if leaving full screen mode.
  if (m_fullScreenMode)
  {
    ChangeDisplaySettings(NULL, 0);
  }

  DestroyWindow(m_hwnd);
  m_hwnd = NULL;

  UnregisterClass(m_appName, m_hinstance);
  m_hinstance = NULL;

  // Release the pointer to this class.
  EngineHandle = NULL;

  return;
}

void CobaltEngine::Run()
{
  MSG msg;
  bool done;

  ZeroMemory(&msg, sizeof(MSG));

  done = false;
  while (!done)
  {
    // Handle the windows messages.
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    // If windows signals to end the application then exit out.
    if (msg.message == WM_QUIT || !Frame())
    {
      done = true;
    }
  }

  return;
}

bool CobaltEngine::Frame()
{
  if (Input.Pressed(Inputs::Esc))
    return false;

  return Graphics.Frame();
}

LRESULT CALLBACK CobaltEngine::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
  switch (umsg)
  {
    case WM_KEYDOWN:
    {
      Input.KeyDown((unsigned int)wparam);
      return 0;
    }
    case WM_KEYUP:
    {
      Input.KeyUp((unsigned int)wparam);
      return 0;
    }
    default:
    {
      return DefWindowProc(hwnd, umsg, wparam, lparam);
    }
  }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
  switch (umessage)
  {
  // Check if the window is being destroyed.
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  // Check if the window is being closed.
  case WM_CLOSE:
    PostQuitMessage(0);
    return 0;
  // Process all other messages
  default:
    return EngineHandle->MessageHandler(hwnd, umessage, wparam, lparam);
  }
}