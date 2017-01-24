#include "CobaltEngine.h"
#include "CobaltBitmap.h"

extern CobaltEngine* EngineHandle;

#define KERNELSIZE 8

const Color DefaultColor = Color(255, 255, 255);
const Color DefaultOutline = Color(0, 0, 0, 215);

Bitmap::Bitmap()
{
  m_texture = 0;
  m_textureView = 0;
  m_textColor = DefaultColor;
  m_outlineColor = DefaultOutline;
}

Bitmap::~Bitmap()
{
  Release();
}

void Bitmap::Create(LPCWSTR filename)
{
  Release();
  m_fontname = EngineHandle->Font;

  LoadTexture(filename);
  InitializeResource(
    EngineHandle->Graphics->DirectX.GetDevice(),
    EngineHandle->Graphics->DirectX.GetDeviceContext());
}

void Bitmap::Create(unsigned width, unsigned height)
{
  Release();
  m_fontname = EngineHandle->Font;

  m_textureData = new BYTE[width * height * 4];
  memset(m_textureData, 0, sizeof(BYTE) * width * height * 4);
  m_width = width;
  m_height = height;

  InitializeResource(
    EngineHandle->Graphics->DirectX.GetDevice(),
    EngineHandle->Graphics->DirectX.GetDeviceContext());
}

void Bitmap::Release()
{
  if (m_texture)
  {
    m_texture->Release();
    m_texture = 0;
  }

  if (m_textureView)
  {
    m_textureView->Release();
    m_textureView = 0;
  }

  if (m_textureData)
  {
    delete[] m_textureData;
    m_textureData = 0;
  }
}

void Bitmap::SetTextColor(Color textColor)
{
  m_textColor = textColor;
}

void Bitmap::SetOutlineColor(Color outlineColor)
{
  m_outlineColor = outlineColor;
}

void Bitmap::SetFont(LPWSTR fontname)
{
  m_fontname = fontname;
}

Color Bitmap::Pixel(unsigned x, unsigned y)
{
  if (x > Width() || y > Height())
    return Color();

  BYTE* colorIdx = m_textureData + x * 4 + y * 4 * Width();
  return Color(colorIdx[0], colorIdx[1], colorIdx[2], colorIdx[3]);
}

void Bitmap::SetPixel(unsigned x, unsigned y, Color color)
{
  if (x > Width() || y > Height())
    return;

  m_dirty = true;

  BYTE* colorIdx = m_textureData + x * 4 + y * 4 * Width();
  colorIdx[0] = color.Red;
  colorIdx[1] = color.Blue;
  colorIdx[2] = color.Green;
  colorIdx[3] = color.Alpha;
}

void Bitmap::Fill(Color color)
{
  Fill(Rect(0, 0, Width(), Height()), color);
}

void Bitmap::Fill(Rect area, Color color)
{
  if (area.X > (int)Width())
    return;

  if (area.Y > (int)Height())
    return;

  if (area.X < 0)
    area.X = 0;

  if (area.Y < 0)
    area.Y = 0;

  if (area.Width + area.X > Width())
    area.Width = Width() - area.X;

  if (area.Height + area.Y > Height())
    area.Height = Height() - area.Y;

  m_dirty = true;

  for (unsigned line = 0; line < area.Height; line++)
  {
    BYTE* pixelIdx = m_textureData + area.X * 4 + (area.Y + line) * 4 * Width();
    for (unsigned pixel = 0; pixel < area.Width; pixel++)
    {
      pixelIdx[0] = color.Red;
      pixelIdx[1] = color.Green;
      pixelIdx[2] = color.Blue;
      pixelIdx[3] = color.Alpha;
      pixelIdx += 4;
    }
  }
}

void Bitmap::Gradient(Rect area, Color color1, Color color2)
{
  Gradient(area, color1, color2, false);
}

