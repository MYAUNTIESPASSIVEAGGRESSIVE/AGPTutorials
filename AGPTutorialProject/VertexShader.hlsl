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

cbuffer PerObjectCB
{
    matrix world;
    float4 ambientLightColour;
    float4 directionalLightColour;
    float4 directionalLightDirection;
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
    output.colour = saturate(ambientLightColour + float4(directionalFinal, 1));
    
    //output.colour = float4(1, 1, 1, 1);
    return output;
}