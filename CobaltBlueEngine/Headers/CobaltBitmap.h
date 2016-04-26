#ifndef COBALTBITMAP
#define COBALTBITMAP

#include "Direct3d.h"
#include "CobaltTexture.h"
#include "Model2d.h"

#undef DrawText

class Bitmap
{
public:
  Bitmap();
  ~Bitmap();

  void Create(LPCWSTR filename);
  void Create(unsigned width, unsigned height);
  void Update(ID3D11DeviceContext* context);
  void Release();
  
  void SetTextColor(Color textColor);
  void SetOutlineColor(Color outlineColor);
  void SetFont(LPWSTR fontname);

  void SetPixel(unsigned x, unsigned y, Color color);
  void Fill(Color color);
  void Fill(Rect area, Color color);
  void Gradient(Rect area, Color color1, Color color2);
  void Gradient(Rect area, Color color1, Color color2, bool horz);
  void DrawText(LPCWSTR text, unsigned size, Rect area);
  void DrawText(LPCWSTR text, unsigned size, Rect area, bool bold, bool italic);
  void Blend(Bitmap* bitmap, Rect area, Point target);
  void Blur(unsigned size);
  void FastBlur(unsigned size);
  void Greyscale();
  void HueRotate(float degrees);


  ID3D11ShaderResourceView* GetTexture();
  unsigned Width();
  unsigned Height();
  Color Pixel(unsigned x, unsigned y);

private:
  void BlurH(unsigned size);
  void BlurV(unsigned size);

  void InitializeResource(ID3D11Device* device, ID3D11DeviceContext* context);
  void LoadTexture(LPCWSTR filename);

  bool m_dirty;
  unsigned m_width;
  unsigned m_height;
  BYTE* m_textureData;
  ID3D11Texture2D* m_texture;
  ID3D11ShaderResourceView* m_textureView;
  Color m_textColor;
  Color m_outlineColor;
  LPCWSTR m_fontname;
};

#endif