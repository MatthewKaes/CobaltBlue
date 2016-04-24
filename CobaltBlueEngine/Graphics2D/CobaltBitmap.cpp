#include "CobaltEngine.h"
#include "CobaltBitmap.h"

extern CobaltEngine* EngineHandle;

#define KERNELSIZE 4

const Color DefaultColor = Color(255, 255, 255);
const Color DefaultOutline = Color(0, 0, 0, 150);

CobaltBitmap::CobaltBitmap()
{
  m_texture = 0;
  m_textureView = 0;
  m_textColor = DefaultColor;
  m_outlineColor = DefaultOutline;
}

CobaltBitmap::~CobaltBitmap()
{
  Release();
}

void CobaltBitmap::Create(ID3D11Device* device, ID3D11DeviceContext* context, LPCWSTR filename)
{
  m_fontname = EngineHandle->Font;

  LoadTexture(filename);
  InitializeResource(device, context);
}

void CobaltBitmap::Create(ID3D11Device* device, ID3D11DeviceContext* context, unsigned width, unsigned height)
{
  m_fontname = EngineHandle->Font;

  m_textureData = new BYTE[width * height * 4];
  memset(m_textureData, 0, sizeof(BYTE) * width * height * 4);
  m_width = width;
  m_height = height;

  InitializeResource(device, context);
}

void CobaltBitmap::Release()
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
}

void CobaltBitmap::SetTextColor(Color textColor)
{
  m_textColor = textColor;
}

void CobaltBitmap::SetOutlineColor(Color outlineColor)
{
  m_outlineColor = outlineColor;
}

void CobaltBitmap::SetFont(LPWSTR fontname)
{
  m_fontname = fontname;
}

Color CobaltBitmap::Pixel(unsigned x, unsigned y)
{
  if (x > Width() || y > Height())
    return Color();

  BYTE* colorIdx = m_textureData + x * 4 + y * 4 * Width();
  return Color(colorIdx[0], colorIdx[1], colorIdx[2], colorIdx[3]);
}

void CobaltBitmap::SetPixel(unsigned x, unsigned y, Color color)
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

void CobaltBitmap::Fill(Color color)
{
  Fill(Rect(0, 0, Width(), Height()), color);
}

void CobaltBitmap::Fill(Rect area, Color color)
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

  for (unsigned line = 0; line < area.Height; line++)
  {
    BYTE* pixelIdx = m_textureData + area.X * 4 + (area.Y + line) * 4 * Width();
    for (unsigned pixel = 0; pixel < area.Width; pixel++)
    {
      pixelIdx[0] = color.Red;
      pixelIdx[1] = color.Blue;
      pixelIdx[2] = color.Green;
      pixelIdx[3] = color.Alpha;
      pixelIdx += 4;
    }
  }
}

void CobaltBitmap::Gradient(Rect area, Color color1, Color color2)
{
  Gradient(area, color1, color2, false);
}

void CobaltBitmap::Gradient(Rect area, Color color1, Color color2, bool horz)
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

void CobaltBitmap::DrawText(LPCWSTR text, unsigned size, Rect area)
{
  DrawText(text, size, area, false, false);
}

void CobaltBitmap::DrawText(LPCWSTR text, unsigned size, Rect area, bool bold, bool italic)
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
            if (dist < distSqr)
              distSqr = dist;

            shaded = true;
          }
          xdiff++;
        }
        ydiff++;
      }

      if (shaded)
      {
        float mod = 1.0f;
        for(int i = 2; i < distSqr; i += 2)
          mod *= 1.45f;

        float pixelAlpha = m_outlineColor.Alpha / (255.0f * mod);
        pixelIdx[0] = (BYTE)(m_outlineColor.Red * pixelAlpha + (1 - pixelAlpha) * pixelIdx[0]);
        pixelIdx[1] = (BYTE)(m_outlineColor.Green * pixelAlpha + (1 - pixelAlpha) * pixelIdx[1]);
        pixelIdx[2] = (BYTE)(m_outlineColor.Blue * pixelAlpha + (1 - pixelAlpha) * pixelIdx[2]);
        pixelIdx[3] = (BYTE)(min(255 * pixelAlpha + (1 - pixelAlpha) * pixelIdx[3], 255));
      }

      if (*rawQuad)
      {
        float pixelAlpha = (float)m_textColor.Alpha * pixelSrc[0] * pixelSrc[1] * pixelSrc[2] / 4228250625.0f;

        // Text is blury as a result of AA. Add a factor to sharpen text.
        pixelAlpha = min(pixelAlpha * 1.15f, 1.0f);
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
}

void CobaltBitmap::Update(ID3D11DeviceContext* context)
{
  if (!m_dirty)
  {
    return;
  }

  m_dirty = false;

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

ID3D11ShaderResourceView* CobaltBitmap::GetTexture()
{
  return m_textureView;
}

unsigned CobaltBitmap::Width()
{
  return m_width;
}

unsigned CobaltBitmap::Height()
{
  return m_height;
}

void CobaltBitmap::InitializeResource(ID3D11Device* device, ID3D11DeviceContext* context)
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

  Update(context);

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

void CobaltBitmap::LoadTexture(LPCWSTR filename)
{
  const CachedImage* image = EngineHandle->Cache->GetImage(filename);
  m_textureData = image->GetImage();

  m_width = image->Width();
  m_height = image->Height();
}