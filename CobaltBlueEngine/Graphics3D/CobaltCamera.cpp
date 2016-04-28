#include "CobaltCamera.h"
#include <dxgi.h>
#include <d3dcommon.h>

CobaltCamera::CobaltCamera()
{
  m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
  m_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void CobaltCamera::SetPosition(float x, float y, float z)
{
  m_position = D3DXVECTOR3(x, y, z);
}

void CobaltCamera::SetRotation(float degX, float degY, float degZ)
{
  m_rotation = D3DXVECTOR3(degX, degY, degZ);
}

void CobaltCamera::Move(float x, float y, float z, float seconds)
{
  m_positionTran = D3DXVECTOR3(x, y, z);
  m_moveDur = seconds;
}

void CobaltCamera::Rotate(float degX, float degY, float degZ, float seconds)
{
  m_rotationTran = D3DXVECTOR3(degX, degY, degZ);
  m_rotDur = seconds;
}

void CobaltCamera::Update(float frameTime)
{
  if (m_moveDur > 0.0f)
  {
    float durationTime = frameTime > m_moveDur ? m_moveDur : frameTime;
    m_position += m_positionTran * (durationTime / m_moveDur);
    m_positionTran -= m_positionTran * (durationTime / m_moveDur);
    m_moveDur -= frameTime;
  }

  if (m_rotDur > 0.0f)
  {
    float durationTime = frameTime > m_rotDur ? m_rotDur : frameTime;
    m_rotation += m_rotationTran * (durationTime / m_rotDur);
    m_rotationTran -= m_rotationTran * (durationTime / m_rotDur);
    m_rotDur -= frameTime;
  }

  D3DXVECTOR3 up = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 0.0f);
  D3DXVECTOR3 lookAt = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 0.0f);
  float yaw, pitch, roll;
  D3DXMATRIX rotationMatrix;

  // Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
  pitch = m_rotation.x* 0.0174532925f;
  yaw = m_rotation.y * 0.0174532925f;
  roll = m_rotation.z * 0.0174532925f;

  // Create the rotation matrix from the yaw, pitch, and roll values.
  D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

  // Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
  D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
  D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

  // Translate the rotated camera position to the location of the viewer.
  lookAt = m_position + lookAt;

  // Finally create the view matrix from the three updated vectors.
  D3DXMatrixLookAtLH(&m_viewMatrix, &m_position, &lookAt, &up);
}

void CobaltCamera::GetView(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}
