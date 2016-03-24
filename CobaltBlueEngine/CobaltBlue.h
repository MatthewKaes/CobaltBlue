#ifdef COBALTENGINEAPI
#define COBALTAPI __declspec( dllexport )
#else
#define COBALTAPI __declspec( dllimport )
#endif

extern "C" {
  COBALTAPI void Test();
}