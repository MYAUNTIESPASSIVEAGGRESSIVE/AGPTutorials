#include "Debug.h"
#include "Window.h"
#include <iostream>
#include "Renderer.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Texture.h"

void OpenConsole();

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{

	Window _window{ 800, 600 , hInstance, nCmdShow };
	Renderer _renderer{ _window };

	Mesh mesh_cube{ _renderer, "Assets/cube.obj" };
	Mesh mesh_sphere{ _renderer, "Assets/LoafPoly.obj" };
	Mesh mesh_grass{ _renderer, "Assets/grass.obj", true };
	Texture tex_box{ _renderer, "Assets/Box.bmp" };
	Texture tex_grass{ _renderer, "Assets/grass.png", true };

	GameObject go_grass{ "Grass", &mesh_grass, &tex_grass };
	GameObject go1{ "Cube", &mesh_cube, &tex_box };
	GameObject go2{ "Sphere", &mesh_sphere, &tex_box};

	_renderer.RegisterGameObject(&go1);
	_renderer.RegisterGameObject(&go2);
	_renderer.RegisterGameObject(&go_grass); // when rendering transparent/transulcent textures make sure they are drawn last

	_renderer.camera.transform.position = DirectX::XMVectorSetZ(_renderer.camera.transform.position, -10);

	go1.transform.position = DirectX::XMVectorSet(-2, 0, 0, 1);
	go2.transform.position = DirectX::XMVectorSet(2, 0, 0, 1);

	_renderer.pointLights[0] = { DirectX::XMVECTOR{-1, -1, -3}, {0.85f, 0, 0.85f}, 30, false };
	_renderer.pointLights[1] = { DirectX::XMVECTOR{-1, 1, -4}, {0, 0.85f, 0.85f}, 40, true };

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
			// game code here

			auto kbState = DirectX::Keyboard::Get().GetState();

			if (kbState.W) _renderer.camera.transform.Translate({ 0, 0, 0.001f });
			if (kbState.A) _renderer.camera.transform.Translate({ -0.001f, 0, 0 });
			if (kbState.S) _renderer.camera.transform.Translate({ 0, 0, -0.001f });
			if (kbState.D) _renderer.camera.transform.Translate({ 0.001f, 0, 0 });
			if (kbState.Q) _renderer.camera.transform.Translate({ 0, 0.001f, 0 });
			if (kbState.E) _renderer.camera.transform.Translate({ 0, -0.001f, 0 });

			auto msState = DirectX::Mouse::Get().GetState();
			_renderer.camera.transform.Rotate({ -(float)msState.y * 0.001f, (float)msState.x * 0.001f, 0 });

			if (msState.leftButton) _renderer.camera.transform.position = { 0, 0, -5 };
			if (msState.rightButton) _renderer.camera.transform.position = { 0, 0, 5 };

			_window.HandleInput(_renderer.camera);

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