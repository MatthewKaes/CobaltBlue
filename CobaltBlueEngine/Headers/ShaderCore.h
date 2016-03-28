#ifndef COBALTSHADERCORE
#define COBALTSHADERCORE

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
using namespace DirectX;
using namespace std;

class ShaderCore
{
public:
  ~ShaderCore();

  void Initialize(ID3D11Device* device, HWND window);
  void Shutdown();
  bool Render(ID3D11DeviceContext* context, int indexCount, XMMATRIX  worldMatrix, XMMATRIX  viewMatrix, XMMATRIX  projectionMatrix);

private:
  struct MatrixBufferType
  {
    XMMATRIX world;
    XMMATRIX view;
    XMMATRIX projection;
  };

  void InitializeShader(ID3D11Device* device, HWND window, WCHAR* vertexShader, WCHAR* pixelShader);
  void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

  void SetShaderParameters(ID3D11DeviceContext* context, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
  void RenderShader(ID3D11DeviceContext* context, int indexCount);

  ID3D11VertexShader* m_vertexShader;
  ID3D11PixelShader* m_pixelShader;
  ID3D11InputLayout* m_layout;
  ID3D11Buffer* m_matrixBuffer;
};

#endif