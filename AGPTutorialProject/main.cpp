#include "Debug.h"
#include "Window.h"
#include <iostream>
#include "Renderer.h"

void OpenConsole();

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
		}
		else
		{
			_renderer.transform.position = DirectX::XMVectorSetZ(_renderer.transform.position, 0.5f);

			static float fakeTime = 0;
			fakeTime += 0.0001f; // simulate time passage
			Transform& t = _renderer.transform; // cache reference to transform (easy access)
			t.position = DirectX::XMVectorSetZ(t.position, 0.75f); // set x so its in front of the cam
			t.position = DirectX::XMVectorSetX(t.position, sin(fakeTime)); // oscillate L+R
			t.Rotate({ 0.0001f, 0.0001f, 0 }); // spin object per frame


			// game code here
			_renderer.RenderFrame();
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