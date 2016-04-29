#include "CobaltAnimation.h"
#include "CobaltEngine.h"

extern std::unordered_set<Model2D*> g_updateListings;

void Animation::Create(LPWSTR textureFile, unsigned cellsX, unsigned cellsY)
{
  g_updateListings.insert(this);
  m_parallax.Create(textureFile, 1, 1);
  m_width = m_parallax.ImgWidth() / cellsX;
  m_height = m_parallax.ImgHeight() / cellsY;

  m_parallax.Width = m_width;
  m_parallax.Height = m_height;
  m_cellCountX = cellsX;
  m_cellCountY = cellsY;
}

void Animation::Pause()
{
  m_frameTime = 0;
}

void Animation::Play(unsigned frameTime)
{
  m_frameTime = frameTime;
  m_frameCount = 0;
}

unsigned Animation::Width()
{
  return m_width;
}

unsigned Animation::Height()
{
  return m_height;
}

void Animation::Update()
{
  m_parallax.X = X;
  m_parallax.Y = Y;

  if (m_frameTime)
  {
    m_frameCount += 1;
    if (m_frameCount % m_frameTime == 0)
    {
      m_frameCount = 0;
      if (++m_cellX % m_cellCountX == 0)
      {
        m_cellX = 0;
        if (YRoll)
        {
          if (++m_cellY % m_cellCountY == 0)
          {
            m_cellY = 0;
          }
        }
      }
      m_parallax.Ox = m_width * m_cellX;
      m_parallax.Oy = m_height * m_cellY;
    }
  }
}

Bitmap* Animation::Bitmap()
{
  return m_parallax.Bitmap();
}