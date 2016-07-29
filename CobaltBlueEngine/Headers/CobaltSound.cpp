// Need to get around Microsoft #pragma deprecated nonsense
// so this needs to be included first.
#include <experimental/filesystem>

#include "CobaltSound.h"
#include "Dshow.h"


CobaltSound::CobaltSound(HWND window)
{
  m_window = window;
}

CobaltSound::~CobaltSound()
{
  for (auto sound : m_audioGraphs)
  {
    delete sound.second;
  }
}

void CobaltSound::Play(LPCWSTR filename, float volume)
{
  LoadSound(filename);

  SoundEffect* SE = m_audioGraphs[filename];
  SetVolume(volume, SE->m_audioController);
  StartPosition(SE->m_mediaSeeker);
  if (SE->m_mediaController->Run() == S_FALSE)
  {
    FILTER_STATE fs;
    HRESULT hr = SE->m_mediaController->GetState(100, (OAFilterState*)&fs);

    if (fs == State_Running)
      Play(filename, volume);
  }
}

void CobaltSound::SetVolume(float vol, IBasicAudio* audioController)
{
  audioController->put_Volume((long)(-10000 * (1.0f - vol)));
}

void CobaltSound::StartPosition(IMediaSeeking* mediaSeeker)
{
  LONGLONG targetPos = 0;
  mediaSeeker->SetPositions(&targetPos, AM_SEEKING_AbsolutePositioning, nullptr, AM_SEEKING_NoPositioning);
}

void CobaltSound::LoadSound(LPCWSTR filename)
{
  if (m_audioGraphs.find(filename) != m_audioGraphs.end())
  {
    return;
  }

  if (!std::experimental::filesystem::exists(filename))
  {
    std::wstring message(L"Failed to load audio file: '");
    message.append(filename);
    message.append(L"'\nFile could not be found. Make sure file is on disk.");
    MessageBox(m_window, message.c_str(), L"Audio Missing", MB_OK);
    ExitProcess(-1);
  }

  m_audioGraphs[filename] = new SoundEffect();
  SoundEffect* SE = m_audioGraphs[filename];

  if (SUCCEEDED(CoCreateInstance(CLSID_FilterGraph,
    NULL,
    CLSCTX_INPROC_SERVER,
    IID_IGraphBuilder,
    (void **)&SE->m_graphBuilder)))
  {
    SE->m_graphBuilder->QueryInterface(IID_IMediaControl, (void **)&SE->m_mediaController);
    SE->m_graphBuilder->QueryInterface(IID_IBasicAudio, (void**)&SE->m_audioController);
    SE->m_graphBuilder->QueryInterface(IID_IMediaSeeking, (void**)&SE->m_mediaSeeker);

    SE->m_graphBuilder->RenderFile(filename, NULL);
  }
}

CobaltSound::SoundEffect::SoundEffect()
{
  m_graphBuilder = nullptr;
  m_mediaController = nullptr;
  m_audioController = nullptr;
  m_mediaSeeker = nullptr;
}

CobaltSound::SoundEffect::~SoundEffect()
{
  if (m_mediaController)
  {
    m_mediaController->Stop();
    m_mediaController->Release();
    m_mediaController = nullptr;
  }

  if (m_graphBuilder)
  {
    m_graphBuilder->Release();
    m_graphBuilder = nullptr;
  }

  if (m_audioController)
  {
    m_audioController->Release();
    m_audioController = nullptr;
  }

  if (m_mediaSeeker)
  {
    m_mediaSeeker->Release();
    m_mediaSeeker = nullptr;
  }
}