void Bitmap::Gradient(Rect area, Color color1, Color color2, bool horz)
{
  if (area.X < 0)
    area.X = 0;

  if (area.Y < 0)
    area.Y = 0;

  if (area.Width + area.X > Width())
    area.Width = Width() - area.X;

  if (area.Height + area.Y > Height())
    area.Height = Height() - area.Y;

  m_dirty = true;

  if (!horz)
  {
    double end = (double)area.Y + area.Height;
    for (unsigned line = 0; line < area.Height; line++)
    {
      BYTE* pixelIdx = m_textureData + area.X * 4 + (area.Y + line) * 4 * Width();
      Color grad;
      double fract1 = (area.Height - line) / (float)area.Height;
      double fract2 = line / (float)area.Height;
      grad.Red = (UCHAR)(color1.Red * fract1 + color2.Red * fract2);
      grad.Green = (UCHAR)(color1.Green * fract1 + color2.Green * fract2);
      grad.Blue = (UCHAR)(color1.Blue * fract1 + color2.Blue * fract2);
      grad.Alpha = (UCHAR)(color1.Alpha * fract1 + color2.Alpha * fract2);
      for (unsigned pixel = 0; pixel < area.Width; pixel++)
      {
        pixelIdx[0] = grad.Red;
        pixelIdx[1] = grad.Green;
        pixelIdx[2] = grad.Blue;
        pixelIdx[3] = grad.Alpha;
        pixelIdx += 4;
      }
    }
  }
  else
  {
    double end = (double)area.X + area.Width;
    for (unsigned line = 0; line < area.Width; line++)
    {
      BYTE* pixelIdx = m_textureData + (area.X + line) * 4 + area.Y * 4 * Width();
      Color grad;
      double fract1 = (area.Width - line) / (float)area.Width;
      double fract2 = (line) / (float)area.Width;
      grad.Red = (UCHAR)(color1.Red * fract1 + color2.Red * fract2);
      grad.Green = (UCHAR)(color1.Green * fract1 + color2.Green * fract2);
      grad.Blue = (UCHAR)(color1.Blue * fract1 + color2.Blue * fract2);
      grad.Alpha = (UCHAR)(color1.Alpha * fract1 + color2.Alpha * fract2);
      for (unsigned pixel = 0; pixel < area.Height; pixel++)
      {
        pixelIdx[0] = grad.Red;
        pixelIdx[1] = grad.Green;
        pixelIdx[2] = grad.Blue;
        pixelIdx[3] = grad.Alpha;
        pixelIdx += Width() * 4;
      }
    }
  }
}

void Bitmap::DrawText(LPCWSTR text, unsigned size, Rect area)
{
  DrawText(text, size, area, true, false);
}

