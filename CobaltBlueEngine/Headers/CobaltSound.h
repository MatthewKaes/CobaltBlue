#ifndef COBALTBLUESOUND
#define COBALTBLUESOUND

#include "CobaltBlueCore.h"

#include <mmsystem.h>
#include <strmif.h>
#include <control.h>
#include <unordered_map>

#pragma comment(lib, "strmiids.lib")

class CobaltSound {
public:
  CobaltSound(HWND window);
  ~CobaltSound();

  void Play(LPCWSTR filename, float volume);

private:
  class SoundEffect {
  public:
    SoundEffect();
    ~SoundEffect();

    IGraphBuilder*  m_graphBuilder;
    IMediaControl*  m_mediaController;
    IBasicAudio* m_audioController;
    IMediaSeeking* m_mediaSeeker;
  };

  void LoadSound(LPCWSTR filename);
  void SetVolume(float vol, IBasicAudio* audioController);
  void StartPosition(IMediaSeeking* mediaSeeker);

  std::unordered_map<LPCWSTR, SoundEffect*> m_audioGraphs;
  HWND m_window;
};

#endif