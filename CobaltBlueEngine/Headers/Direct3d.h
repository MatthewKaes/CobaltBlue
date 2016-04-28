#ifndef COBALTDIRECT3D
#define COBALTDIRECT3D

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dx11.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <D3DX11.h>
#include <D3DX10math.h>

const float SCREEN_DEPTH = 1002.0f;
const float SCREEN_NEAR = 0.1f;

#define WINDOW_WIDTH_OFFSET = 6;
#define WINDOW_HEIGHT_OFFSET = 33;
#define MULTISAMPLE_QUALITY 0
enum class AntiAlias {
  None = 1,
  MSAA2x = 2,
  MSAA4x = 4,
  MSAA8x = 8,
};

class Direct3D
{
public:
  Direct3D();

  bool Initialize(int width, int height, bool vsync, HWND window, bool fullscreen, float screenDepth, float screenNear, AntiAlias antiAlias);
  void Shutdown();

  void BeginScene(float red, float green, float blue, float alpha);
  void EndScene();

  ID3D11Device* GetDevice();
  ID3D11DeviceContext* GetDeviceContext();

  void GetProjectionMatrix(D3DXMATRIX& projMatrix);
  void GetWorldMatrix(D3DXMATRIX& worldMatrix);
  void GetOrthoMatrix(D3DXMATRIX& orthoMatrix);
  void GetVideoCardInfo(char*, int&);
  bool VSync();

  void SetZBuffer(bool enable);
  void SetVSync(bool vsync);

private:
  bool m_vsync_enabled;
  int m_videoCardMemory;
  char m_videoCardDescription[128];
  IDXGISwapChain* m_swapChain;
  ID3D11Device* m_device;
  ID3D11DeviceContext* m_deviceContext;
  ID3D11RenderTargetView* m_renderTargetView;
  ID3D11Texture2D* m_depthStencilBuffer;
  ID3D11DepthStencilState* m_depthStencilState;
  ID3D11DepthStencilView* m_depthStencilView;
  ID3D11DepthStencilState* m_depthDisabledStencilState;
  ID3D11BlendState* m_alphaEnableBlendingState;
  ID3D11RasterizerState* m_rasterState;
  D3DXMATRIX m_projectionMatrix;
  D3DXMATRIX m_worldMatrix;
  D3DXMATRIX m_orthoMatrix;
};

#endif