#include <Common.hlsli>

struct VOut
{
    float4 position : SV_Position;
    float3 uvw : TEXCOORD;
};

VOut main( VIn input )
{
    VOut output;
    
    output.position = mul(WVP, float4(input.position, 1));
    
    output.uvw = input.position.xyz;
    
    return output;
}