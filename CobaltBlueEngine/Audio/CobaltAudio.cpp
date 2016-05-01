// Need to get around Microsoft #pragma deprecated nonsense
// so this needs to be included first.
#include <experimental/filesystem>

#include "CobaltAudio.h"
#include "Dshow.h"

CobaltAudio::CobaltAudio(HWND window)
{
  m_graphBuilder = nullptr;
  m_mediaController = nullptr;
  m_audioController = nullptr;
  m_mediaSeeker = nullptr;
  m_eventController = nullptr;
  m_duration = 0.0f;
  m_loop = true;
  m_volume = 1.0f;
  m_state = AudioState::Standby;
  m_window = window;
}

CobaltAudio::~CobaltAudio()
{
  Stop();
}

void CobaltAudio::Play(LPCWSTR filename)
{
  if (!std::experimental::filesystem::exists(filename))
  {
    std::wstring message(L"Failed to load audio file: '");
    message.append(filename);
    message.append(L"'\nFile could not be found. Make sure file is on disk.");
    MessageBox(m_window, message.c_str(), L"Audio Missing", MB_OK);
    ExitProcess(-1);
  }

  Stop();
  if (SUCCEEDED(CoCreateInstance(CLSID_FilterGraph,
    NULL,
    CLSCTX_INPROC_SERVER,
    IID_IGraphBuilder,
    (void **)&this->m_graphBuilder)))
  {
    m_graphBuilder->QueryInterface(IID_IMediaControl, (void **)&m_mediaController);
    m_graphBuilder->QueryInterface(IID_IMediaEventEx, (void **)&m_eventController);
    m_graphBuilder->QueryInterface(IID_IBasicAudio, (void**)&m_audioController);
    m_graphBuilder->QueryInterface(IID_IMediaSeeking, (void**)&m_mediaSeeker);

    HRESULT hr = m_graphBuilder->RenderFile(filename, NULL);
    if (SUCCEEDED(hr))
    {
      m_state = AudioState::Paused;
      if (m_mediaSeeker)
      {
        LONGLONG duration;
        m_mediaSeeker->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);
        // returns 10,000,000 / second.
        m_mediaSeeker->GetDuration(&duration);
        m_duration = duration / 10000000.0f;
      }

      if (m_eventController)
      {
        m_eventController->SetNotifyWindow((OAHWND)m_window, COBALT_AUDIO, NULL);
      }
      else
      {
        m_state = AudioState::Failed;
      }
    }
    else
    {
      m_state = AudioState::Failed;
    }
  }

  // Start up the audio stream.
  Play();
}

void CobaltAudio::Play()
{
  if (m_mediaController)
  {
    if (m_state == AudioState::Paused || m_state == AudioState::Finished)
    {
      if (m_state == AudioState::Finished)
      {
        SetPosition(0.0f);
      }

      HRESULT hr = m_mediaController->Run();
      if (SUCCEEDED(hr))
      {
        m_state = AudioState::Playing;
      }
      else
      {
        m_state = AudioState::Failed;
      }
    }
  }
}

void CobaltAudio::Pause()
{
  if (m_state == AudioState::Playing && m_mediaController)
  {
    HRESULT hr = m_mediaController->Pause();
    if (SUCCEEDED(hr))
    {
      m_state = AudioState::Paused;
    }
    else
    {
      m_state = AudioState::Failed;
    }
  }
}

void CobaltAudio::Stop()
{
  Cleanup();
}

void CobaltAudio::Fade(float fadeTime)
{
  m_fadeOut = m_volume / fadeTime;
}

void CobaltAudio::Frame(float duration)
{
  if (m_fadeOut > 0)
  {
    m_volume -= duration * m_fadeOut;

    if (m_volume < 0)
    {
      m_volume = 0.0f;
      m_fadeOut = 0.0f;
    }

    SetVolume(m_volume);
  }
}

float CobaltAudio::Volume()
{
  if (m_state == AudioState::Failed || m_state == AudioState::Standby)
  {
    return -1.0f;
  }

  return m_volume;
}

float CobaltAudio::Duration()
{
  if (m_state == AudioState::Failed || m_state == AudioState::Standby)
  {
    return -1.0f;
  }

  return m_duration;
}

float CobaltAudio::Position()
{
  if (m_state == AudioState::Failed || 
     m_state == AudioState::Standby ||
     !m_mediaSeeker)
  {
    return -1.0f;
  }

  __int64 curPos = -1;
  HRESULT hr = m_mediaSeeker->GetCurrentPosition(&curPos);
  if (SUCCEEDED(hr))
  {
    return curPos / 10000000.0f;
  }

  return -1;
}

AudioState CobaltAudio::Status()
{
  return m_state;
}

void CobaltAudio::SetVolume(float vol)
{
  if (vol > 1.0f || vol < 0.0f)
    return;

  if (m_state == AudioState::Failed ||
      m_state == AudioState::Standby ||
      !m_audioController)
  {
    return;
  }

  m_volume = vol;
  m_audioController->put_Volume((long)(-10000 * (1.0f - m_volume)));
}

void CobaltAudio::SetPosition(float seconds)
{
  if (m_state == AudioState::Failed ||
    m_state == AudioState::Standby ||
    !m_mediaSeeker)
  {
    return;
  }

  LONGLONG targetPos = (LONGLONG)(10000000 * seconds);
  HRESULT hr = m_mediaSeeker->SetPositions(&targetPos, AM_SEEKING_AbsolutePositioning, nullptr, AM_SEEKING_NoPositioning);
  if (FAILED(hr))
    return;
}

void CobaltAudio::Seek(float seconds)
{
  if (m_state == AudioState::Failed ||
    m_state == AudioState::Standby ||
    !m_mediaSeeker)
  {
    return;
  }

  LONGLONG targetPos = (LONGLONG)(10000000 * seconds);
  HRESULT hr = m_mediaSeeker->SetPositions(&targetPos, AM_SEEKING_RelativePositioning, nullptr, AM_SEEKING_NoPositioning);
  if (FAILED(hr))
    return;
}

void CobaltAudio::SetLoop(bool loopAudio)
{
  m_loop = loopAudio;
}

LRESULT CALLBACK CobaltAudio::EventHandler()
{
  if (!m_eventController)
    return S_OK;

  long evCode;
  LONG_PTR param1;
  LONG_PTR param2;

  while (m_eventController->GetEvent(&evCode, &param1, &param2, 0) == S_OK)
  {
    switch (evCode)
    {
    case EC_COMPLETE:
      if (m_loop)
      {
        SetPosition(0.0f);
        Play();
      }
      else
      {
        m_state = AudioState::Finished;
      }
      break;
    }
    m_eventController->FreeEventParams(evCode, param1, param2);
  }

  return S_OK;
}

void CobaltAudio::Cleanup()
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

  if (m_eventController)
  {
    m_eventController->SetNotifyWindow(NULL, COBALT_AUDIO, NULL);
    m_eventController->Release();
    m_eventController = nullptr;
  }

  m_state = AudioState::Standby;
}