// Command.cpp : Defines the entry point for the application.
//

#define STRICT
#include "command.h"
#include "connect.h"

static TCHAR CmdModName[] = TEXT("Command Module");
static TCHAR CntModName[] = TEXT("Connect Module");
// HANDLE connectPort(LPCTSTR port_name, COMMCONFIG * cc1);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#pragma warning (disable: 4096)

LPCTSTR	lpszCommName1 = TEXT("Com1");
HANDLE hComm1;	// Handle to the Com1 port
using namespace std;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInstance,
	LPSTR lspszCmdParam, int nCmdShow)
{
	HWND cmdhwnd; //Window handler for first window
	HWND connecthwnd;
	MSG Msg;
	WNDCLASSEX Wcl;

	Wcl.cbSize = sizeof(WNDCLASSEX);
	Wcl.style = CS_HREDRAW | CS_VREDRAW;
	Wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION); // large icon 
	Wcl.hIconSm = NULL; // use small version of large icon
	Wcl.hCursor = LoadCursor(NULL, IDC_ARROW);  // cursor style

	Wcl.lpfnWndProc = WndProc;
	Wcl.hInstance = hInst;
	Wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //white background
	Wcl.lpszClassName = CmdModName;

	Wcl.lpszMenuName = TEXT("COMMANDMENU"); // The menu Class
	Wcl.cbClsExtra = 0;      // no extra memory needed
	Wcl.cbWndExtra = 0;

	if (!RegisterClassEx(&Wcl))
		return 0;

	cmdhwnd = CreateWindow(CmdModName, CmdModName, WS_OVERLAPPEDWINDOW, 10, 10,
		600, 400, NULL, NULL, hInst, NULL);
	ShowWindow(cmdhwnd, nCmdShow);	//Show the first window - our connect module with nothing
	UpdateWindow(cmdhwnd);

	// Message loop
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return Msg.wParam;
}

// Function that handles threads for this module
DWORD WINAPI connectThreadHandler(LPVOID params)
{
	do
	{
		OutputDebugString("Thread handler is active");
	} while (true);

	return 0;
}
// Create listening thread
//HANDLE createListeningThread(HANDLE port, LPVOID threadParam, LPDWORD threadId) {
//	HANDLE th = CreateThread(NULL, 0, &connectThreadHandler, &threadParam, 0, threadId);
//	if (th) {
//		char procID[10] = {0,0,0,0,0,0,0,0,0,0};
//		sprintf_s(procID, "%ul", threadId);
//		OutputDebugString(procID);
//	}
//	else {
//		OutputDebugString("thread creation error");
//	}
//	return th;
//}


// After connected to port, init threads that handle reading from + writing to port
int initListenTh(HANDLE porthnd) {
	DWORD listenThID;
	LPDWORD listenThIDLP = &listenThID;
	bool connected = true;
	// Create listen
	HANDLE listenThHnd = CreateThread(NULL, 0, &connectThreadHandler, &connected, 0, &listenThID);
	return 0;
};


//int disconnectPort(HANDLE port) {
//	CloseHandle(port);
//	return 0;
//}

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message,
	WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_COM1:
			hComm1 = connectPort(lpszCommName1, hwnd);
			break;
		case IDM_DIE:
			// Test msg
			MessageBox(hwnd, TEXT("YOU'VE CALLLED IDM_DIE"), TEXT("IDM_CONNECT"), MB_OK);
			break;
		}
		break;
	case WM_CHAR:		
		// Everything else, send over
		switch (LOWORD(wParam))
		{
		case 65:
			OutputDebugString("A pressed!!!!!"); // Works
		}
		break;
	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE:
			OutputDebugString("ESC pressed");
		}
		break;
	case WM_DESTROY:	// Terminate program
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}