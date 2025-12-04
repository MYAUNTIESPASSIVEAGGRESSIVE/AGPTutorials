Texture2D texture0;
sampler sampler0;

float4 main(float4 position : SV_Position, float2 uv : TEXCOORD, float4 colour : COLOR) : SV_TARGET
{
    float4 sampled = texture0.Sample(sampler0, uv);
    //clip(sampled.a < 0.1f ? -1 : 1); // negates depth and blend states
    return colour * sampled;
}