#include "Header.hlsli"

struct VertexOutput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

float4 main(VertexOutput input) : SV_TARGET
{
    float4 albedo = diffuseMap.Sample(samp, input.uv);
    
    //float scale = (albedo.r * 0.3f + albedo.g * 0.59f + albedo.b * 0.11f);
    
    float scale = dot(albedo.rgb, float3(0.3f, 0.59f, 0.11f));
    
    return scale.xxxx * mDiffuse; // 알파값은 처리하기 나름, 굳이 여기서 처리할 필요는 없음
}