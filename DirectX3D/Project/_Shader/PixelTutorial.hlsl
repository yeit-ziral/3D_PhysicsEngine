struct VertexOutput 
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float diffuse : DIFFUSE;
};


float4 main(VertexOutput input) : SV_TARGET // -> 결과값을 OM으로 보내준다는 뜻, symentic name-> 변수의 사용 용도를 적어 둔 것,
{
	//return float4(1.0f, 1.0f, 0.0f, 1.0f); // 1.0f -> UNORM 형식
    return input.color;
}