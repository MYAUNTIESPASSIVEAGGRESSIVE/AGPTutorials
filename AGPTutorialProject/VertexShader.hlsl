struct VIn
{
    float3 position : SV_Position;
    float2 uv : TEXCOORD;
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
};

VOut main(VIn input)
{
    VOut output;
    output.position = mul(world, float4(input.position, 1));
    output.colour = float4(1, 1, 1, 1);
    output.uv = input.uv;
    return output;
}