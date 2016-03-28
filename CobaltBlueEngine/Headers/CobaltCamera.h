#ifndef COBALTBLUECAMERA
#define COBALTBLUECAMERA

#include "CobaltBlueCore.h"

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class CobaltCamera 
{
public:
  CobaltCamera();

  void SetPosition(float x, float y, float z);
  void SetRotation(float degX, float degY, float degZ);
  void Move(float x, float y, float z);
  void Rotate(float degX, float degY, float degZ);

  void Update();
  void GetView(XMMATRIX& viewMatrix);

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