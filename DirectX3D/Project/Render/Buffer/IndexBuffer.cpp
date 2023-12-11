#include "Framework.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(vector<UINT>& indices)
{
    D3D11_BUFFER_DESC bufferDesc = {};

    bufferDesc.ByteWidth = sizeof(UINT) * indices.size();
    bufferDesc.Usage = D3D11_USAGE_DEFAULT; // DEFAULT는 CPU에서 정보 수정 불가능
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA data;

    data.pSysMem = indices.data(); // vertices[0]으로 넘길 수 있지만 이 경우 vertices[0]이 비어있으면 오류가 생김

    DEVICE->CreateBuffer(&bufferDesc, &data, &indexBuffer);
}

IndexBuffer::~IndexBuffer()
{
    indexBuffer->Release();
}

void IndexBuffer::IASetBuffer()
{
    DC->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
}
