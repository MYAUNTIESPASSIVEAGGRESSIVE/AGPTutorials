#include "Window.h"
#include "Debug.h"

const wchar_t* windowName = L"DirectX Hello World!"; // wide char array

Window::Window(int Hwidth, int Hheight, HINSTANCE Hinstance, int nCmdShow)
	: instance(Hinstance), width(Hwidth), height(Hheight)
{
	// registering window class
	// how windows stores properties for
	WNDCLASSEX wc = {}; // "= {}" sets all values to 0;
	// fill struct with info
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc; // window procedure function, make sure it handles window creation or calls DefWindowProc.
	wc.hInstance = instance; // give our app's handle
	wc.lpszClassName = L"WindowClass1"; // windows will store our window class under this name
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW; // background colour of the app (not needed for D3D app)#

	//register class with ^^ struct. if fails, if block executes
	if (!RegisterClassEx(&wc))
	{
		LOG("failed to register class!");
	}


	// adjust window dimentions so top windows bar is not taking pixes away from app
	RECT wr = { 0,0, width, height };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	handle = CreateWindowEx(NULL,
		L"WindowClass1", // name of the window class
		windowName,// title of window
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, // alternative window style allows resizing
		100, // xpos of window
		100, // ypos of window
		wr.right - wr.left, // width of window
		wr.bottom - wr.top, // height of window
		NULL, // no parent window
		NULL, // no menus
		instance, // application handle
		NULL); // used with nultiple windows

	if (handle != NULL)
	{
		ShowWindow(handle, nCmdShow);
	}
	else
	{
		LOG("failed to create window!");
		DWORD var = GetLastError();
		LOG("LOL");
	}
		
}

LRESULT Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;

		case 'W':
			break;

		}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}