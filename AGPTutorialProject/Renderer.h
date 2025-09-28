#pragma once

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;

class Window;

class Renderer
{
public:
	Renderer(Window& inWindow);
	void Release();

private:
	IDXGISwapChain* swapchain = nullptr; // pointer to swap chain interface
	ID3D11Device* dev = nullptr; // pointer to Direct3D Device interface
	ID3D11DeviceContext* devcon = nullptr; // pointer to Direct3D Device Context
	Window& window;

};

