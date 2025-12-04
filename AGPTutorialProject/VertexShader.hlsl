#define MAX_POINT_LIGHTS 8

struct VIn
{
    float3 position : SV_Position;
    float2 uv : TEXCOORD;
    float3 norm : NORMAL;
};

struct VOut
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD;
    float4 colour : COLOR;
};

struct PointLight
{
    float4 position;
    
    float4 colour;
    
    float strength;
    bool enabled;
    float2 padding;
};

cbuffer PerObjectCB
{
    matrix world;
    float4 ambientLightColour;
    float4 directionalLightColour;
    float4 directionalLightDirection;
    PointLight pointLights[MAX_POINT_LIGHTS];
};


VOut main(VIn input)
{
    VOut output;
    output.position = mul(world, float4(input.position, 1));
    output.uv = input.uv;
    
    //lighting
    float diffuseAmount = dot(directionalLightDirection.xyz, input.norm);
    diffuseAmount = saturate(diffuseAmount);
    float3 directionalFinal = directionalLightColour * diffuseAmount;
    
    //point lighting
    float3 pointFinal = float3(0, 0, 0);
    
    for (int i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        if(!pointLights[i].enabled)
            continue;
        
        float3 pointLightDir = normalize(pointLights[i].position.xyz - input.position);
        float pointLightDistance = length(pointLights[i].position.xyz - input.position);
        float pointLightAttenuation = pointLights[i].strength / (pointLightDistance * pointLightDistance + pointLights[i].strength);
        float pointAmount = dot(pointLightDir, input.norm) * pointLightAttenuation;
        pointAmount = saturate(pointAmount);
        pointFinal += pointLights[i].colour * pointAmount;
    }
    
    output.colour = saturate(ambientLightColour + float4(directionalFinal, 1) + float4(pointFinal, 1));
    //output.colour = float4(pointFinal, 1);
    return output;
}