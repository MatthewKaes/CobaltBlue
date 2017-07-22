#include "CobaltInput.h"

CobaltInput::CobaltInput()
{
  Clear();
}

bool CobaltInput::Pressed(Inputs key)
{
  return m_keys[(int)key];
}

bool CobaltInput::Triggered(Inputs key)
{
  return m_keys[(int)key] && !m_lastKeys[(int)key];
}

int CobaltInput::NumberInput()
{
  for(int i = (int)Inputs::N0; i < (int)Inputs::N9; i++)
    if (Triggered((Inputs)i))
      return i - (int)Inputs::N0;

  return -1;
}

bool CobaltInput::Released(Inputs key)
{
  return !m_keys[(int)key] && m_lastKeys[(int)key];
}

void CobaltInput::KeyUp(Inputs key)
{
  KeyUp((int)key);
}

void CobaltInput::Clear()
{
  for (int i = 0; i < 256; i++)
  {
    m_lastKeys[i] = m_keys[i] = false;
  }
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
