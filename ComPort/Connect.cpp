#include "connect.h"
// Throw in another cpp
HANDLE connectPort(LPCTSTR port_name, HWND windowHandle) {
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
