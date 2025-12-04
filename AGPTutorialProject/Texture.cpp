#include "Texture.h"

#include <WICTextureLoader.h>
#include <d3d11.h>

#include "Renderer.h"

Texture::Texture(Renderer& renderer, std::string path, bool transparent)
	: isTransparent(transparent)
{
	ID3D11Device* dev = renderer.GetDevice();
	ID3D11DeviceContext* devcon = renderer.GetDeviceContext();

	std::wstring filepath = std::wstring(path.begin(), path.end());

	DirectX::CreateWICTextureFromFile(dev, devcon, filepath.c_str(), NULL, &texture);


	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // linear filter
	//samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC; // anisotropic filter
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; // point filter
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	dev->CreateSamplerState(&samplerDesc, &sampler);
}

Texture::~Texture()
{
	if (texture) texture->Release();
	if (sampler) sampler->Release();
}
