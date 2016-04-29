#include "CobaltBlue.h" 

#pragma comment(lib, "CobaltBlueEngine.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#include <experimental/filesystem>

class AppScene : public CobaltScene {
public:
  void Start(CobaltEngine* engine)
  {
    spr.Create(200, 200, 400, 200);
    spr.Bitmap()->Gradient(Rect(0, 0, 200, 200), Color(255, 0, 0), Color(0, 255, 0, 100), true);
    spr.Bitmap()->Gradient(Rect(200, 0, 200, 200), Color(0, 255, 0, 100), Color(255, 0, 0), true);
  }

  void Update(CobaltEngine* engine)
  {
    spr.Ox += 2;

    if (engine->Input->Pressed(Inputs::Esc))
    {
      engine->Exit();
      return;
    }

    if (engine->Input->Pressed(Inputs::Space))
      engine->GotoScene(new AppScene);
  }

  void Terminate(CobaltEngine* engine)
  { 
  }

private:
  Parallax spr;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
  CobaltEngine engine(1366, 768, L"Test App", false, AntiAlias::MSAA8x);
  engine.SetFPS(60);
  engine.Graphics->SetVSync(true);

  engine.Run(new AppScene());

  return 0;
}