void Bitmap::DrawText(LPCWSTR text, unsigned size, Rect area, bool bold, bool italic)
{
  if (area.X < 0)
    area.X = 0;

  if (area.Y < 0)
    area.Y = 0;

  if (area.Width + area.X > Width())
    area.Width = Width() - area.X;

  if (area.Height + area.Y > Height())
    area.Height = Height() - area.Y;

  m_dirty = true;

  HDC hdc = GetDC(NULL);
  HDC mdc = CreateCompatibleDC(hdc);
  HBITMAP bm = CreateCompatibleBitmap(hdc, area.Width, area.Height);

  HFONT hf = CreateFont(size, 0, 0, 0, bold ? FW_BOLD : FW_NORMAL, italic, FALSE, FALSE,
    ANSI_CHARSET, OUT_STROKE_PRECIS, CLIP_DEFAULT_PRECIS,
    ANTIALIASED_QUALITY, DEFAULT_PITCH, m_fontname);

  RECT r = { 0, 0, (int)area.Width, (int)area.Height };

  BITMAPINFO bmi;
  bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
  bmi.bmiHeader.biWidth = area.Width;
  bmi.bmiHeader.biHeight = area.Height;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;

  SelectObject(mdc, bm);
  FillRect(mdc, &r, (HBRUSH)GetStockObject(NULL_BRUSH));

  SelectObject(mdc, hf);
  SetBkMode(mdc, TRANSPARENT);
  ::SetTextColor(mdc, RGB(255, 255, 255));
  ::DrawTextW(mdc, text, lstrlenW(text), &r, DT_LEFT | DT_EXTERNALLEADING | DT_WORDBREAK);

  BYTE* scanlines = new BYTE[area.Height * area.Width * 4 + 2 * KERNELSIZE];
  int* scanlinesBlocks = reinterpret_cast<int*>(scanlines);
  GetDIBits(mdc, bm, 0, area.Height, scanlines, &bmi, DIB_RGB_COLORS);
  memmove(scanlines + 2 * KERNELSIZE, scanlines, area.Height * area.Width * 4);

  for (unsigned line = 0; line < area.Height; line++)
  {
    BYTE* pixelIdx = m_textureData + area.X * 4 + (area.Y + line) * 4 * Width();
    for (unsigned pixel = 0; pixel < area.Width; pixel++)
    {
      BYTE* pixelSrc = scanlines + 4 * (area.Height - line - 1) * area.Width + pixel * 4;
      int* rawQuad = scanlinesBlocks + (area.Height - line - 1) * area.Width + pixel;

      int xLeft = max(min(pixel, KERNELSIZE), 0);
      int xRight = min(area.Width - pixel, KERNELSIZE);
      int xBottom = min(line, KERNELSIZE);
      int xTop = min(area.Height - line - 1, KERNELSIZE);
      bool shaded = false;
      int distSqr = (KERNELSIZE + 1) * (KERNELSIZE + 1);

      int ydiff = -xBottom;
      for (int y = line - xBottom; y <= (int)line + xTop; y++)
      {
        int xdiff = -xLeft;
        for (int x = pixel - xLeft; x <= (int)pixel + xRight; x++)
        {
          if (scanlinesBlocks[(area.Height - y - 1) * area.Width + x])
          {
            int dist = xdiff * xdiff + ydiff * ydiff;

            int* pixQuad = scanlinesBlocks + (area.Height - y - 1) * area.Width + x;
            float alphaVal = ((BYTE*)pixQuad)[0] * ((BYTE*)pixQuad)[1] * ((BYTE*)pixQuad)[2] / 16581375.0f;
            if (dist < distSqr && alphaVal >= 0.2f)
              distSqr = dist;

            shaded = true;
          }
          xdiff++;
        }
        ydiff++;
      }

      if (shaded)
      {
        float mod = 0.8f;
        for (int i = 4; i < distSqr; i += 1)
          mod *= 1.38f;

        if (mod > 0.05f)
        {
          float pixelAlpha = m_outlineColor.Alpha / max(255.0f * mod, 255.0f);
          pixelIdx[0] = (BYTE)(m_outlineColor.Red * pixelAlpha + (1 - pixelAlpha) * pixelIdx[0]);
          pixelIdx[1] = (BYTE)(m_outlineColor.Green * pixelAlpha + (1 - pixelAlpha) * pixelIdx[1]);
          pixelIdx[2] = (BYTE)(m_outlineColor.Blue * pixelAlpha + (1 - pixelAlpha) * pixelIdx[2]);
          pixelIdx[3] = (BYTE)(min(255 * pixelAlpha + (1 - pixelAlpha) * pixelIdx[3], 255));
        }
      }

      if (*rawQuad)
      {
        float pixelAlpha = (float)m_textColor.Alpha * pixelSrc[0] * pixelSrc[1] * pixelSrc[2] / 4228250625.0f;

        pixelIdx[0] = (BYTE)(m_textColor.Red * pixelAlpha + (1 - pixelAlpha) * pixelIdx[0]);
        pixelIdx[1] = (BYTE)(m_textColor.Green * pixelAlpha + (1 - pixelAlpha) * pixelIdx[1]);
        pixelIdx[2] = (BYTE)(m_textColor.Blue * pixelAlpha + (1 - pixelAlpha) * pixelIdx[2]);
        pixelIdx[3] = (BYTE)(min(255 * pixelAlpha + (1 - pixelAlpha) * pixelIdx[3], 255));
      }

      pixelIdx += 4;
    }
  }

  delete[] scanlines;
  DeleteObject(hf);
  DeleteObject(bm);
  DeleteDC(mdc);
  ReleaseDC(NULL, hdc);
}

