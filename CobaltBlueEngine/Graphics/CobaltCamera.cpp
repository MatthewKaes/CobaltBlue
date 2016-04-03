#include "CobaltCamera.h"
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>

CobaltCamera::CobaltCamera()
{
  m_position = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
  m_rotation = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

void CobaltCamera::SetPosition(float x, float y, float z)
{
  m_position = XMVectorSet(x, y, z, 0.0f);
}

void CobaltCamera::SetRotation(float degX, float degY, float degZ)
{
  m_rotation = XMVectorSet(degX, degY, degZ, 0.0f);
}

void CobaltCamera::Move(float x, float y, float z, float seconds)
{
  m_positionTran = XMVectorSet(x, y, z, 0.0f);
  m_moveDur = seconds;
}

void CobaltCamera::Rotate(float degX, float degY, float degZ, float seconds)
{
  m_rotationTran = XMVectorSet(degX, degY, degZ, 0.0f);
  m_rotDur = seconds;
}

void CobaltCamera::Update(float frameTime)
{
  if (m_moveDur > 0.0f)
  {
    double durationTime = frameTime > m_moveDur ? m_moveDur : frameTime;
    m_position += m_positionTran * (durationTime / m_moveDur);
    m_positionTran -= m_positionTran * (durationTime / m_moveDur);
    m_moveDur -= frameTime;
  }

  if (m_rotDur > 0.0f)
  {
    double durationTime = frameTime > m_rotDur ? m_rotDur : frameTime;
    m_rotation += m_rotationTran * frameTime;
    m_rotDur -= frameTime;
  }

  XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
  XMVECTOR lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
  float yaw, pitch, roll;
  XMMATRIX rotationMatrix;

  // Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
  pitch = m_rotation.m128_f32[0] * 0.0174532925f;
  yaw = m_rotation.m128_f32[1] * 0.0174532925f;
  roll = m_rotation.m128_f32[2] * 0.0174532925f;

  // Create the rotation matrix from the yaw, pitch, and roll values.
  rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

  // Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
  lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
  up = XMVector3TransformCoord(up, rotationMatrix);

  // Translate the rotated camera position to the location of the viewer.
  lookAt = m_position + lookAt;

  // Finally create the view matrix from the three updated vectors.
  m_viewMatrix = XMMatrixLookAtLH(m_position, lookAt, up);
}

void CobaltCamera::GetView(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}
