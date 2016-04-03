#ifndef COBALTBLUESYNC
#define COBALTBLUESYNC

#include <chrono>

#define TIMEPOINT std::chrono::time_point<std::chrono::system_clock, std::chrono::duration< double > >

class CobaltSynchronizer
{
public:
  CobaltSynchronizer();
  void SetFPS(unsigned FPS);
  void Start();
  void Sync();
  float FrameTime();
private:
  TIMEPOINT m_startTime;
  double m_frameDuration;
  double m_lastFrame;
};

#endif