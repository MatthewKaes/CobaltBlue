#ifndef COBALTBLUEAUDIO
#define COBALTBLUEAUDIO

#define COBALT_AUDIO WM_APP + 1   // Private message.

#include "CobaltBlueCore.h"

#include <mmsystem.h>
#include <strmif.h>
#include <control.h>

#pragma comment(lib, "strmiids.lib")

enum class AudioState {
  Standby,
  Playing,
  Paused,
  Finished,
  Failed
};

class CobaltAudio
{
public:
  CobaltAudio(HWND window);
  ~CobaltAudio();

  void Play(LPCWSTR filename);
  void Play();
  void Pause();
  void Stop();

  float Volume();
  float Duration();
  float Position();
  AudioState Status();

  void SetVolume(float vol);
  void SetPosition(float seconds);
  void Seek(float seconds);
  void SetLoop(bool loopAudio);

  LRESULT CALLBACK EventHandler();

private:
  void Cleanup();

  IGraphBuilder*  m_graphBuilder;
  IMediaControl*  m_mediaController;
  IBasicAudio* m_audioController;
  IMediaSeeking * m_mediaSeeker;
  IMediaEventEx *  m_eventController;
  HWND m_window;
  float m_volume;
  float m_duration;
  bool m_loop;
  AudioState m_state;
};

#endif