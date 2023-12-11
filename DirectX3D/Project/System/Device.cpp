#include "Framework.h"
#include "Device.h"

Device::Device()
{
    CreateDeviceAndSwapChain();
    CreateBackBuffer();
}

Device::~Device()
{
                device->Release();
         deviceContext->Release();
             swapChain->Release();
      renderTargetView->Release();
      depthStencilView->Release();
}

void Device::CreateDeviceAndSwapChain()
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

    swapChainDesc.BufferCount = 1; // backBuffer가 몇개인가

    swapChainDesc.BufferDesc.Width = WIN_WIDTH;
    swapChainDesc.BufferDesc.Height = WIN_HEIGHT; // 크기
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // RGBA를 실수로 넣겠다는 뜻
    // RGBA 8비트 * 4개 = 32비트, UNORM = Unsigned Normal = 0 ~ 1

    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60; //refreshRate = 갱신빈도(얼마나 화면을 갱신하냐), Numerator : 분자
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1; // Denomiantor : 분모

    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // buffer 사용처가 rendertarget ouput이라는 뜻

    swapChainDesc.OutputWindow = hWnd;

    swapChainDesc.SampleDesc.Count = 1; // sampling(표본화)와 관련있음, 확대,축소시 어떻게 그릴것인지 미리 계산
    swapChainDesc.SampleDesc.Quality = 0;

    swapChainDesc.Windowed = true; // 전체화면을 쓸것인지, 창 화면을 쓸것인지 정함
    // Desc는 뭔가를 만든다는 뜻임

    D3D11CreateDeviceAndSwapChain
    (
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        0,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT, // 속성
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &swapChain,
        &device,
        nullptr,
        &deviceContext
    );
}

void Device::CreateBackBuffer()
{
    ID3D11Texture2D* backBuffer;

    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

    device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);

    backBuffer->Release();


    // DSV

    ID3D11Texture2D* depthBuffer;

    D3D11_TEXTURE2D_DESC depthDesc;

    depthDesc.Width = WIN_WIDTH;
    depthDesc.Height = WIN_HEIGHT;
    depthDesc.MipLevels = 1; // 확대 축소에 관한 내용
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // Depth Stancil을 나눠서 처리한다
    depthDesc.SampleDesc.Count = 1;
    depthDesc.SampleDesc.Quality = 0;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthDesc.CPUAccessFlags = 0;
    depthDesc.MiscFlags = 0;

    device->CreateTexture2D(&depthDesc, nullptr, &depthBuffer);

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

    dsvDesc.Format          = depthDesc.Format;
    dsvDesc.ViewDimension   = D3D11_DSV_DIMENSION_TEXTURE2D;

    device->CreateDepthStencilView(depthBuffer, &dsvDesc, &depthStencilView);

    depthBuffer->Release();

}

void Device::Clear()
{
    deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView); // 랜더링 파이프라인의 Output Merge 단계

    float clearColor[4] = { 0.33f, 0.33f, 0.4f, 1.0f };

    deviceContext->ClearRenderTargetView(renderTargetView, clearColor); // backBuffer를 clear 함

    deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
}

void Device::Present()
{
    swapChain->Present(0, 0); // backBuffer와 frontBuffer를 교체하는 함수
}
