struct VIn
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NOMRAL;
};

cbuffer PerObject : register(b12)
{
    matrix World;
    matrix WVP;
}

cbuffer PerFrame : register(b11)
{
    float3 cameraPosition;
    float padding;
}