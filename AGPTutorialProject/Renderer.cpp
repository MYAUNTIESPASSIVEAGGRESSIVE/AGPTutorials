#include "Renderer.h"
#include "Debug.h"
#include "Window.h"
#include <d3d11.h>
#include "ShaderLoading.h"
#include <DirectXColors.h>
//#define _XM_NO_INTRINSICS_
//#define XM_NO_ALIGHTNMENT // removes some optimisations
#include <DirectXMath.h>
using namespace DirectX;

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT4 Colour;
};

// does not cross 16 byte boundry
struct CBuffer_PerObject
{
	XMMATRIX WVP; // 64 byte world matrix
	// each row is 16 bytes
	// XMMATRIX aligns with SIMD hardware
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

	InitGraphics();
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
	
	// init the depth buffer
	hr = InitDepthBuffer();
	if (FAILED(hr))
	{
		LOG("Failed to create depth buffer");
		return hr;
	}

	// set the backbuffer as the current render target
	devcon->OMSetRenderTargets(1, &backBuffer, depthBuffer);

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
	// loads the pixel and vertex shader
	ShaderLoading::LoadVertexShader("Compiled Shaders/VertexShader.cso", dev, &pVS, &pIL);

	if (FAILED(ShaderLoading::LoadVertexShader("Compiled Shaders/VertexShader.cso", dev, &pVS, &pIL)))
	{
		LOG("Failed to load vertex shader");
		return NULL;
	}

	ShaderLoading::LoadPixelShader("Compiled Shaders/PixelShader.cso", dev, &pPS);

	if (FAILED(ShaderLoading::LoadPixelShader("Compiled Shaders/PixelShader.cso", dev, &pPS)))
	{
		LOG("Failed to load vertex shader");
		return NULL;
	}

	// sets those shader objects
	devcon->VSSetShader(pVS, 0, 0);
	devcon->PSSetShader(pPS, 0, 0);

	devcon->IASetInputLayout(pIL);

	return S_OK;
}

void Renderer::InitGraphics()
{
	// cube vertices
	Vertex vertices[] =
	{
		{ XMFLOAT3{-0.5f, -0.5f, -0.5f}, XMFLOAT4{Colors::Red}    }, // front BL
		{ XMFLOAT3{-0.5f,  0.5f, -0.5f}, XMFLOAT4{Colors::Lime}   }, // front TL
		{ XMFLOAT3{ 0.5f,  0.5f, -0.5f}, XMFLOAT4{Colors::Blue}   }, // front TR
		{ XMFLOAT3{ 0.5f, -0.5f, -0.5f}, XMFLOAT4{Colors::White}  }, // front BR

		{ XMFLOAT3{-0.5f, -0.5f,  0.5f}, XMFLOAT4{Colors::Cyan}   }, // back BL
		{ XMFLOAT3{-0.5f,  0.5f,  0.5f}, XMFLOAT4{Colors::Purple} }, // back TL
		{ XMFLOAT3{ 0.5f,  0.5f,  0.5f}, XMFLOAT4{Colors::Yellow} }, // back TR
		{ XMFLOAT3{ 0.5f, -0.5f,  0.5f}, XMFLOAT4{Colors::Black}  }, // back BR

	};

	/*
	Usage – we can inform D3D about how we intend to use this buffer, specifically if we plan on changing it at runtime. 
	Typically, we would specify this as usage_default which means we do not need to change the contents of the buffer after creation, 
	but dynamic is optimised for CPU changes to the buffer. 
	This allows us to do CPU-level animations or deformations.
	Dynamic also allows us to map and write to the buffer (step 7).
	Byte Width – how big this buffer is and how much data it can store. You can simply use a sizeof(vertices) here if you wish to include the entire mesh, which would be a typical use case. Alternatively, you can specify a custom size if you wish to only include a portion of a model or some other specific use case.
	Bind Flags – telling D3D what data we will store in this buffer. This allows D3D to perform behind-the-scenes optimisations to how it stores and copies data in this buffer.
	CPU Access – we can specify whether we want no CPU access (NULL), CPU-read, CPU-write or both.
	*/

	D3D11_BUFFER_DESC bdesc = { 0 };
	bdesc.Usage = D3D11_USAGE_DYNAMIC; // allows for CPU-write and GPU-read
	//bdesc.ByteWidth = sizeof(Vertex) * 3; // size of buffer - sizeof vertex * num of vertices
	bdesc.ByteWidth = sizeof(vertices); // can use this but only in local scope
	bdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // use as vertex buffer
	bdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // allow CPU to write in buffer
	dev->CreateBuffer(&bdesc, NULL, &vBuffer);

	if (FAILED(vBuffer) || vBuffer == 0);
	{
		LOG("failed to create vertex buffer");
	}

	//copy the verticies into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(vBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); // map the buffer

	memcpy(ms.pData, vertices, sizeof(vertices)); // copy the data into the buffer

	devcon->Unmap(vBuffer, NULL);

	// Create an index buffer
	unsigned int indices[] =
	{   /* front */ 0,1,2,2,3,0, /* back */ 7,6,5,5,4,7, /* left */   4,5,1,1,0,4,
		/* right */ 3,2,6,6,7,3, /* top */  1,5,6,6,2,1, /* bottom */ 4,0,3,3,7,4,};

	// fill in a buffer description
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(indices);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// define the rosource data.
	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.pSysMem = indices;

	if (FAILED(dev->CreateBuffer(&bufferDesc, &initData, &iBuffer)))
	{
		LOG("failed to create index buffer");
	}

	D3D11_BUFFER_DESC cbd = { 0 };
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(CBuffer_PerObject);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	if (FAILED(dev->CreateBuffer(&cbd, NULL, &cBuffer_PerObject)))
	{
		LOG("failed to create Cbuffer");
	}
}