void Bitmap::DrawScreen(Rect area, Rect source)
{
  if (source.X < 0)
    source.X = 0;

  if (source.Y < 0)
    source.Y = 0;

  if (source.Width + source.X > EngineHandle->Graphics->Width())
    source.Width = EngineHandle->Graphics->Width() - source.X;

  if (source.Height + source.Y > EngineHandle->Graphics->Height())
    source.Height = EngineHandle->Graphics->Height() - source.Y;

  if (area.X < 0)
    area.X = 0;

  if (area.Y < 0)
    area.Y = 0;

  if (area.Width + area.X > Width())
    area.Width = Width() - area.X;

  if (area.Height + area.Y > Height())
    area.Height = Height() - area.Y;

  area.Width = source.Width = min(area.Width, source.Width);
  area.Height = source.Height = min(area.Height, source.Height);

  m_dirty = true;

  HDC hdc = GetDC(EngineHandle->Graphics->Window());
  HDC mdc = CreateCompatibleDC(hdc);
  HBITMAP bm = CreateCompatibleBitmap(hdc, EngineHandle->Graphics->Width(), EngineHandle->Graphics->Height());

  SelectObject(mdc, bm);
  BitBlt(mdc, 0, 0, EngineHandle->Graphics->Width(), EngineHandle->Graphics->Height(), hdc, 0, 0, SRCCOPY);

  RECT r = { 0, 0, (int)area.Width, (int)area.Height };

  BITMAPINFO bmi;
  bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
  bmi.bmiHeader.biWidth = EngineHandle->Graphics->Width();
  bmi.bmiHeader.biHeight = EngineHandle->Graphics->Height();
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;

  BYTE* scanlines = new BYTE[EngineHandle->Graphics->Width() * EngineHandle->Graphics->Height() * 4];
  GetDIBits(mdc, bm, 0, EngineHandle->Graphics->Height(), scanlines, &bmi, DIB_RGB_COLORS);

  int sourceY = EngineHandle->Graphics->Height() - source.Y;
  for (unsigned line = 0; line < area.Height; line++)
  {
    BYTE* pixelIdx = m_textureData + area.X * 4 + (area.Y + line) * 4 * Width();
    BYTE* pixelSrc = scanlines + source.X * 4 + (sourceY - line - 1) * 4 * EngineHandle->Graphics->Width();
    for (unsigned i = 0; i < area.Width; i++)
    {
      pixelIdx[0] = pixelSrc[2];
      pixelIdx[1] = pixelSrc[1];
      pixelIdx[2] = pixelSrc[0];
      pixelIdx[3] = 255;
      pixelIdx += 4;
      pixelSrc += 4;
    }
  }

  delete[] scanlines;
  DeleteObject(bm);
  DeleteDC(mdc);
  ReleaseDC(NULL, hdc);
}

void Bitmap::Blend(Bitmap* bitmap, Rect area, Point target)
{
  if (!bitmap)
    return;

  if (area.X < 0)
    area.X = 0;

  if (area.Y < 0)
    area.Y = 0;

  if (area.Width + area.X > bitmap->Width())
    area.Width = bitmap->Width() - area.X;

  if (area.Height + area.Y > bitmap->Height())
    area.Height = bitmap->Height() - area.Y;

  if (area.Width + target.X > Width())
    area.Width = Width() - target.X;

  if (area.Height + target.Y > Height())
    area.Height = Height() - target.Y;

  m_dirty = true;

  for (unsigned line = 0; line < area.Height; line++)
  {
    BYTE* pixelIdx = m_textureData + target.X * 4 + (target.Y + line) * 4 * Width();
    BYTE* pixelSrc = bitmap->m_textureData + area.X * 4 + (area.Y + line) * 4 * bitmap->Width();
    for (unsigned pixel = 0; pixel < area.Width; pixel++)
    {
      float pixelAlpha = pixelSrc[3] / 255.0f;
      if (pixelAlpha != 0.0f)
      {
        pixelIdx[0] = (BYTE)(pixelSrc[0] * pixelAlpha + (1.0f - pixelAlpha) * pixelIdx[0]);
        pixelIdx[1] = (BYTE)(pixelSrc[1] * pixelAlpha + (1.0f - pixelAlpha) * pixelIdx[1]);
        pixelIdx[2] = (BYTE)(pixelSrc[2] * pixelAlpha + (1.0f - pixelAlpha) * pixelIdx[2]);
        pixelIdx[3] = (BYTE)(min(255 * pixelAlpha + (1 - pixelAlpha) * pixelIdx[3], 255));
      }

      pixelIdx += 4;
      pixelSrc += 4;
    }
  }
}

