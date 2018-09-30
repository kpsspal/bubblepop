#include "common.h"
#include <stdio.h>
#include "myapp.h"
#include <CommCtrl.h>
MyApp MyApp ::m_inst;
HINSTANCE g_hInstance=NULL;
const char g_szClassName[] = "myWindowClass";

#define WINSIZEX 400
#define WINSIZEY 500
#define START_X 20
#define START_Y 20
#define COL1WIDTH 200
#define ROWHEIGHT 30
#define ROWSGAPY 20
#define COL2WIDTH 100
#define COL2GAPX 40


#pragma comment(lib, "ComCtl32.Lib")

#pragma comment(linker,"\"/manifestdependency:type                  = 'win32' \
name                  = 'Microsoft.Windows.Common-Controls' \
version               = '6.0.0.0' \
processorArchitecture = '*' \
publicKeyToken        = '6595b64144ccf1df' \
language              = '*'\"")

#include "inttostring.h"
#include <vector>
std::vector<HWND> g_listwnds;
void CreateSpinCtrl(int indexprop,const char *nameprop,HWND parentwin,int value, int valMin,int valMax)
{
	HWND hwndchild = CreateWindowEx(WS_EX_LEFT | WS_EX_CLIENTEDGE | WS_EX_CONTEXTHELP,
		WC_EDIT,
		nameprop,
		WS_CHILDWINDOW | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_LEFT,
		START_X + COL1WIDTH+COL2GAPX, START_Y + (indexprop*(ROWHEIGHT + ROWSGAPY)), COL2WIDTH,ROWHEIGHT,
		parentwin,
		NULL,
		g_hInstance,
		NULL);

	::ShowWindow(hwndchild,SW_SHOW);
	////::SetWindowText(hwndchild,INTTOSTRING(value).c_str());

	hwndchild = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		UPDOWN_CLASS,
		nameprop,

		WS_CHILDWINDOW | WS_VISIBLE| UDS_AUTOBUDDY | UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_HOTTRACK,
		0,0,0,0,
		parentwin, NULL, g_hInstance, NULL);

    SendMessage(hwndchild, UDM_SETRANGE, 0, MAKELPARAM(valMax, valMin));
	SendMessage(hwndchild, UDM_SETPOS, 0, value);
	::ShowWindow(hwndchild,SW_SHOW);
	g_listwnds.push_back(hwndchild);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CREATE:
		{
#if 1
			HWND hwndchild = NULL;

#define ASSIGN_PROP_INT(__propname,_min1,_max1) CreateSpinCtrl(runindexrow,STRINGIZE(__propname),hwnd,MyApp::get().m_settings.m_##__propname,_min1,_max1);	++runindexrow

			int runindexrow =0;
			ASSIGN_PROP_INT(boardsizex,100,1000);
			ASSIGN_PROP_INT(boardsizey,100,1000);
			ASSIGN_PROP_INT(bubbleradius,5,100);
			ASSIGN_PROP_INT(initbubblescount,0,1000);
			ASSIGN_PROP_INT(cannonanglerangeindegrees,10,90);

			ASSIGN_PROP_INT(ballonseachrowdown,1,20);
			ASSIGN_PROP_INT(scoreeachrowdown,1,20);
			ASSIGN_PROP_INT(scoreeachbubble,1,20);

			hwndchild = CreateWindowEx(\
				WS_EX_CLIENTEDGE,\
				"BUTTON",\
				"Start Game",\
				WS_CHILDWINDOW,\
				START_X+COL1WIDTH+COL2GAPX, WINSIZEY - 80, COL2WIDTH,ROWHEIGHT,\
				hwnd, (HMENU)10000, g_hInstance, NULL);\
				::ShowWindow(hwndchild,SW_SHOW);
#endif
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
		break;
case WM_COMMAND:
	if(LOWORD(wParam)==10000)
    {
		MyApp::get().StartGame();
    }
	return DefWindowProc(hwnd, msg, wParam, lParam);
	break;
case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd,&ps);

#define ASSIGN_PROP_INT(_propname) \
			{\
			/*TextOut(hdc,START_X,START_Y + (runindexrow*(ROWHEIGHT + ROWSGAPY)),STRINGIZE( _propname ),10);*/\
			RECT rect;\
			rect.left = START_X;\
			rect.right = rect.left + COL1WIDTH;\
			rect.top =  START_Y + (runindexrow*(ROWHEIGHT + ROWSGAPY));\
			rect.bottom = rect.top + ROWHEIGHT;\
			DrawText(hdc,STRINGIZE( _propname ),strlen(STRINGIZE( _propname )),&rect,DT_LEFT);\
			++runindexrow;\
			}
			int runindexrow =0;
			ASSIGN_PROP_INT(boardsizex);
			ASSIGN_PROP_INT(boardsizey);
			ASSIGN_PROP_INT(bubbleradius);
			ASSIGN_PROP_INT(initbubblescount);
			ASSIGN_PROP_INT(cannonanglerangeindegrees);

			ASSIGN_PROP_INT(ballonseachrowdown);
			ASSIGN_PROP_INT(scoreeachrowdown);
			ASSIGN_PROP_INT(scoreeachbubble);
			EndPaint(hwnd,&ps);
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void MyApp ::Run(HINSTANCE hInstance,int       nCmdShow)
{

	g_hInstance = hInstance;
	m_settings.LoadSettings();

	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_STANDARD_CLASSES;    // Set the Initialization Flag value.
	InitCommonControlsEx(&icex);          // Initialize the Common Controls Library to use
	// Buttons, Edit Controls, Static Controls, Listboxes,
	// Comboboxes, and  Scroll Bars.


	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	//Registering the Window Class
	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.style         = 0;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return ;
	}

	// Creating the Window
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"Editor for bubblepop app",
		WS_OVERLAPPEDWINDOW & (~WS_MAXIMIZEBOX) & (~WS_SIZEBOX),
		CW_USEDEFAULT, CW_USEDEFAULT, WINSIZEX,WINSIZEY,
		NULL, NULL, hInstance, NULL);

	if(hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return ;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// The Message Loop

	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

}


void MyMessageBox(const char *ptr)
{

}

void MyApp ::StartGame()
{

#define ASSIGN_PROP_INT(__propname) \
	{auto res = SendMessage(g_listwnds[runindexrow], UDM_GETPOS, 0, 0);\
	if( HIWORD( res )  == 0 )\
	{\
	m_settings.m_##__propname = LOWORD( res );\
	}\
	++runindexrow;		}

			int runindexrow =0;
			ASSIGN_PROP_INT(boardsizex);
			ASSIGN_PROP_INT(boardsizey);
			ASSIGN_PROP_INT(bubbleradius);
			ASSIGN_PROP_INT(initbubblescount);
			ASSIGN_PROP_INT(cannonanglerangeindegrees);

			ASSIGN_PROP_INT(ballonseachrowdown);
			ASSIGN_PROP_INT(scoreeachrowdown);
			ASSIGN_PROP_INT(scoreeachbubble);



	if(false == m_settings.SaveSettings() )
	{
		MyMessageBox("Error writing settings");
		return ;
	}

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

	CreateProcess("bubblepop.exe",NULL,NULL,NULL,FALSE,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi);

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles.
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}
