#include "CobaltBlue.h" 

#pragma comment(lib, "CobaltBlueEngine.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#include <experimental/filesystem>

class AppScene : public CobaltScene {
public:
  void Start(CobaltEngine* engine)
  {
    triangle.Create(L"Textures\\stone.png", TextureType::Dynamic);
  }

  void Update(CobaltEngine* engine)
  {
    if (engine->Input->Pressed(Inputs::Esc))
    {
      engine->Exit();
      return;
    }

    if (engine->Input->Pressed(Inputs::Space))
    {
      engine->GotoScene(new AppScene);
    }

    if (engine->Input->Pressed(Inputs::Left))
      engine->Graphics->Camera.Move(-1.0f, 0, 0, 0.2f);

    if (engine->Input->Pressed(Inputs::Right))
      engine->Graphics->Camera.Move(1.0f, 0, 0, 0.2f);

    if (engine->Input->Pressed(Inputs::Down))
      engine->Graphics->Camera.Move(0, -1.0f, 0, 0.2f);

    if (engine->Input->Pressed(Inputs::Up))
      engine->Graphics->Camera.Move(0, 1.0f, 0, 0.2f);
  }

  void Terminate(CobaltEngine* engine)
  { 
  }

private:
  Model3D triangle;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
  CobaltEngine engine(1366, 768, L"Test App", false, AntiAlias::MSAA8x);
  engine.SetFPS(60);

  engine.Run(new AppScene());

  return 0;
}