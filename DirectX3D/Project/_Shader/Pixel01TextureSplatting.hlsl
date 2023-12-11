#include "Header.hlsli"

cbuffer BrushBuffer : register(b10)
{
    int type;
    float3 location;
    
    float range;
    float3 color;
}

struct VertexOutput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewDir : VIEWDIR;
    float3 worldPos : POSITION;
    
    float4 alpha : ALPHA;
};

float3 SetBrushColor(float3 pos)
{
    if (type == 0)
    {
        float x = pos.x - location.x;
        float z = pos.z - location.z;
        
        float distance = sqrt(pow(x, 2) + pow(z, 2));

        
        if (distance <= range)
            return color;
    }
    else if (type == 1)
    {
        float x = pos.x - location.x;
        float z = pos.z - location.z;
        
        if (abs(x) <= range && abs(z) <= range)
            return color;
    }
    
    
    return float3(0, 0, 0);
}

Texture2D alphaMap : register(t10);
Texture2D secondMap : register(t11);

float4 main(LightVertexOutput input) : SV_TARGET
{
    LightMaterial material = GetLightMaterial(input);

    float4 ambient = CalculateAmbient(material);
    
    float4 alpha = alphaMap.Sample(samp, input.uv);
    float4 second = secondMap.Sample(samp, input.uv);
    
    float4 albedo = material.diffuseColor;
    
    if (hasAlphaMap)
        albedo = lerp(albedo, second, alpha.r);
    // else
    //     albedo = lerp(albedo, second, input.alpha.r);
    
    float4 brushColor = float4(SetBrushColor(input.worldPos), 1.0f);
    
    return albedo + brushColor;
}