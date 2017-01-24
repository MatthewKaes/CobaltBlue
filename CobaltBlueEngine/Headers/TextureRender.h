#ifndef COBALTBLUEGRAPHICSTEXTURE
#define COBALTBLUEGRAPHICSTEXTURE
#include <d3d11.h>
#include <D3DX10math.h>
#include "Direct3D.h"

class TextureRender
{
public:
  TextureRender();
  ~TextureRender();

  bool Initialize(ID3D11Device* device, int screenWidth, int screenHeight);
  void Release();

  void SetRenderTarget(Direct3D* d3d);
  void ClearRenderTarget(Direct3D* d3d, float red, float green, float blue, float alpha);
  void Render(Direct3D* d3d);
  ID3D11ShaderResourceView* GetTexture(); 
  int GetIndexCount();

private:
  static const int VertexCount = 4;
  static const int IndexCount = 4;

  struct VertexType
  {
    D3DXVECTOR3 position;
    D3DXVECTOR4 color;
    D3DXVECTOR2 texture;
  };

  ID3D11Texture2D* m_renderTargetTexture;
  ID3D11RenderTargetView* m_renderTargetView;
  ID3D11ShaderResourceView* m_shaderResourceView;
  ID3D11Buffer* m_vertexBuffer;
  ID3D11Buffer* m_indexBuffer;
};

#endif