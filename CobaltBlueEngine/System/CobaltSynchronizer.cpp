#include "CobaltSynchronizer.h"
#include <thread>

CobaltSynchronizer::CobaltSynchronizer()
{
  m_frameDuration = 1.0f;
  m_lastFrame = 0;
}

void CobaltSynchronizer::SetFPS(unsigned FPS)
{
  m_frameDuration = 1.0f / FPS;
}

void CobaltSynchronizer::Start()
{
  m_startTime = std::chrono::system_clock::now();
}

void CobaltSynchronizer::Sync()
{
  TIMEPOINT endFrame = m_startTime + std::chrono::duration<double>(m_frameDuration);
  std::this_thread::sleep_until(endFrame);
  TIMEPOINT test = std::chrono::system_clock::now();
  TIMEPOINT frameTime = std::chrono::system_clock::now();
  std::chrono::duration<double> frameDur = frameTime - m_startTime;
  m_lastFrame = frameDur.count();
}

float CobaltSynchronizer::FrameTime()
{
  return (float)m_lastFrame;
}