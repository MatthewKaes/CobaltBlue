#include "CobaltInput.h"

CobaltInput::CobaltInput()
{
  for (int i = 0; i < 256; i++)
  {
    m_keys[i] = 0;
  }
}

bool CobaltInput::Pressed(Inputs key)
{
  return m_keys[(int)key];
}

void CobaltInput::KeyDown(unsigned int key)
{
  m_keys[key] = true;
}

void CobaltInput::KeyUp(unsigned int key)
{
  m_keys[key] = false;
}
