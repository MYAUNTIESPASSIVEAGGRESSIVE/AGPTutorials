Texture2D texture0 : register(t0);
TextureCube skybox0 : register(t1);
sampler sampler0;

struct PIn
{
    float4 position : SV_Position;
    float4 colour : COLOR;
    float2 uv : TEXCOORD0;
    float3 uvw : TEXCOORD1;
};

cbuffer CbufferPS
{
    float reflectiveness;
    float3 padding;
};

float4 main(PIn input) : SV_TARGET
{
    float4 sampled = texture0.Sample(sampler0, input.uv);
    //clip(sampled.a < 0.1f ? -1 : 1); // negates depth and blend states
    float4 reflectedSampled = skybox0.Sample(sampler0, input.uvw);
    
    float4 combined = ((input.colour * sampled * 0.6) * (1.0 - reflectiveness)) +(reflectedSampled * reflectiveness); // hard coded mixes -> use constant buffers to be able to control this
    
    return saturate(combined);
}