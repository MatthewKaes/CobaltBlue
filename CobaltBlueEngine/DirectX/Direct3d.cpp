#include "Direct3d.h"

Direct3D::Direct3D()
{
  m_swapChain = 0;
  m_device = 0;
  m_deviceContext = 0;
  m_renderTargetView = 0;
  m_depthStencilBuffer = 0;
  m_depthStencilState = 0;
  m_depthDisabledStencilState = 0;
  m_depthStencilView = 0;
  m_rasterState = 0;
}

bool Direct3D::Initialize(int width, int height, bool vsync, HWND content, bool fullscreen, float screenDepth, float screenNear, AntiAlias antiAlias)
{
  HRESULT result;
  IDXGIFactory* factory;
  IDXGIAdapter* adapter;
  IDXGIOutput* adapterOutput;
  size_t stringLength;
  DXGI_ADAPTER_DESC adapterDesc;
  int error;
  DXGI_SWAP_CHAIN_DESC swapChainDesc;
  D3D_FEATURE_LEVEL featureLevel;
  ID3D11Texture2D* backBufferPtr;
  D3D11_TEXTURE2D_DESC depthBufferDesc;
  D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
  D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
  D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
  D3D11_BLEND_DESC blendStateDescription;
  D3D11_RASTERIZER_DESC rasterDesc;
  D3D11_VIEWPORT viewport;
  float fieldOfView, screenAspect;

  // Store the vsync setting.
  m_vsync_enabled = vsync;

  // Create a DirectX graphics interface factory.
  result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
  if (FAILED(result))
  {
    return false;
  }

  // Use the factory to create an adapter for the primary graphics interface (video card).
  result = factory->EnumAdapters(0, &adapter);
  if (FAILED(result))
  {
    return false;
  }

  // Enumerate the primary adapter output (monitor).
  result = adapter->EnumOutputs(0, &adapterOutput);
  if (FAILED(result))
  {
    return false;
  }

  result = adapter->GetDesc(&adapterDesc);
  if (FAILED(result))
  {
    return false;
  }

  // Store the dedicated video card memory in megabytes.
  m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

  // Convert the name of the video card to a character array and store it.
  error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
  if (error != 0)
  {
    return false;
  }

  // Release the adapter output.
  adapterOutput->Release();
  adapterOutput = 0;

  // Release the factory.
  factory->Release();
  factory = 0;

  // Initialize the swap chain description.
  ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

  swapChainDesc.BufferCount = 1;
  swapChainDesc.BufferDesc.Width = width;
  swapChainDesc.BufferDesc.Height = height;
  swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferDesc.RefreshRate.Numerator = 1000;
  swapChainDesc.BufferDesc.RefreshRate.Denominator = 60000;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.OutputWindow = content;
  swapChainDesc.SampleDesc.Count = (UINT)antiAlias;
  swapChainDesc.SampleDesc.Quality = MULTISAMPLE_QUALITY;
  swapChainDesc.Windowed = !fullscreen;
  swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
  swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
  featureLevel = D3D_FEATURE_LEVEL_11_0;

  unsigned int creationFlags = 0;
#ifdef _DEBUG
  creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  // Create the swap chain, Direct3D device, and Direct3D device context.
  result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
  if (FAILED(result))
  {
    return false;
  }

  // Get the pointer to the back buffer.
  result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
  if (FAILED(result))
  {
    return false;
  }

  // Create the render target view with the back buffer pointer.
  result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
  if (FAILED(result))
  {
    return false;
  }

  // Release pointer to the back buffer as we no longer need it.
  backBufferPtr->Release();
  backBufferPtr = 0;

  // Initialize the description of the depth buffer.
  ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

  // Set up the description of the depth buffer.
  depthBufferDesc.Width = width;
  depthBufferDesc.Height = height;
  depthBufferDesc.MipLevels = 1;
  depthBufferDesc.ArraySize = 1;
  depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  depthBufferDesc.SampleDesc.Count = (UINT)antiAlias;
  depthBufferDesc.SampleDesc.Quality = MULTISAMPLE_QUALITY;
  depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  depthBufferDesc.CPUAccessFlags = 0;
  depthBufferDesc.MiscFlags = 0;

  // Create the texture for the depth buffer using the filled out description.
  result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
  if (FAILED(result))
  {
    return false;
  }

  // Initialize the description of the stencil state.
  ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

  depthStencilDesc.DepthEnable = true;
  depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
  depthStencilDesc.StencilEnable = true;
  depthStencilDesc.StencilReadMask = 0xFF;
  depthStencilDesc.StencilWriteMask = 0xFF;
  depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
  depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
  depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
  depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  // Create the depth stencil state.
  result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
  if (FAILED(result))
  {
    return false;
  }

  // Clear the blend state description.
  ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

  //----------------------------------------
  // ALPHA BLENDING
  //----------------------------------------
  // Create an alpha enabled blend state description.
  blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
  blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
  blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
  blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
  blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
  blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
  m_device->CreateBlendState(&blendStateDescription, &m_alphaBlendingState);

  // Create the blend state using the description.
  blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
  blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
  blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
  blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
  blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  m_device->CreateBlendState(&blendStateDescription, &m_additiveBlendingState);

  // Create the blend state using the description.
  blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
  blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR;
  blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
  blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
  blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  m_device->CreateBlendState(&blendStateDescription, &m_mulBlendingState);

  // Default to alpha blend
  SetAlphaBlend();

  // Initialize the description of the 2d stencil state.
  ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

  depthDisabledStencilDesc.DepthEnable = false;
  depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
  depthDisabledStencilDesc.StencilEnable = true;
  depthDisabledStencilDesc.StencilReadMask = 0xFF;
  depthDisabledStencilDesc.StencilWriteMask = 0xFF;
  depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
  depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
  depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
  depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  // Create the depth stencil state.
	result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
  if (FAILED(result))
  {
    return false;
  }

  // Set the depth stencil state.
  m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

  // Initialize the depth stencil view.
  ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

  // Set up the depth stencil view description.
  depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
  depthStencilViewDesc.Texture2D.MipSlice = 0;

  // Create the depth stencil view.
  result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
  if (FAILED(result))
  {
    return false;
  }

  // Bind the render target view and depth stencil buffer to the output render pipeline.
  m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

  // Setup the raster description which will determine how and what polygons will be drawn.
  rasterDesc.AntialiasedLineEnable = true;
  rasterDesc.CullMode = D3D11_CULL_BACK;
  rasterDesc.DepthBias = 0;
  rasterDesc.DepthBiasClamp = 0.0f;
  rasterDesc.DepthClipEnable = true;
  rasterDesc.FillMode = D3D11_FILL_SOLID;
  rasterDesc.FrontCounterClockwise = false;
  rasterDesc.MultisampleEnable = true;
  rasterDesc.ScissorEnable = false;
  rasterDesc.SlopeScaledDepthBias = 0.0f;

  // Create the rasterizer state from the description we just filled out.
  result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
  if (FAILED(result))
  {
    return false;
  }

  // Now set the rasterizer state.
  m_deviceContext->RSSetState(m_rasterState);

  // Setup the viewport for rendering.
  viewport.Width = (float)width;
  viewport.Height = (float)height;
  viewport.MinDepth = 0.0f;
  viewport.MaxDepth = 1.0f;
  viewport.TopLeftX = 0.0f;
  viewport.TopLeftY = 0.0f;

  // Create the viewport.
  m_deviceContext->RSSetViewports(1, &viewport);

  // Setup the projection matrix.
  fieldOfView = (float)D3DX_PI / 4.0f;
  screenAspect = (float)width / (float)height;

  // Create the projection matrix for 3D rendering.
  D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

  // Initialize the world matrix to the identity matrix.
  D3DXMatrixIdentity(&m_worldMatrix);

  // Create an orthographic projection matrix for 2D rendering.
  D3DXMatrixOrthoLH(&m_orthoMatrix, (float)width, (float)height, screenNear, screenDepth);

  return true;
}

