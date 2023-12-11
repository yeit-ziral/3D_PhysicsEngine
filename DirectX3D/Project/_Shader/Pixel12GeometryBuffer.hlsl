#include "Header.hlsli"

GBufferOutput main(LightVertexOutput input) : SV_TARGET
{
    LightMaterial material = GetLightMaterial(input);
    
    return PackGBuffer(material.diffuseColor.rgb, material.normal, material.specularIntecsity.r, CalculateAmbient(material));
}