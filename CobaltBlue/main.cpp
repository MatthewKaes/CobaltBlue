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

    if (engine->Input->Pressed(Inputs::K))
      spr2.Z *= -1;

    if (engine->Input->Pressed(Inputs::Left))
      spr.X -= 2;

    if (engine->Input->Pressed(Inputs::Right))
      spr.X += 2;

    if (engine->Input->Pressed(Inputs::Down))
      spr.Y += 2;

    if (engine->Input->Pressed(Inputs::Up))
      spr.Y -= 2;
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
  engine.SetFPS(120);

  engine.Run(new AppScene());

  return 0;
}