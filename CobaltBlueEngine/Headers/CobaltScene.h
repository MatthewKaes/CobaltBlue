#ifndef COBALTBLUESCENE
#define COBALTBLUESCENE

#include "CobaltEngine.h"
#define COBALTSCENE CobaltEngine* Engine, CobaltInput* Input, CobaltGraphics* Graphics, CobaltAudio* Audio, CobaltSound* Sound, CobaltCache* Cache

class CobaltScene {
public:
  virtual ~CobaltScene() { };
  virtual void Start(COBALTSCENE) = 0;
  virtual void Update(COBALTSCENE) = 0;
  virtual void Terminate(COBALTSCENE) = 0;
};

#endif