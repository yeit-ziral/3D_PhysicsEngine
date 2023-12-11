#pragma once
class ConstBuffer // Shader에 어떤 데이터도 넣을 수 있게 해 줌
{
public:
	ConstBuffer(void* data, UINT dataSize); // 어떤 형식이 들어올지 몰라서 void pointer로 만듬
	virtual ~ConstBuffer();

	void SetVSBuffer(UINT slot);
	void SetPSBuffer(UINT slot);
	void SetCSBuffer(UINT slot);

	void UpdateSubResource();

private:
	void* data;
	UINT  dataSize;

	ID3D11Buffer* constBuffer;

	D3D11_MAPPED_SUBRESOURCE subResource;
};