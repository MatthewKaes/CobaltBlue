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
  void Move(float x, float y, float z, float seconds);
  void Rotate(float degX, float degY, float degZ, float seconds);

  void Update(float frameTime);
  void GetView(XMMATRIX& viewMatrix);

private:
  float m_moveDur;
  float m_rotDur;
  XMVECTOR m_positionTran;
  XMVECTOR m_rotationTran;

  XMVECTOR m_position;
  XMVECTOR m_rotation;

  XMMATRIX m_viewMatrix;
};
#endif