void Direct3D::Release()
{
  // Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
  if (m_swapChain)
  {
    m_swapChain->SetFullscreenState(false, NULL);
  }

  if (m_rasterState)
  {
    m_rasterState->Release();
    m_rasterState = 0;
  }

  if (m_depthStencilView)
  {
    m_depthStencilView->Release();
    m_depthStencilView = 0;
  }

  if (m_depthStencilState)
  {
    m_depthStencilState->Release();
    m_depthStencilState = 0;
  }

  if (m_depthDisabledStencilState)
  {
    m_depthDisabledStencilState->Release();
    m_depthDisabledStencilState = 0;
  }

  if (m_depthStencilBuffer)
  {
    m_depthStencilBuffer->Release();
    m_depthStencilBuffer = 0;
  }

  if (m_alphaBlendingState)
  {
    m_alphaBlendingState->Release();
    m_alphaBlendingState = 0;
  }

  if (m_additiveBlendingState)
  {
    m_additiveBlendingState->Release();
    m_additiveBlendingState = 0;
  }

  if (m_mulBlendingState)
  {
    m_mulBlendingState->Release();
    m_mulBlendingState = 0;
  }

  if (m_renderTargetView)
  {
    m_renderTargetView->Release();
    m_renderTargetView = 0;
  }

  if (m_deviceContext)
  {
    m_deviceContext->Release();
    m_deviceContext = 0;
  }

  if (m_device)
  {
    m_device->Release();
    m_device = 0;
  }

  if (m_swapChain)
  {
    m_swapChain->Release();
    m_swapChain = 0;
  }

  return;
}

