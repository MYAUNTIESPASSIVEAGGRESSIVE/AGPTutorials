#pragma once

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;

class Window;

class Renderer
{
public:
	Renderer(Window& inWindow);
	void Release();
	void RenderFrame();

private:
	Window& window;

	IDXGISwapChain* swapchain = nullptr; // pointer to swap chain interface
	ID3D11Device* dev = nullptr; // pointer to Direct3D Device interface
	ID3D11DeviceContext* devcon = nullptr; // pointer to Direct3D Device Context
	ID3D11RenderTargetView* backBuffer = nullptr; // pointer to the D3D render target view

	long InitD3D();
};

