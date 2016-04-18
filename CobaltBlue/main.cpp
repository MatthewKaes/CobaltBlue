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
    spr2.Create(L"Textures\\stone.png");
    spr2.Z = 1000.0f;
    spr.Z = 0.1f;
    //spr.Tint = Color(255, 0, 155, 120);
  }

  void Update(CobaltEngine* engine)
  {
    if (engine->Input->Pressed(Inputs::Esc))
    {
      engine->Exit();
      return;
    }

    spr2.Tint.Alpha = spr2.Tint.Red -= 1;
    if (spr2.Tint.Red == 0)
      spr2.Tint.Red = 255;

    spr.Tint = spr2.Tint;

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