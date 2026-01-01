#include "Material_Lit.h"
#include "d3d11.h"
#include "GameObject.h"
#include "Texture.h"
#include "Renderer.h"
#include "Lighting.h"

Material_Lit::Material_Lit(std::string name, Renderer& renderer, 
	std::string vShaderFilename, std::string pShaderFilename, 
	Texture* texture) : Material(name, renderer, vShaderFilename, pShaderFilename, texture)
{
	CreateCBuffer(sizeof(CBufferLighting), cbuffer);
	CreateCBuffer(sizeof(CBufferPS), cbufferPixelShader);
}

void Material_Lit::Bind()
{
	Material::Bind();

	if (cbufferPixelShader != nullptr)
	{
		devcon->PSSetConstantBuffers(0,1,&cbufferPixelShader);
	}

	if (skyboxTexture != nullptr)
	{
		ID3D11ShaderResourceView* t = skyboxTexture->GetTexture();
		devcon->PSGetShaderResources(1, 1, &t);
	}
}

void Material_Lit::UpdateMaterial(GameObject* GO)
{
	using namespace DirectX;
	Material::UpdateMaterial(GO);

	//lighting
	CBufferLighting cbData;
	//ambient light
	cbData.ambientLightColour = renderer.ambientLightColour;
	// directional light
	DirectionalLight& dirLight = renderer.directionalLight;
	cbData.directionalLight.colour = dirLight.colour;
	XMMATRIX transpose = XMMatrixTranspose(GO->transform.GetWorldMatrix());
	cbData.directionalLight.transposedDirection = XMVector3Transform(XMVector3Normalize(dirLight.directionFrom), transpose);

	PointLight* pointLights = renderer.pointLights;
	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		cbData.pointLights[i].enabled = pointLights[i].enabled;

		if (!pointLights[i].enabled)
			continue;

		XMMATRIX inverse = XMMatrixInverse(nullptr, GO->transform.GetWorldMatrix());

		cbData.pointLights[i].position = XMVector3Transform(pointLights[i].position, inverse);
		cbData.pointLights[i].colour = pointLights[i].colour;
		cbData.pointLights[i].strength = pointLights[i].strength;
	}

	UpdateCBuffer(cbData, cbuffer);

	CBufferPS cbpsData;
	cbpsData.reflectiveness = reflectiveness;
	UpdateCBuffer(cbpsData, cbufferPixelShader);
}
