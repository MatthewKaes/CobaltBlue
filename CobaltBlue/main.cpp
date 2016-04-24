#include "CobaltBlue.h" 

#pragma comment(lib, "CobaltBlueEngine.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#include <experimental/filesystem>

class AppScene : public CobaltScene {
public:
  void Start(CobaltEngine* engine)
  {
    blur.Create(L"Textures\\green.png");
    spr.Create(L"Textures\\stone.png");

    // Preform a bunch of blends.
    spr.Bitmap()->Blend(&blur, Rect(0, 0, blur.Width(), blur.Height()), Point(100, 100));
    spr.Bitmap()->Blend(&blur, Rect(0, 0, blur.Width(), blur.Height()), Point(200, 150));
    spr.Bitmap()->Blend(&blur, Rect(0, 0, blur.Width(), blur.Height()), Point(450, 190));
    spr.Bitmap()->Blend(&blur, Rect(50, 50, blur.Width(), blur.Height()), Point(0, 0));
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
      spr.Bitmap()->Blend(&blur, Rect(0, 0, blur.Width(), blur.Height()), Point(rand() % spr.Width(), rand() % spr.Height()));

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
  Bitmap blur;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
  CobaltEngine engine(1366, 768, L"Test App", false, AntiAlias::MSAA8x);
  engine.SetFPS(60);
  engine.Graphics->SetVSync(true);

  engine.Run(new AppScene());

  return 0;
}