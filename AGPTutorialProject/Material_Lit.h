#pragma once
#include "Material.h"
#include "Lighting.h"

class Material_Lit :
    public Material
{
public:
    float reflectiveness = 0.1f;

    Material_Lit(std::string name, Renderer& renderer,
        std::string vShaderFilename, std::string pShaderFilename,
        Texture* texture);

    virtual void Bind() override;

    virtual void UpdateMaterial(GameObject* GO) override;

    void SetReflectionTexture(Texture* reflectionTexture) { skyboxTexture = reflectionTexture; }

protected:

    Texture* skyboxTexture = nullptr;
    ID3D11Buffer* cbufferPixelShader = nullptr;

    struct DirectionalLightCBData
    {
        DirectX::XMVECTOR transposedDirection;
        DirectX::XMVECTOR colour;
    };

    struct PointLightCBData
    {
        DirectX::XMVECTOR position;
        DirectX::XMVECTOR colour;
        float strength = 1;
        unsigned int enabled = 0;
        DirectX::XMFLOAT2 padding;
    };

    struct CBufferLighting : CBufferBase
    {
        DirectX::XMVECTOR ambientLightColour{ 1,1,1,1 };
        DirectionalLightCBData directionalLight;
        PointLightCBData pointLights[MAX_POINT_LIGHTS];
    };

    struct CBufferPS : CBufferBase
    {
        float reflectiveness;
        DirectX::XMFLOAT3 padding;
    };
};

