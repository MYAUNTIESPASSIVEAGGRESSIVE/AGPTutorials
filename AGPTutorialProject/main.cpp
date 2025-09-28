#include "Debug.h"
#include "Window.h"
#include <iostream>
#include "Renderer.h"


int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{

	Window _window{ 800, 600 , hInstance, nCmdShow };
	Renderer _renderer{ _window };

	OpenConsole();
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

	_renderer.Release();

	return 0;
}

void OpenConsole()
{
	if (AllocConsole())
	{
		// redirecting input, output and errors to allocated console
		FILE* fp = nullptr;
		freopen_s(&fp, "CONIN$", "r", stdin);
		freopen_s(&fp, "CONOUT$", "w", stdout);
		freopen_s(&fp, "CONOUT$", "w", stderr);
		std::ios::sync_with_stdio(true);

		std::cout << "Hello Console!" << std::endl;
	}
}