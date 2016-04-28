#ifndef COBALTBLUECAMERA
#define COBALTBLUECAMERA

#include "CobaltBlueCore.h"

#include <dxgi.h>
#include <d3dcommon.h>
#include <D3DX11.h>
#include <D3DX10math.h>

class CobaltCamera 
{
public:
  CobaltCamera();

  void SetPosition(float x, float y, float z);
  void SetRotation(float degX, float degY, float degZ);
  void Move(float x, float y, float z, float seconds);
  void Rotate(float degX, float degY, float degZ, float seconds);

  void Update(float frameTime);
  void GetView(D3DXMATRIX& viewMatrix);

private:
  float m_moveDur;
  float m_rotDur;
  D3DXVECTOR3 m_positionTran;
  D3DXVECTOR3 m_rotationTran;

  D3DXVECTOR3 m_position;
  D3DXVECTOR3 m_rotation;

  D3DXMATRIX m_viewMatrix;
};
#endif