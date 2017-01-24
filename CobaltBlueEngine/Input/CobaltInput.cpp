#include "CobaltInput.h"

CobaltInput::CobaltInput()
{
  for (int i = 0; i < 256; i++)
  {
    m_lastKeys[i] = m_keys[i] = false;
  }
}

bool CobaltInput::Pressed(Inputs key)
{
  return m_keys[(int)key];
}

bool CobaltInput::Triggered(Inputs key)
{
  return m_keys[(int)key] && !m_lastKeys[(int)key];
}

bool CobaltInput::Released(Inputs key)
{
  return !m_keys[(int)key] && m_lastKeys[(int)key];
}

void CobaltInput::KeyUp(Inputs key)
{
  KeyUp((int)key);
}

Point CobaltInput::MousePos()
{
  return m_mousePoint;
}

void CobaltInput::MousePos(unsigned x, unsigned y)
{
  m_mousePoint.X = x;
  m_mousePoint.Y = y;
}

bool CobaltInput::AnyKey()
{
  for (int i = 0; i < 256; i++)
  {
    if (m_keys[i] && !m_lastKeys[i])
    {
      return true;
    }
  }

  return false;
}

void CobaltInput::Frame()
{
  for (int i = 0; i < 256; i++)
  {
    m_lastKeys[i] = m_keys[i];
  }
}

void CobaltInput::KeyDown(unsigned int key)
{
  m_keys[key] = true;
}

void CobaltInput::KeyUp(unsigned int key)
{
  m_keys[key] = false;
}
