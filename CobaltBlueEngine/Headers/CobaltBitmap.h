#ifndef COBALTBITMAP
#define COBALTBITMAP

#include "Direct3d.h"
#include "CobaltTexture.h"
#include "Model2d.h"

#undef DrawText

class CobaltBitmap
{
public:
  CobaltBitmap();
  ~CobaltBitmap();

  void Create(ID3D11Device* device, ID3D11DeviceContext* context, LPCWSTR filename);
  void Create(ID3D11Device* device, ID3D11DeviceContext* context, unsigned width, unsigned height);
  void Update(ID3D11DeviceContext* context);
  void Release();
  
  Color GetPixel(unsigned x, unsigned y);
  void SetPixel(unsigned x, unsigned y, Color color);
  void Fill(Color color);
  void Fill(Rect area, Color color);
  void Gradient(Rect area, Color color1, Color color2);
  void Gradient(Rect area, Color color1, Color color2, bool horz);
  void DrawText(LPWSTR text, unsigned size, Rect area, Color textColor);
  void DrawText(LPWSTR text, unsigned size, Rect area, Color textColor, LPWSTR fontname);
  void DrawText(LPWSTR text, unsigned size, Rect area, Color textColor, LPWSTR fontname, bool bold, bool italic);

  ID3D11ShaderResourceView* GetTexture();
  unsigned Width();
  unsigned Height();

private:
  void InitializeResource(ID3D11Device* device, ID3D11DeviceContext* context);
  void LoadTexture(LPCWSTR filename);

  bool m_dirty;
  unsigned m_width;
  unsigned m_height;
  BYTE* m_textureData;
  ID3D11Texture2D* m_texture;
  ID3D11ShaderResourceView* m_textureView;
};

#endif