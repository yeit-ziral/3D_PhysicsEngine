#pragma once
class StructuredBuffer
{
public:
	StructuredBuffer(void* inputData, UINT inputStride, UINT inputCount, UINT outputStride, UINT outputCount);
	~StructuredBuffer();

	void Copy(void* data, UINT size); // GPU에서 넘겨받은 읽을 수 있는 데이터를 쓸 수 있는 데이터로 변환
	void UpdateInput(void* data);

	ID3D11UnorderedAccessView* GetUAV() { return uav; }
	ID3D11ShaderResourceView* GetSRV() { return srv; }

	// D3D11_USAGE_WRITE는 버퍼를 다시 GPU에서 사용할 때


	void SetSRV();
	void SetUAV();

private:
	void CreateInput();
	void CreateSRV();
	void CreateOutput();
	void CreateUAV(); // 데이터를 채우는 역할
	void CreateResult(); // output은 D3D11_USAGE_DEFAULT이라 CPU가 읽지 못하기에 여기서 D3D11_USAGE_STAGING로 변환

private:
	ID3D11UnorderedAccessView* uav; // GPU에서 정보를 계산한 결과값을 CPU에 넘겨주기 위해, 쓰기용(D3D11_USAGE_DEFAULT)
	ID3D11ShaderResourceView*  srv; // lay, poligon의 정보를 GPU에 넘기기 위함,  읽기용

	ID3D11Resource*  input; // GPU에 넘겨줄 데이터
	ID3D11Resource* output; // CPU에 넘겨줄 데이터
	ID3D11Resource* result; // CPU가 읽을 수 있는 데이터로 변환되어 전달

	// input, srv(함께 생성) -> output,result,uav (함께 생성)
	// 데이터 변환 순서(?) : input -> srv -> output -> uav  -> result 함수 순서

	void* inputData;
	UINT inputStride;
	UINT inputCount;

	UINT outputStride;
	UINT outputCount;
};