void Bitmap::Blt(Bitmap* bitmap, Rect area, Point target)
{
  if (!bitmap || area.X > (int)bitmap->Width() || area.Y > (int)bitmap->Height())
    return;

  if (area.X < 0)
    area.X = 0;

  if (area.Y < 0)
    area.Y = 0;

  if (area.Width + area.X > bitmap->Width())
    area.Width = bitmap->Width() - area.X;

  if (area.Height + area.Y > bitmap->Height())
    area.Height = bitmap->Height() - area.Y;

  if (area.Width + target.X > Width())
    area.Width = Width() - target.X;

  if (area.Height + target.Y > Height())
    area.Height = Height() - target.Y;

  m_dirty = true;

  for (unsigned line = 0; line < area.Height; line++)
  {
    BYTE* pixelIdx = m_textureData + target.X * 4 + (target.Y + line) * 4 * Width();
    BYTE* pixelSrc = bitmap->m_textureData + area.X * 4 + (area.Y + line) * 4 * bitmap->Width();
    memcpy(pixelIdx, pixelSrc, area.Width * 4);
  }
}

void Bitmap::Blur(unsigned size)
{
  if (!size)
    return;

  m_dirty = true;

  BlurH(size);
  BlurH(size);
  BlurV(size);
  BlurV(size);
}

void Bitmap::FastBlur(unsigned size)
{
  if (!size)
    return;

  m_dirty = true;

  BlurH(size);
  BlurV(size);
}

void Bitmap::Greyscale()
{
  m_dirty = true;

  unsigned totalPixels = Width() * Height();
  BYTE* pixelReader = m_textureData;
  for (unsigned i = 0; i < totalPixels; i++)
  {
    BYTE value = (BYTE)(0.299f * pixelReader[0] + 0.587f * pixelReader[1] + 0.114f * pixelReader[2]);
    pixelReader[0] = value;
    pixelReader[1] = value;
    pixelReader[2] = value;
    pixelReader += 4;
  }
}

void Bitmap::HueRotate(float degrees)
{

  float Q1 = (float)(sin(-degrees * D3DX_PI / 180.0f) / sqrt(3));
  float Q2 = (float)((1 - cos(-degrees * D3DX_PI / 180.0f)) / 3.0f);

  m_dirty = true;

  unsigned totalPixels = Width() * Height();
  BYTE* pixelReader = m_textureData;
  for (unsigned i = 0; i < totalPixels; i++)
  {
    float r = pixelReader[0];
    float g = pixelReader[1];
    float b = pixelReader[2];
    float max = r > b ? g > r ? g : r : g > b ? g : b;
    float rb = r - b;
    float gr = g - r;
    float bg = b - g;
    float r1 = Q2 *(gr - rb) - Q1*bg + r;
    float Z =  Q2 * (bg - rb) + Q1*gr;
    g += Z + (r - r1);
    b -= Z;
    r = r1;
    float adjust = max / (r > b ? g > r ? g : r : g > b ? g : b);
    pixelReader[0] = (BYTE)(max(min(r * adjust, 255), 0));
    pixelReader[1] = (BYTE)(max(min(g * adjust, 255), 0));
    pixelReader[2] = (BYTE)(max(min(b * adjust, 255), 0));

    pixelReader += 4;
  }
}

