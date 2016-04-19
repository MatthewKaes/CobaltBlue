#ifndef COBALTBLUEINPUT
#define COBALTBLUEINPUT

#include "CobaltBlueCore.h"
#include "InputTable.h"

class CobaltInput
{
public:
  // API Friend classes
  friend class CobaltEngine;

  CobaltInput();

  bool Pressed(Inputs key);
  bool Triggered(Inputs key);
  bool Released(Inputs key);
  bool AnyKey();

protected:
  void Frame();
  void KeyDown(unsigned int key);
  void KeyUp(unsigned int key);

private:
  bool m_keys[256];
  bool m_lastKeys[256];
};

#endif