#include "session.h"

// Data struct to pass through
struct sessionHandles {
	HANDLE portHnd;
	HWND wndHnd;
};

// Throw in another cpp
HANDLE initPort(LPCTSTR port_name, HWND windowHandle) {
	// Init configs
	COMMCONFIG cc;
	cc.dwSize = sizeof(COMMCONFIG);
	cc.wVersion = 0x100;
	// Create the port
	HANDLE portHandle = CreateFile(port_name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	// BOOL SetupComm(HANDLE hCommDev, DWORD cbInQueue, DWORD cbOutQueue);
	if (portHandle != INVALID_HANDLE_VALUE) {
		// Init port
		if (SetupComm(portHandle, 4, 4)) {
			// Config for the port
			GetCommConfig(portHandle, &cc, &cc.dwSize);
			// Connect the port
			if (CommConfigDialog(port_name, windowHandle, &cc)) {
				// Pressed OK, nothing broke
				// initConnect(portHandle);
				OutputDebugStringA("Setup success");
			}
			else {
				OutputDebugStringA("Setup cancelled");
			}
		}
		else {
			OutputDebugStringA("Initializing serial port failed");
		}
	}
	else {
		OutputDebugStringA("Handle Setup Error");
	}
	return portHandle;
}

// Function that handles threads for this module
DWORD WINAPI listenThreadHandler(LPVOID sh)
{
	char buffer[2];
	DWORD dwBytesWritten;
	DWORD dwBytesRead;
	sessionHandles sHand = *reinterpret_cast<sessionHandles*>(sh);
	HDC hdc = GetDC(sHand.wndHnd);
	do
	{
		ReadFile(sHand.portHnd, &buffer, sizeof(buffer), &dwBytesRead, NULL);
		if (dwBytesRead) {
			TextOut(hdc, 0, 0, buffer, 2);
			OutputDebugString("Reading thread");
		}
	} while (true);
	return 0;
}

// After connected to port, init threads that handle reading from + writing to port
int initListenTh(HANDLE porthnd, HWND wndhnd) {
	DWORD listenThID;
	LPDWORD listenThIDLP = &listenThID;
	// Build data struct to be passed into listenThreadHandler
	sessionHandles sh;
	sh.portHnd = porthnd;
	sh.wndHnd = wndhnd;
	// sessionHandles * shp = &sh;
	// bool connected = true;
	// Create listen
	HANDLE listenThHnd = CreateThread(NULL, 0, &listenThreadHandler, &sh, 0, &listenThID);
	return 0;
};
