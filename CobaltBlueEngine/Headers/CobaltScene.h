#ifndef COBALTBLUESCENE
#define COBALTBLUESCENE

class CobaltScene {
public:
  virtual void Start(CobaltEngine* engine) { return; };
  virtual void Update(CobaltEngine* engine) { return; };
  virtual void Terminate(CobaltEngine* engine) { return; };
};

#endif