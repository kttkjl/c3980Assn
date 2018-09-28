#pragma once
#include <windows.h>

// Connect to a port, returns a HANDLE to the port
HANDLE initPort(LPCTSTR port_name, HWND windowHandle);

// After connected to port, init threads that handle reading from + writing to port
int initListenTh(HANDLE porthnd, HWND hwnd);
