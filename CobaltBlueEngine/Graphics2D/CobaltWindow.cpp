#include "CobaltWindow.h"
#include <unordered_set>

extern std::unordered_set<Model2D*> g_updateListings;

Window::~Window()
{
  g_updateListings.erase(this);
}

void Window::Create(LPCWSTR textureFile)
{
  g_updateListings.insert(this);
  m_background.Create(textureFile);
  m_foreground.Create(m_background.Width(), m_background.Height());
}

unsigned Window::Width()
{
  return m_background.Width();
}

unsigned Window::Height()
{
  return m_background.Height();
}
void Window::Update(float frameTime)
{
  m_background.X = m_foreground.X = X;
  m_background.Y = m_foreground.Y = Y;
  m_background.Z = m_foreground.Z = Z;
  m_background.Tint = m_foreground.Tint = Tint;
  m_background.Zoom = m_foreground.Zoom = Zoom;

  m_background.Update(frameTime);
  m_foreground.Update(frameTime);
}

void Window::Hide(bool hide)
{
  m_background.Hide = hide;
  m_foreground.Hide = hide;
}

Bitmap* Window::Foreground()
{
  return m_foreground.Bitmap();
}

Bitmap* Window::Background()
{
  return m_background.Bitmap();
}