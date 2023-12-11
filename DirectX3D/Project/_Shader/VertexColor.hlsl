#include "Header.hlsli"


struct VertexOutput // VertexInput을 그대로 반환하면 pos의 symentic name이 SV_POSITION이 아니라서 만듬
{
    float4 pos : SV_POSITION; //SV_POSITION (System value position)-> main의 반환값으로 리턴값이 RS(Rasterizer : 시스템)로 간다는 뜻 중간단계인 HS나 TS가 존재하면 SV_POSITION이 아닌 다른 것을 써 해당 중간단계로 보내줌
    float4 color : COLOR;
    float3 normal : NORMAL;
};

VertexOutput main(VertexColorNormal input)
{
    
    VertexOutput output;
    
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    output.color = input.color;
    
    output.normal = normalize(mul(input.normal, (float3x3) world));
    
    
    return output;
}