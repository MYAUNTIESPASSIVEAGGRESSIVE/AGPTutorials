#pragma once
#include "Transform.h"
#include "Camera.h"
#include <vector>

// renderer + swap chain structs
struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;

// shader structs
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

// draw buffer
struct ID3D11Buffer;

// depth buffer
struct ID3D11DepthStencilView;

class Window;

//class Mesh;
class GameObject;

class Texture;

class Renderer
{
public:
	Renderer(Window& inWindow);
	void Release();
	void RenderFrame();
	ID3D11Device* GetDevice() { return dev; }
	ID3D11DeviceContext* GetDeviceContext() { return devcon; }
	void RegisterGameObject(GameObject* go);
	void RemoveGameObject(GameObject* go);

	std::vector<GameObject*> gameObjects;

	Camera camera;

	Texture* texture;

private:
	Window& window;

	IDXGISwapChain* swapchain = nullptr; // pointer to swap chain interface
	ID3D11Device* dev = nullptr; // pointer to Direct3D Device interface
	ID3D11DeviceContext* devcon = nullptr; // pointer to Direct3D Device Context
	ID3D11RenderTargetView* backBuffer = nullptr; // pointer to the D3D render target view

	ID3D11VertexShader* pVS = nullptr; // vertex shader
	ID3D11PixelShader* pPS = nullptr; // pixel shader
	ID3D11InputLayout* pIL = nullptr; // Input Layout

	//ID3D11Buffer* vBuffer = nullptr; // vertex buffer
	//ID3D11Buffer* iBuffer = nullptr; // index buffer
	ID3D11Buffer* cBuffer_PerObject = nullptr; // const buffer

	ID3D11DepthStencilView* depthBuffer = NULL; // depth buffer ptr

	long InitD3D();
	long InitPipeline();
	void InitGraphics();
	long InitDepthBuffer();
};

