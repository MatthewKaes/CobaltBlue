#ifndef COBALTBITMAP
#define COBALTBITMAP

#include "Direct3d.h"
#include "CobaltTexture.h"
#include "Model2d.h"

#undef DrawText

enum class TextAlign
{
  Left = 0,
  Center = 1,
  Right = 2
};

class Bitmap
{
public:
  Bitmap();
  ~Bitmap();

  void Create(LPCWSTR filename);
  void Create(unsigned width, unsigned height);
  void Update();
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
  void DrawText(LPCWSTR text, unsigned size, Rect area, TextAlign align);
  void DrawText(LPCWSTR text, unsigned size, Rect area, TextAlign align, bool bold, bool italic);
  void DrawScreen(Rect area, Rect source);
  void Blend(const Bitmap* bitmap, Rect area, Point target);
  void Blt(const Bitmap* bitmap, Rect area, Point target);
  void Blur(unsigned size);
  void FastBlur(unsigned size);
  void Greyscale();
  void HueRotate(float degrees);


  ID3D11ShaderResourceView* GetTexture();
  unsigned Width() const;
  unsigned Height() const;
  Color Pixel(unsigned x, unsigned y) const;

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
  RealColor m_textColor;
  RealColor m_outlineColor;
  LPCWSTR m_fontname;
};

#endif