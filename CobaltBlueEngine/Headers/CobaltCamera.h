#ifndef COBALTBLUECAMERA
#define COBALTBLUECAMERA

#include "CobaltBlueCore.h"
#include "Direct3d.h"

class CobaltCamera 
{
public:
  CobaltCamera();

  void SetPosition(float x, float y, float z);
  void SetRotation(float degX, float degY, float degZ);
  void Move(float x, float y, float z);
  void Rotate(float degX, float degY, float degZ);

  void Update();
  XMMATRIX GetView();

private:
  float m_posX;
  float m_posY;
  float m_posZ;
  float m_rotX;
  float m_rotY;
  float m_rotZ;
  XMMATRIX m_viewMatrix;
};
#endif