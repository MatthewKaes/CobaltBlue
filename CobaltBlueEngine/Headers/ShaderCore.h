#ifndef COBALTSHADERCORE
#define COBALTSHADERCORE

#include <D3DX11.h>
#include <D3Dcompiler.h>
#include <D3DX10math.h>
#include <fstream>
using namespace std;

class ShaderCore
{
public:
  ShaderCore();
  ~ShaderCore();

  void Initialize(ID3D11Device* device, HWND window);
  void Shutdown();
  bool Render2D(ID3D11DeviceContext* context, int indexCount, bool light, D3DXMATRIX& worldMatrix, D3DXMATRIX& viewMatrix, D3DXMATRIX& projectionMatrix, D3DXVECTOR4 translate, D3DXVECTOR4 color, D3DXVECTOR2 dimensions, ID3D11ShaderResourceView* texture);
  bool Render3D(ID3D11DeviceContext* context, int indexCount, D3DXMATRIX& worldMatrix, D3DXMATRIX& viewMatrix, D3DXMATRIX& projectionMatrix, ID3D11ShaderResourceView* texture);

private:
  struct BufferType3D
  {
    D3DXMATRIX world;
    D3DXMATRIX view;
    D3DXMATRIX projection;
  };

  struct BufferType2D
  {
    D3DXMATRIX world;
    D3DXMATRIX view;
    D3DXMATRIX projection;
    D3DXVECTOR4 trans;
    D3DXVECTOR4 color;
    D3DXVECTOR2 metrics;
    D3DXVECTOR2 buffer;
  };

  void InitializeShader2D(ID3D11Device* device, HWND window, WCHAR* vertexFile, WCHAR* pixelFile, ID3D11PixelShader** pixelShader, ID3D11VertexShader** vectorShader);
  void InitializeShader3D(ID3D11Device* device, HWND window, WCHAR* vertexShader, WCHAR* pixelShader);
  void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

  void SetShaderParameters2D(ID3D11DeviceContext* context, D3DXMATRIX& worldMatrix, D3DXMATRIX& viewMatrix, D3DXMATRIX& projectionMatrix, D3DXVECTOR4 translate, D3DXVECTOR4 color, D3DXVECTOR2 dimensions, ID3D11ShaderResourceView* texture);
  void SetShaderParameters3D(ID3D11DeviceContext* context, D3DXMATRIX& worldMatrix, D3DXMATRIX& viewMatrix, D3DXMATRIX& projectionMatrix, ID3D11ShaderResourceView* texture);
  void RenderShader2D(ID3D11DeviceContext* context, int indexCount, ID3D11PixelShader* pixelShader, ID3D11VertexShader* vectorShader);
  void RenderShader3D(ID3D11DeviceContext* context, int indexCount);

  ID3D11VertexShader* m_3DvertexShader;
  ID3D11PixelShader* m_3DpixelShader;
  ID3D11VertexShader* m_2DvertexShader;
  ID3D11PixelShader* m_2DpixelShader;
  ID3D11VertexShader* m_2DvertexShaderLight;
  ID3D11PixelShader* m_2DpixelShaderLight;
  ID3D11InputLayout* m_layout;
  ID3D11Buffer* m_2DmatrixBuffer;
  ID3D11Buffer* m_3DmatrixBuffer;
  ID3D11SamplerState* m_2DsampleState;
  ID3D11SamplerState* m_3DsampleState;
};

#endif