long Renderer::InitDepthBuffer()
{
	HRESULT hr;
	DXGI_SWAP_CHAIN_DESC scd = {};
	swapchain->GetDesc(&scd);

	D3D11_TEXTURE2D_DESC tex2dDesc = { 0 };
	tex2dDesc.Width = window.GetWidth();
	tex2dDesc.Height = window.GetHeight();
	tex2dDesc.ArraySize = 1;
	tex2dDesc.MipLevels = 1;
	tex2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex2dDesc.SampleDesc.Count = scd.SampleDesc.Count; // same sample count as the swap chain
	tex2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex2dDesc.Usage = D3D11_USAGE_DEFAULT;

	ID3D11Texture2D* zBufferTexture;
	hr = dev->CreateTexture2D(&tex2dDesc, NULL, &zBufferTexture);
	if (FAILED(hr))
	{
		LOG("Failed to create Z-Buffer Texture");
		return E_FAIL;
	}

	// create the depth buffer view
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = tex2dDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	hr = dev->CreateDepthStencilView(zBufferTexture, &dsvDesc, &depthBuffer);
	if (FAILED(hr))
	{
		LOG("Failed to create dpeth stencil view");
		return E_FAIL;
	}

	zBufferTexture->Release();

	return S_OK;
}

void Renderer::RenderFrame()
{
	// clear back buffer with colour
	devcon->ClearRenderTargetView(backBuffer, DirectX::Colors::DarkSlateGray);
	devcon->ClearDepthStencilView(depthBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// select which vertex buffer to use
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset); // This tells the rendering context which vertex buffers should be used.
	devcon->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R32_UINT, 0); // This tells the rendering context which vertex buffers should be used.

	// select which primitive we are using
	devcon->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // This will determine how the vertex buffer should be turned into geometry

	// UPDATE VALUES BEFORE ISSUEING DRAW
	CBuffer_PerObject cbufferData;
	cbufferData.WVP = XMMatrixIdentity();
	XMMATRIX world = transform1.GetWorldMatrix();
	XMMATRIX view = camera.GetViewMatrix();
	XMMATRIX projection = camera.GetProjectionMatrix(window.GetWidth(), window.GetHeight());
	cbufferData.WVP = world * view * projection;
	devcon->UpdateSubresource(cBuffer_PerObject, NULL, NULL, &cbufferData, NULL, NULL);
	devcon->VSSetConstantBuffers(0, 1, &cBuffer_PerObject);

	devcon->DrawIndexed(36, 0, 0); // parameters are number of vertices to draw + where in the buffer to start

	cbufferData.WVP = transform2.GetWorldMatrix() * view * projection;
	devcon->UpdateSubresource(cBuffer_PerObject, NULL, NULL, &cbufferData, NULL, NULL);
	devcon->DrawIndexed(36, 0, 0);

	// flip the back and front buffers
	swapchain->Present(0, 0);
}

// clean up function
void Renderer::Release()
{
	if (backBuffer) backBuffer->Release();
	if (swapchain) swapchain->Release();
	if (dev) dev->Release();
	if (devcon) devcon->Release();
	if (pVS) pVS->Release();
	if (pPS) pPS->Release();
	if (pIL) pIL ->Release();
	if (vBuffer) vBuffer->Release();
	if (iBuffer) iBuffer->Release();
	if (cBuffer_PerObject) cBuffer_PerObject->Release();
	if (depthBuffer) depthBuffer->Release();
}