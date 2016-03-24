#define EXPORT __declspec( dllexport )

extern "C" {
  EXPORT void Test();
}