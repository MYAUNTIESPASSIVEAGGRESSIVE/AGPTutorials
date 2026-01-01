#define MAX_POINT_LIGHTS 8

struct DirectionalLight
{
    float4 transposedDirection;
    float4 colour;
};

struct PointLight
{
    float4 position;
    float4 colour;
    float strength;
    bool enabled;
    float2 padding;
};

cbuffer LightingData /* : register(b13)*/
{
    float4 ambientLightCol;
    DirectionalLight dirLight;
    PointLight pointLights[MAX_POINT_LIGHTS];
}

float3 CalculateDirectionalContribution(DirectionalLight light, float3 vNormal)
{
    float diffuseAmount = dot(normalize(light.transposedDirection.xyz), vNormal);
    diffuseAmount = saturate(diffuseAmount);
    return light.colour * diffuseAmount;
}

float3 CalculatePointLights(PointLight pointLights[MAX_POINT_LIGHTS], float4 vPosition, float3 vNormal)
{
    float3 pointFinal = float3(0, 0, 0);
    for (int i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        if (!pointLights[i].enabled)
            continue;
        
        float4 pointLightDir = normalize(pointLights[i].position - vPosition);
        float pointLightDistance = length(pointLights[i].position - vPosition);
        float pointLightAttenuation = pointLights[i].strength / (pointLightDistance * pointLightDistance + pointLights[i].strength);
        float pointAmount = dot(pointLightDir.xyz, vNormal) * pointLightAttenuation;
        pointAmount = saturate(pointAmount);
        pointFinal += pointLights[i].colour * pointAmount;
    }
    return pointFinal;
}

float3 CalculateAllLighting(float3 ambientColour, DirectionalLight light, PointLight pointLights[MAX_POINT_LIGHTS], float4 vPosition, float3 vNormal)
{
    float3 directional = CalculateDirectionalContribution(light, vNormal);
    float3 pointTotal = CalculatePointLights(pointLights, vPosition, vNormal);
    return saturate(ambientColour + directional + pointTotal);
}

float3 CalculateReflectionUVW(matrix World, float4 vertexPos, float3 vertexNorm, float3 camPos)
{
    // vertex world pos
    float3 wpos = mul(World, vertexPos);
    
    // surface nromal in WS
    float3 wnormal = normalize(mul(World, float4(vertexNorm, 0)));
    
    // obtain eye vector
    float3 eyeDir = normalize(camPos - wpos);
    
    // compute refect vector
    //return 2.0 * dot(eyeDir, wnormal) * wnormal - eyeDir;
    return reflect(-eyeDir, wnormal); // this is same as above ^^
}