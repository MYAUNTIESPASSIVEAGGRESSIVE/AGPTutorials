#pragma once
#include "Transform.h"
#include "Camera.h"
#include <vector>

#define MAX_POINT_LIGHTS 8

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

struct ID3D11RasterizerState;

struct ID3D11BlendState;

struct ID3D11DepthStencilState;

struct PointLight
{
	DirectX::XMVECTOR position{ 0,0,0 };
	DirectX::XMVECTOR colour{ 1,1,1 };

	float strength = 10;
	bool enabled = false;
	float padding[2];
};


class Window;

//class Mesh;
class GameObject;

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

	DirectX::XMVECTOR ambientLightColour{ 0.1f, 0.1f, 0.1f};
	DirectX::XMVECTOR directionalLightColour{ 0.9f, 0.8f, 0.75f };
	DirectX::XMVECTOR directionalLightShinesFrom{ 0.9f, 0.8f, 0.75f };
	//DirectX::XMVECTOR pointLightPosition{ 0, 1.0f, -1 };
	//DirectX::XMVECTOR pointLightColour{ 0.85f, 0, 0.85f, 1 };
	//float pointLightStrength = 15;
	PointLight pointLights[MAX_POINT_LIGHTS];

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

	ID3D11RasterizerState* rasterizerCullBack = nullptr;
	ID3D11RasterizerState* rasterizerCullNone = nullptr;

	ID3D11BlendState* blendOpaque = nullptr;
	ID3D11BlendState* blendTransparent = nullptr;

	ID3D11DepthStencilState* depthWriteOff = nullptr;


	long InitD3D();
	long InitPipeline();
	void InitGraphics();
	long InitDepthBuffer();
};

