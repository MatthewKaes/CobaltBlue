#ifndef COBALTBLUEINPUT
#define COBALTBLUEINPUT

#include "CobaltBlueCore.h"
#include "InputTable.h"

class COBALTAPI CobaltInput
{
public:
  // API Friend classes
  friend class CobaltEngine;

  CobaltInput();

  bool Pressed(Inputs key);

protected:
  void KeyDown(unsigned int key);
  void KeyUp(unsigned int key);

private:
  bool m_keys[256];
};

#endif