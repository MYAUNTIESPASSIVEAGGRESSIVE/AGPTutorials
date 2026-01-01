#include "Material.h"
#include "Debug.h"
#include "Texture.h"
#include "Renderer.h"
#include "ShaderLoading.h"
#include "d3d11.h"

Material::Material(std::string name, Renderer& renderer, 
	std::string vShaderFilename, std::string pShaderFilename, 
	Texture* texture)
	: name(name), renderer(renderer), dev(renderer.GetDevice()), devcon(renderer.GetDeviceContext()), texture(texture)
{
	HRESULT hr;

	hr = ShaderLoading::LoadVertexShader(vShaderFilename, dev, &vShader, &vLayout);
	if (FAILED(hr))
	{
		LOG("Material " + name + "failed to load, aborting");
		return;
	}

	hr = ShaderLoading::LoadPixelShader(pShaderFilename, dev, &pShader);
	if (FAILED(hr))
	{
		LOG("Material " + name + "failed to load, aborting");
		return;
	}
}

void Material::Bind()
{
	devcon->VSSetShader(vShader, 0, 0);
	devcon->PSSetShader(pShader, 0, 0);
	devcon->IASetInputLayout(vLayout);

	// fix issue with non-textured mats having previous texture
	if (texture != nullptr)
	{
		ID3D11SamplerState* s = texture->GetSampler();
		devcon->PSSetSamplers(0, 1, &s);
		ID3D11ShaderResourceView* t = texture->GetTexture();
		devcon->PSSetShaderResources(0, 1, &t);
	}

	if (cbuffer != nullptr)
	{
		devcon->VSSetConstantBuffers(0, 1, &cbuffer);
	}
}

Material::~Material()
{
	if (vShader) vShader->Release();
	if (pShader) pShader->Release();
	if (vLayout) vLayout->Release();
}

void Material::CreateCBuffer(unsigned int byteWidth, ID3D11Buffer*& targetBuffer)
{
	D3D11_BUFFER_DESC cbd{ 0 };
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = byteWidth;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	if (FAILED(dev->CreateBuffer(&cbd, NULL, &targetBuffer)))
	{
		LOG("failed to create cbuffer for material");
	}
}

void Material::UpdateCBuffer(CBufferBase& cbData, ID3D11Buffer*& targetBuffer)
{
	devcon->UpdateSubresource(targetBuffer, 0, 0, &cbData, 0, 0);
}
