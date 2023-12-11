#include "Framework.h"
#include "ConstBuffer.h"

ConstBuffer::ConstBuffer(void* data, UINT dataSize)
    : data(data), dataSize(dataSize)
{
    D3D11_BUFFER_DESC bufferDesc = {};

    bufferDesc.ByteWidth            = dataSize;                     // 만들 크기
    bufferDesc.Usage                = D3D11_USAGE_DYNAMIC;          // 사용처, 다른 종류들의 특징과 차이점 기억해야 함  중요!!
    bufferDesc.BindFlags            = D3D11_BIND_CONSTANT_BUFFER;   
    bufferDesc.CPUAccessFlags       = D3D11_CPU_ACCESS_WRITE;       // 다른 기능도 추가하려면 or 연산자 | 를 사용하면 됨, CPU가 쓰기 가능하게 함
    bufferDesc.MiscFlags            = 0; //기타등등의 느낌
    bufferDesc.StructureByteStride  = 0;

    DEVICE->CreateBuffer(&bufferDesc, nullptr, &constBuffer); // 데이터 쓰지 않음
}

ConstBuffer::~ConstBuffer()
{
    constBuffer->Release();
}

void ConstBuffer::SetVSBuffer(UINT slot)
{
    UpdateSubResource();

    DC->VSSetConstantBuffers(slot, 1, &constBuffer);
}

void ConstBuffer::SetPSBuffer(UINT slot)
{
    UpdateSubResource();

    DC->PSSetConstantBuffers(slot, 1, &constBuffer);
}

void ConstBuffer::SetCSBuffer(UINT slot)
{
    UpdateSubResource();

    DC->CSSetConstantBuffers(slot, 1, &constBuffer);
}

void ConstBuffer::UpdateSubResource()
{
    // Map, Unmap

    DC->Map(constBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource); // Map : 쓸 수 있게 뚜껑을 여는것, 여기서는 subResource를 수정할 수 있게 하는 작업이다.

    memcpy(subResource.pData, data, dataSize); // 배열의 데이터를 복사할 때 씀 , _Dst-> 목적지, _Src->원본, dataSize만큼 옮김

    DC->Unmap(constBuffer, 0); // Unmap : 다시 뚜껑을 닫는것, 정보 오염을 막는 역할

    //DC->UpdateSubresource(constBuffer, 0, nullptr, data, 0, 0); // 여기서 데이터(WVP의 행렬) 넘겨줌
}