void Bitmap::BlurH(unsigned size)
{    
  int* rArray = new int[size * 2 + 1];
  int* gArray = new int[size * 2 + 1];
  int* bArray = new int[size * 2 + 1];
  int* aArray = new int[size * 2 + 1];
  float items = (float)size * 2 + 1;
  for (unsigned line = 0; line < Height(); line++)
  {
    BYTE* pixelReader = m_textureData + line * 4 * Width();
    int index = 0;

    int rAcum = 0;
    int bAcum = 0;
    int gAcum = 0;
    int aAcum = 0;

    for (unsigned i = 0; i < size; i++)
    {
      rArray[i] = pixelReader[0] * pixelReader[0];
      gArray[i] = pixelReader[1] * pixelReader[1];
      bArray[i] = pixelReader[2] * pixelReader[2];
      aArray[i] = pixelReader[3] * pixelReader[3];
      rAcum += pixelReader[0] * pixelReader[0];
      gAcum += pixelReader[1] * pixelReader[1];
      bAcum += pixelReader[2] * pixelReader[2];
      aAcum += pixelReader[3] * pixelReader[3];
    }

    for (unsigned i = size; i < size * 2 + 1; i++)
    {
      rArray[i] = pixelReader[0] * pixelReader[0];
      gArray[i] = pixelReader[1] * pixelReader[1];
      bArray[i] = pixelReader[2] * pixelReader[2];
      aArray[i] = pixelReader[3] * pixelReader[3];
      rAcum += pixelReader[0] * pixelReader[0];
      gAcum += pixelReader[1] * pixelReader[1];
      bAcum += pixelReader[2] * pixelReader[2];
      aAcum += pixelReader[3] * pixelReader[3];
      pixelReader += 4;
    }

    BYTE* pixelIdx = m_textureData + line * 4 * Width();
    for (unsigned pixel = 0; pixel < Width(); pixel++)
    {
      index = pixel % (size * 2 + 1);
      pixelIdx[0] = (BYTE)(round(sqrt(rAcum / items)));
      rAcum -= rArray[index];
      rArray[index] = pixelReader[0] * pixelReader[0];
      rAcum += rArray[index];

      pixelIdx[1] = (BYTE)(round(sqrt(gAcum / items)));
      gAcum -= gArray[index];
      gArray[index] = pixelReader[1] * pixelReader[1];
      gAcum += gArray[index];

      pixelIdx[2] = (BYTE)(round(sqrt(bAcum / items)));
      bAcum -= bArray[index];
      bArray[index] = pixelReader[2] * pixelReader[2];
      bAcum += bArray[index];

      pixelIdx[3] = (BYTE)(round(sqrt(aAcum / items)));
      aAcum -= aArray[index];
      aArray[index] = pixelReader[3] * pixelReader[3];
      aAcum += aArray[index];
      
      pixelIdx += 4;
      if (pixel < Width() - size - 2)
        pixelReader += 4;
    }
  }
  delete[] rArray;
  delete[] gArray;
  delete[] bArray;
  delete[] aArray;
}

