
#include "Header.hlsli"

struct VertexOutput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 viewDir : VIEWDIR;
};

float4 main(VertexOutput input) : SV_TARGET
{
    float3 L = normalize(-lights[0].direction);
    
    float diffuseIntensity = saturate(dot(input.normal, -L)); //N dot L
    
    float4 albedo = float4(1, 1, 1, 1);
    
    if(hasDiffuseMap)
        albedo = diffuseMap.Sample(samp, input.uv); // 모든 반사
    
 
    
    //SPECULAR
    float specularIntensity = 0;
    
    float3 reflection = normalize(reflect(L, input.normal)); // 정반사
    
    specularIntensity = saturate(dot(-reflection, input.viewDir));
    
    float4 specularSample = float4(1, 1, 1, 1);
    
    if(hasSpecularMap)
        specularSample = specularMap.Sample(samp, input.uv);
    
    float4 specular = pow(specularIntensity, shininess) * specularSample * mSpecular;
    
    float4 diffuse = albedo * diffuseIntensity * mDiffuse;
    
    float4 ambient = albedo * float4(ambientLight, 1.0f) * mAmbient;
    
    return diffuse + specular + ambient;
}