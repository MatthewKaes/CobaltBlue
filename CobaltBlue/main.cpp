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
    spr.Bitmap()->Fill(Rect(100, 100, 100, 100), Color(255, 255, 255, 120));
    spr.Bitmap()->Fill(Rect(125, 125, 50, 50), Color(255, 0, 0));

    spr2.Z = 1;
    spr2.Create(400,200);
    spr2.Bitmap()->Gradient(Rect(0, 0, 100, 200), Color(0, 255, 0), Color(0, 0, 255));
    spr2.Bitmap()->Gradient(Rect(100, 0, 200, 100), Color(0, 255, 0), Color(0, 0, 255), true);
  }

  void Update(CobaltEngine* engine)
  {
    if (engine->Input->Pressed(Inputs::Esc))
    {
      engine->Exit();
      return;
    }

    if (engine->Input->Triggered(Inputs::K))
    {
      spr2.Z *= -1;
    }
    else if (engine->Input->Released(Inputs::K))
    {
      spr2.Z *= -1;
    }

    if (engine->Input->Pressed(Inputs::Space))
      engine->GotoScene(new AppScene);

    if (engine->Input->Pressed(Inputs::Left))
      spr.X -= 4;

    if (engine->Input->Pressed(Inputs::Right))
      spr.X += 4;

    if (engine->Input->Pressed(Inputs::Down))
      spr.Y += 4;

    if (engine->Input->Pressed(Inputs::Up))
      spr.Y -= 4;
  }

  void Terminate(CobaltEngine* engine)
  { 
  }

private:
  Sprite spr;
  Sprite spr2;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
  CobaltEngine engine(1366, 768, L"Test App", false, AntiAlias::MSAA8x);
  engine.SetFPS(60);
  engine.Graphics->SetVSync(true);

  engine.Run(new AppScene());

  return 0;
}