void Bitmap::BlurV(unsigned size)
{
  int* rArray = new int[size * 2 + 1];
  int* gArray = new int[size * 2 + 1];
  int* bArray = new int[size * 2 + 1];
  int* aArray = new int[size * 2 + 1];
  float items = (float)size * 2 + 1;
  for (unsigned pixel = 0; pixel < Width(); pixel++)
  {
    BYTE* pixelReader = m_textureData + 4 * pixel;
    int index = 0;

    int rAcum = 0;
    int bAcum = 0;
    int gAcum = 0;
    int aAcum = 0;

    for (unsigned i = 0; i < size; i++)
    {
      rArray[i] = pixelReader[0] * pixelReader[0];
      gArray[i] = pixelReader[1] * pixelReader[1];
      bArray[i] = pixelReader[2] * pixelReader[2];
      aArray[i] = pixelReader[3] * pixelReader[3];
      rAcum += pixelReader[0] * pixelReader[0];
      gAcum += pixelReader[1] * pixelReader[1];
      bAcum += pixelReader[2] * pixelReader[2];
      aAcum += pixelReader[3] * pixelReader[3];
    }

    for (unsigned i = size; i < size * 2 + 1; i++)
    {
      rArray[i] = pixelReader[0] * pixelReader[0];
      gArray[i] = pixelReader[1] * pixelReader[1];
      bArray[i] = pixelReader[2] * pixelReader[2];
      aArray[i] = pixelReader[3] * pixelReader[3];
      rAcum += pixelReader[0] * pixelReader[0];
      gAcum += pixelReader[1] * pixelReader[1];
      bAcum += pixelReader[2] * pixelReader[2];
      aAcum += pixelReader[3] * pixelReader[3];
      pixelReader += 4 * Width();
    }

    BYTE* pixelIdx = m_textureData + 4 * pixel;
    for (unsigned line = 0; line < Height(); line++)
    {
      index = line % (size * 2 + 1);
      pixelIdx[0] = (BYTE)(round(sqrt(rAcum / items)));
      rAcum -= rArray[index];
      rArray[index] = pixelReader[0] * pixelReader[0];
      rAcum += rArray[index];

      pixelIdx[1] = (BYTE)(round(sqrt(gAcum / items)));
      gAcum -= gArray[index];
      gArray[index] = pixelReader[1] * pixelReader[1];
      gAcum += gArray[index];

      pixelIdx[2] = (BYTE)(round(sqrt(bAcum / items)));
      bAcum -= bArray[index];
      bArray[index] = pixelReader[2] * pixelReader[2];
      bAcum += bArray[index];

      pixelIdx[3] = (BYTE)(round(sqrt(aAcum / items)));
      aAcum -= aArray[index];
      aArray[index] = pixelReader[3] * pixelReader[3];
      aAcum += aArray[index];

      pixelIdx += 4 * Width();
      if (line < Height() - size - 2)
        pixelReader += 4 * Width();
    }
  }
  delete[] rArray;
  delete[] gArray;
  delete[] bArray;
  delete[] aArray;
}

void Bitmap::Update()
{
  if (!m_dirty)
  {
    return;
  }

  m_dirty = false;

  ID3D11DeviceContext* context = EngineHandle->Graphics->DirectX.GetDeviceContext();
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  context->Map(m_texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

  BYTE* buffer = m_textureData;
  BYTE* mappedData = reinterpret_cast<BYTE*>(mappedResource.pData);
  for (UINT i = 0; i < m_height; i++)
  {
    memcpy(mappedData, buffer, m_width * 4);
    mappedData += mappedResource.RowPitch;
    buffer += m_width * 4;
  }

  context->Unmap(m_texture, 0);
}

ID3D11ShaderResourceView* Bitmap::GetTexture()
{
  return m_textureView;
}

unsigned Bitmap::Width()
{
  return m_width;
}

unsigned Bitmap::Height()
{
  return m_height;
}

void Bitmap::InitializeResource(ID3D11Device* device, ID3D11DeviceContext* context)
{
  D3D11_TEXTURE2D_DESC textureDesc;
  HRESULT hResult;
  unsigned int rowPitch;
  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

  m_dirty = true;

  // Setup the description of the texture.
  textureDesc.Height = m_height;
  textureDesc.Width = m_width;
  textureDesc.MipLevels = 1;
  textureDesc.ArraySize = 1;
  textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  textureDesc.SampleDesc.Count = 1;
  textureDesc.SampleDesc.Quality = 0;
  textureDesc.Usage = D3D11_USAGE_DYNAMIC;
  textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  textureDesc.MiscFlags = 0;

  // Create the empty texture.
  hResult = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
  if (FAILED(hResult))
  {
    //return false;
  }

  // Set the row pitch of the targa image data.
  rowPitch = (m_width * 4) * sizeof(unsigned char);

  Update();

  // Setup the shader resource view description.
  srvDesc.Format = textureDesc.Format;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MostDetailedMip = 0;
  srvDesc.Texture2D.MipLevels = -1;

  // Create the shader resource view for the texture.
  hResult = device->CreateShaderResourceView(m_texture, 0, &m_textureView);
  if (FAILED(hResult))
  {
    //return false;
  }
}

void Bitmap::LoadTexture(LPCWSTR filename)
{
  const CachedImage* image = EngineHandle->Cache->GetImage(filename);
  m_textureData = image->GetImage();

  m_width = image->Width();
  m_height = image->Height();
}