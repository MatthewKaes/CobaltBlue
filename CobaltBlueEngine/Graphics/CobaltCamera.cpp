#include "CobaltCamera.h"

CobaltCamera::CobaltCamera()
{
  m_posX = 0;
  m_posY = 0;
  m_posZ = 0;
  m_rotX = 0;
  m_rotY = 0;
  m_rotZ = 0;
}

void CobaltCamera::SetPosition(float x, float y, float z)
{
  m_posX = x;
  m_posY = y;
  m_posZ = z;
}

void CobaltCamera::SetRotation(float degX, float degY, float degZ)
{
  m_rotX = degX;
  m_rotY = degY;
  m_rotZ = degZ;
}

void CobaltCamera::Move(float x, float y, float z)
{
  m_posX += x;
  m_posY += y;
  m_posZ += z;
}

void CobaltCamera::Rotate(float degX, float degY, float degZ)
{
  m_rotX += degX;
  m_rotY += degY;
  m_rotZ += degZ;
}

void CobaltCamera::Update()
{
  XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
  XMVECTOR position = DirectX::XMVectorSet(m_posX, m_posY, m_posZ, 0.0f);
  XMVECTOR lookAt = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
  float yaw, pitch, roll;
  XMMATRIX rotationMatrix;

  // Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
  pitch = m_rotX * 0.0174532925f;
  yaw = m_rotY * 0.0174532925f;
  roll = m_rotZ * 0.0174532925f;

  // Create the rotation matrix from the yaw, pitch, and roll values.
  rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

  // Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
  lookAt = DirectX::XMVector3TransformCoord(lookAt, rotationMatrix);
  up = DirectX::XMVector3TransformCoord(up, rotationMatrix);

  // Translate the rotated camera position to the location of the viewer.
  lookAt = position + lookAt;

  // Finally create the view matrix from the three updated vectors.
  m_viewMatrix = DirectX::XMMatrixLookAtLH(position, lookAt, up);
}

XMMATRIX CobaltCamera::GetView()
{
  return m_viewMatrix;
}
