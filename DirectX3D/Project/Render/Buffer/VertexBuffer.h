#pragma once
class VertexBuffer
{
public:
	template<typename T>
	VertexBuffer(vector<T>& vertices);
	~VertexBuffer();

	void IASetBuffer( D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void IASetBuffer(UINT slot, D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    void UpdateVertex(void* data, UINT count);

private:
	ID3D11Buffer* vertexBuffer; // constBuffer, IndexBuffer등이 추가로 있음

    UINT stride = 0;
    UINT offset = 0;
};

template<typename T>
inline VertexBuffer::VertexBuffer(vector<T>& vertices)
{
    stride = sizeof(T);
    offset = 0;

    // vertexBuffer에 담아서 넘김
    D3D11_BUFFER_DESC bufferDesc = {};

    bufferDesc.ByteWidth           = stride * vertices.size();
    bufferDesc.Usage               = D3D11_USAGE_DEFAULT; // DEFAULT는 CPU에서 정보 수정 불가능
    bufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags      = 0;
    bufferDesc.MiscFlags           = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA data; //초기 데이터 설정

    data.pSysMem = vertices.data(); // vertices[0]으로 넘길 수 있지만 이 경우 vertices[0]이 비어있으면 오류가 생김

    DEVICE->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
}