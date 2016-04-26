#include "CobaltBlue.h" 

#pragma comment(lib, "CobaltBlueEngine.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#include <experimental/filesystem>

class AppScene : public CobaltScene {
public:
  void Start(CobaltEngine* engine)
  {
    spr.Create(L"Textures\\stone.png");
    spr.Bitmap()->Greyscale();

    baseline.Create(L"Textures\\blurTest.png");
    baseline.X = 700;

    // Make sure blur is fast and lossless.
    for (int i = 0; i < 10; i++)
      baseline.Bitmap()->FastBlur(1);

  }

  void Update(CobaltEngine* engine)
  {
    if (engine->Input->Pressed(Inputs::Esc))
    {
      engine->Exit();
      return;
    }

    if (engine->Input->Pressed(Inputs::Space))
      engine->GotoScene(new AppScene);

    if (engine->Input->Pressed(Inputs::A))
      spr.Bitmap()->Blur(7);

    if (engine->Input->Pressed(Inputs::S))
      spr.Bitmap()->FastBlur(7);
  }

  void Terminate(CobaltEngine* engine)
  { 
  }

private:
  Sprite spr;
  Sprite baseline;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
  CobaltEngine engine(1366, 768, L"Test App", false, AntiAlias::MSAA8x);
  engine.SetFPS(60);
  engine.Graphics->SetVSync(true);

  engine.Run(new AppScene());

  return 0;
}