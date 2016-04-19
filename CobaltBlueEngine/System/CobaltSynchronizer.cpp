#include "CobaltSynchronizer.h"
#include <thread>

CobaltSynchronizer::CobaltSynchronizer()
{
  m_frameDuration = 1.0f;
  m_lastFrame = 0;
  m_frameCount = 0;
}

void CobaltSynchronizer::SetFPS(unsigned FPS)
{
  m_frameDuration = 1.0f / FPS;
  m_fps = FPS;
}

unsigned CobaltSynchronizer::FPS()
{
  return m_fps;
}

unsigned CobaltSynchronizer::FrameCount()
{
  return m_frameCount;
}

void CobaltSynchronizer::Start()
{
  m_startTime = std::chrono::system_clock::now();
}

void CobaltSynchronizer::Sync(bool lockFrame)
{
  if (lockFrame)
  {
    TIMEPOINT endFrame = m_startTime + std::chrono::duration<double>(m_frameDuration);
    std::this_thread::sleep_until(endFrame);
  }

  TIMEPOINT frameTime = std::chrono::system_clock::now();
  std::chrono::duration<double> frameDur = frameTime - m_startTime;
  m_lastFrame = frameDur.count();

  m_frameCount += 1;
}

float CobaltSynchronizer::FrameTime()
{
  return (float)m_lastFrame;
}