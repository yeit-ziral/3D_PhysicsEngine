
#include "Header.hlsli"

float4 main(LightVertexOutput input) : SV_TARGET
{
    LightMaterial material = GetLightMaterial(input);
    
    float4 ambient = CalculateAmbient(material);
    //float4 color = CalculateDirectional(material, lights[0]); // 나중에 active = true인 얘들을 켜주도록 바꿔야 함
    float4 color = CalculateLights(material);
    
    return ambient + color + mEmissive;
}