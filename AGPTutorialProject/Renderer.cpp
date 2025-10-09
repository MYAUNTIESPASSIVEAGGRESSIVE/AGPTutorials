#include "Renderer.h"
#include "Debug.h"
#include "Window.h"
#include <d3d11.h>
#include <d3dcompiler.h>

//#define _XM_NO_INTRINSICS_
//#define XM_NO_ALIGHTNMENT // removes some optimisations
#include <DirectXMath.h>
using namespace DirectX;

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT4 Colour;
};


Renderer::Renderer(Window& inWindow)
	: window(inWindow)
{
	if (InitD3D() != S_OK)
	{
		LOG("Failed to initalise D3D renderer");
		return;
	}

	if (InitPipeline() != S_OK)
	{
		LOG("failed to initalise Pipeline");
		return;
	}
}

long Renderer::InitD3D()
{
	// struct to hold info about swap chain
	DXGI_SWAP_CHAIN_DESC scd = {};

	// fill the swap chain description struct
	scd.BufferCount = 1; // 1 back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32-bit color
	scd.BufferDesc.Width = window.GetWidth(); // set the back buffer width
	scd.BufferDesc.Height = window.GetHeight(); // set the back buffer height
	scd.BufferDesc.RefreshRate.Numerator = 60; // 60fps
	scd.BufferDesc.RefreshRate.Denominator = 1; // 60/1 = 60 fps
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // intended swapchain use
	scd.OutputWindow = window.GetHandle(); // window to use
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
		&swapchain,
		&dev,
		NULL,
		&devcon);

	// get the address of the back buffer
	ID3D11Texture2D* backBufferTexture = nullptr;

	// get the back buffer from the swap chain
	hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
	if (FAILED(hr))
	{
		LOG("failed to create backbuffer texture.");
		return hr;
	}

	hr = dev->CreateRenderTargetView(backBufferTexture, NULL, &backBuffer);

	backBufferTexture->Release();
	if (FAILED(hr))
	{
		LOG("failed to create backbuffer view.");
		return hr;
	}
	
	// set the backbuffer as the current render target
	devcon->OMSetRenderTargets(1, &backBuffer, NULL);

	// define and set viewport struct
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)window.GetWidth();
	viewport.Height = (float)window.GetHeight();
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	devcon->RSSetViewports(1, &viewport);

	if (FAILED(hr))
	{
		LOG("failed to create a renderer");
		return hr; // ABORT SHIP!! YARRRRR
	}

	return S_OK;
}

long Renderer::InitPipeline()
{
	// load and compile the vertex and pixel shaders
	HRESULT result;
	ID3DBlob* VS, *PS, *pErrorBlob;

	result = D3DCompileFromFile(L"VertexShader.hlsl", 0, 0, "main", "vs_4_0", 0, 0, &VS, &pErrorBlob);
	if (FAILED(result))
	{
		LOG(reinterpret_cast<const char*> (pErrorBlob->GetBufferPointer()));
		pErrorBlob->Release();
		return result;
	}

	result = D3DCompileFromFile(L"PixelShader.hlsl", 0, 0, "main", "ps_4_0", 0, 0, &PS, &pErrorBlob);
	if (FAILED(result))
	{
		LOG(reinterpret_cast<const char*> (pErrorBlob->GetBufferPointer()));
		pErrorBlob->Release();
		return result;
	}

	// creates the vertex and pixel shader objects
	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

	// sets those shader objects
	devcon->VSSetShader(pVS, 0, 0);
	devcon->PSSetShader(pPS, 0, 0);


	// creates the input layout description
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOUR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	/*
		ABOUT ABOVE ^^^^^
		Semantic Name – A D3D recognisable name explaining the usage value of the data. See docs for a list of semantics.
		Semantic Index – Used to differentiate multiple of the same semantics.
		Data Format – Type and size of this value. Used by input assembler to read the correct number of bytes.
		Input Slot – An advanced technique allowing for a more complex vertex buffer setup. We won’t use this.
		Byte Offset – How deep into the buffer is this element. We can tell D3D to figure it out itself. Sometimes manual values are required.
		Input Slot Class – Is this data input per vertex or per instance.
		Instance Data Step Rate – Used with instanced draw calls, we won’t use this.
	*/

	result = dev->CreateInputLayout(ied, ARRAYSIZE(ied), VS->GetBufferPointer(), VS->GetBufferSize(), &pIL);
	VS->Release();
	PS->Release();
	if (FAILED(result))
	{
		LOG("failed to create input layout");
		return result;
	}

	devcon->IASetInputLayout(pIL);

	return S_OK;
}

void Renderer::Release()
{
	if (backBuffer) backBuffer->Release();
	if (swapchain) swapchain->Release();
	if (dev) dev->Release();
	if (devcon) devcon->Release();
	if (pVS) pVS->Release();
	if (pPS) pPS->Release();
	if (pIL) pIL ->Release();
}

void Renderer::RenderFrame()
{
	FLOAT bg[4] = { 1.0f, 0.0f, 0.0f, 1.0f };

	devcon->ClearRenderTargetView(backBuffer, bg);

	// could include <DirectXColors.h>
	// then devcon->ClearRenderTarget(backBuffer, DirectX::Colors::DarkSlateGray);
	// can press F12 on Colors/DarkSlateGray to see list
	// using namespace DirectX make it less cumbersome

	// flip the back and front buffers
	swapchain->Present(0, 0);
}