void Direct3D::BeginScene(float red, float green, float blue, float alpha)
{
  float color[4];

  // Setup the color to clear the buffer to.
  color[0] = red;
  color[1] = green;
  color[2] = blue;
  color[3] = alpha;

  // Clear the back buffer.
  m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

  // Clear the depth buffer.
  m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

  return;
}

void Direct3D::EndScene()
{
  // Present the back buffer to the screen since rendering is complete.
  if (m_vsync_enabled)
  {
    // Lock to screen refresh rate.
    m_swapChain->Present(1, 0);
  }
  else
  {
    // Present as fast as possible.
    m_swapChain->Present(0, 0);
  }

  return;
}

void Direct3D::SetAlphaBlend()
{
  float blendFactor[4];

  // Setup the blend factor.
  blendFactor[0] = 0.0f;
  blendFactor[1] = 0.0f;
  blendFactor[2] = 0.0f;
  blendFactor[3] = 0.0f;

  // Turn on the alpha blending.
  m_deviceContext->OMSetBlendState(m_alphaBlendingState, blendFactor, 0xffffffff);
}

void Direct3D::SetAddBlend()
{
  float blendFactor[4];

  // Setup the blend factor.
  blendFactor[0] = 0.0f;
  blendFactor[1] = 0.0f;
  blendFactor[2] = 0.0f;
  blendFactor[3] = 0.0f;

  // Turn on the alpha blending.
  m_deviceContext->OMSetBlendState(m_additiveBlendingState, blendFactor, 0xffffffff);
}

void Direct3D::SetMulBlend()
{
  float blendFactor[4];

  // Setup the blend factor.
  blendFactor[0] = 0.0f;
  blendFactor[1] = 0.0f;
  blendFactor[2] = 0.0f;
  blendFactor[3] = 0.0f;

  // Turn on the alpha blending.
  m_deviceContext->OMSetBlendState(m_mulBlendingState, blendFactor, 0xffffffff);
}

ID3D11Device* Direct3D::GetDevice()
{
  return m_device;
}

ID3D11DeviceContext* Direct3D::GetDeviceContext()
{
  return m_deviceContext;
}

void Direct3D::GetProjectionMatrix(D3DXMATRIX& projMatrix)
{
  projMatrix = m_projectionMatrix;
}

void Direct3D::GetWorldMatrix(D3DXMATRIX& worldMatrix)
{
  worldMatrix = m_worldMatrix;
}

void Direct3D::GetOrthoMatrix(D3DXMATRIX& orthoMatrix)
{
  orthoMatrix = m_orthoMatrix;
}

bool Direct3D::VSync()
{
  return m_vsync_enabled;
}

void Direct3D::GetVideoCardInfo(char* cardName, int& memory)
{
  strcpy_s(cardName, 128, m_videoCardDescription);
  memory = m_videoCardMemory;
}

ID3D11DepthStencilView* Direct3D::GetDepthStencilView()
{
  return m_depthStencilView;
}

void Direct3D::SetZBuffer(bool enable)
{
  if (enable)
  {
    m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
  }
  else
  {
    m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
  }
}

void Direct3D::SetVSync(bool vsync)
{
  m_vsync_enabled = vsync;
}

void Direct3D::SetRenderToScreen()
{	
  // Bind the render target view and depth stencil buffer to the output render pipeline.
  m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
}