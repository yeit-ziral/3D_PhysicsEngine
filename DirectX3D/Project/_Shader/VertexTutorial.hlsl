cbuffer World : register(b0)
{
    matrix world;

};

cbuffer View : register(b1)
{
    matrix view;
};

cbuffer Proj : register(b2)
{
    matrix projection;
};

struct VertexInput
{
    float4 pos : POSITION; // 동차좌표계 쓰려고 float3가 아닌 float4로 한 차수 올림
    float4 color : COLOR;
};

//struct VertexColorNormal
//{
//    float4 pos : POSITION;
//    float4 color : COLOR;
//    float3 normal : NORMAL;
//};

struct VertexOutput // VertexInput을 그대로 반환하면 pos의 symentic name이 SV_POSITION이 아니라서 만듬
{
    float4 pos      : SV_POSITION; //SV_POSITION (System value position)-> main의 반환값으로 리턴값이 RS(Rasterizer : 시스템)로 간다는 뜻 중간단계인 HS나 TS가 존재하면 SV_POSITION이 아닌 다른 것을 써 해당 중간단계로 보내줌
    float4 color    : COLOR;
};

VertexOutput main(VertexInput input)
{
    //float4 a = { 1, 2, 3, 4 };
	//
    //float4 b = a.xyzw;
    //float4 b1 = a.rgba; // hlsl은 좀 더 유연하게 사용 가능 
    
    VertexOutput output;
    
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.color = input.color;
    
	return output;
}