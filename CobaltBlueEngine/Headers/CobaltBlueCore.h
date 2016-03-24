#ifndef COBALTBLUE
#define COBALTBLUE

// Setup API Exports
#ifdef COBALTENGINEAPI
#define COBALTAPI __declspec( dllexport )
#else
#define COBALTAPI
#endif

// Windows
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


#endif