#ifndef COBALTDIRECT3D
#define COBALTDIRECT3D

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>

const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

using namespace DirectX;

class Direct3D
{
public:
  Direct3D();

  bool Initialize(int width, int height, bool vsync, HWND window, bool fullscreen, float screenDepth, float screenNear);
  void Shutdown();

  void BeginScene(float red, float green, float blue, float alpha);
  void EndScene();

  ID3D11Device* GetDevice();
  ID3D11DeviceContext* GetDeviceContext();

  void GetProjectionMatrix(XMMATRIX& projMatrix);
  void GetWorldMatrix(XMMATRIX& worldMatrix);
  void GetOrthoMatrix(XMMATRIX& orthoMatrix);

  void GetVideoCardInfo(char*, int&);

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
  ID3D11RasterizerState* m_rasterState;
  XMMATRIX m_projectionMatrix;
  XMMATRIX m_worldMatrix;
  XMMATRIX m_orthoMatrix;
};

#endif