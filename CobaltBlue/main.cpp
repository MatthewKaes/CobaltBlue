#include "CobaltBlue.h" 

#pragma comment(lib, "CobaltBlueEngine.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#include <experimental/filesystem>

class AppScene : public CobaltScene {
public:
  void Start(CobaltEngine* engine)
  {
    spr.Create(L"Textures\\stone.png", engine->Graphics->Width(), engine->Graphics->Height());
    spr.Tint = Color(0, 255, 255);
  }

  void Update(CobaltEngine* engine)
  {
    spr.Ox += 2;
    spr.Oy += 2;

    if (fade)
    {
      spr.Tint.Red += 1;
      spr.Tint.Blue -= 1;
      if (spr.Tint.Blue == 0)
      {
        fade = false;
      }
    }
    else
    {
      spr.Tint.Blue += 1;
      spr.Tint.Red -= 1;
      if (spr.Tint.Red == 0)
      {
        fade = true;
      }
    }

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
  bool fade = true;
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