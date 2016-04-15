#ifndef COBALTBLUESCENE
#define COBALTBLUESCENE

class CobaltScene {
public:
  virtual ~CobaltScene() { };
  virtual void Start(CobaltEngine* engine) = 0;
  virtual void Update(CobaltEngine* engine) = 0;
  virtual void Terminate(CobaltEngine* engine) = 0;
};

#endif