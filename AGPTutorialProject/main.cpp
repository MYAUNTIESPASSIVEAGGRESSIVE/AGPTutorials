#include <Windows.h>
#include <d3d11.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

HINSTANCE g_hInst = NULL; // handle to this instance
HWND g_hWnd = NULL; // handle to the created window
const wchar_t* windowName = L"DirectX Hello World!"; // wide char array

IDXGISwapChain* g_swapchain = NULL; // pointer to swap chain interface
ID3D11Device* g_dev = NULL; // pointer to Direct3D Device interface
ID3D11DeviceContext* g_devcon = NULL; // pointer to Direct3D Device Context

void CleanD3D();

HRESULT InitWindow(HINSTANCE instanceHandle, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
HRESULT InitD3D(HWND hWnd);

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
	if (FAILED(InitWindow(hInstance, nCmdShow)))
	{
		MessageBox(NULL, L"Failed to create window", L"Crititcal Error!", MB_ICONERROR | MB_OK);
	}

	if (FAILED(InitD3D(g_hWnd)))
	{
		MessageBox(NULL, L"Unable to create swapchain and device.", L"Critical Error", MB_ICONERROR | MB_OK);
	}

	// hold window event messages
	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);

			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
			else
			{
				// game code here
			}
		}
	}

	CleanD3D();

	return 0;
}

HRESULT InitWindow(HINSTANCE instanceHandle, int nCmdShow)
{
	g_hInst = instanceHandle; // store our app handle (app mem location)

	// registering window class
	// how windows stores properties for
	WNDCLASSEX wc = {}; // "= {}" sets all values to 0;
	// fill struct with info
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc; // window procedure function, make sure it handles window creation or calls DefWindowProc.
	wc.hInstance = instanceHandle; // give our app's handle
	wc.lpszClassName = L"WindowClass1"; // windows will store our window class under this name
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW; // background colour of the app (not needed for D3D app)#

	//register class with ^^ struct. if fails, if block executes
	if (!RegisterClassEx(&wc)) 
	{
		return E_FAIL; // returns fail code
	}


	// adjust window dimentions so top windows bar is not taking pixes away from app
	RECT wr = { 0,0, SCREEN_WIDTH, SCREEN_HEIGHT};
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	g_hWnd = CreateWindowEx(NULL,
		L"WindowClass1", // name of the window class
		windowName,// title of window
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, // alternative window style allows resizing
		100, // xpos of window
		100, // ypos of window
		wr.right - wr.left, // width of window
		wr.bottom - wr.top, // height of window
		NULL, // no parent window
		NULL, // no menus
		instanceHandle, // application handle
		NULL); // used with nultiple windows

	if (g_hWnd == NULL) return E_FAIL;

	ShowWindow(g_hWnd, nCmdShow);

	return S_OK;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		// this message is sent when the user clsoes the window
	case WM_DESTROY:
	{
		// send a quit message
		PostQuitMessage(0);
		return 0;
	}
	// key being pressed events
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(g_hWnd);
			// destorying the window is not the same as closing the app.
			// Calls WM_DESTORY which leads to PostQuitMessage(0) being called.
			break;
		case 'W':
			// w key pressed
			break;
		}

	default:
		// let windows handle everything else with default handling
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

HRESULT InitD3D(HWND hWnd)
{
	// struct to hold info about swap chain
	DXGI_SWAP_CHAIN_DESC scd = {};

	// fill the swap chain description struct
	scd.BufferCount = 1; // 1 back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32-bit color
	scd.BufferDesc.Width = SCREEN_WIDTH; // set the back buffer width
	scd.BufferDesc.Height = SCREEN_HEIGHT; // set the back buffer height
	scd.BufferDesc.RefreshRate.Numerator = 60; // 60fps
	scd.BufferDesc.RefreshRate.Denominator = 1; // 60/1 = 60 fps
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // intended swapchain use
	scd.OutputWindow = hWnd; // window to use
	scd.SampleDesc.Count = 1; // number of smaples for AA
	scd.Windowed = TRUE; // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // allow full-screen switching

	HRESULT hr;

	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&g_swapchain,
		&g_dev,
		NULL,
		&g_devcon);

	if (FAILED(hr)) return hr;
}

void CleanD3D() 
{
	if (g_swapchain) g_swapchain->Release();
	if (g_dev) g_dev->Release();
	if (g_devcon) g_devcon->Release();
}