#include "common.h"
#include "myapp.h"

#ifdef _MSC_VER
int CALLBACK WinMain(
  _In_ HINSTANCE hInstance,
  _In_ HINSTANCE hPrevInstance,
  _In_ LPSTR     lpCmdLine,
  _In_ int       nCmdShow
)
#else
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR,int nCmdShow)
#endif
{
	MyApp::get().Run(hInstance,nCmdShow);
	return 0;
}
