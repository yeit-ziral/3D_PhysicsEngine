
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
    float4 pos : POSITION;
};

struct VertexOutput // VertexInput을 그대로 반환하면 pos의 symentic name이 SV_POSITION이 아니라서 만듬
{
    float4 pos : SV_POSITION;
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
    
    return output;
}