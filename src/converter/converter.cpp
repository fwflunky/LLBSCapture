//
// Created by user on 04.05.2025.
//

#include "converter.h"
#include <d3d11.h>

std::string& converter::convertToBGR24(IDXGISwapChain* sw) {
    static std::string buffer;
    static ID3D11Device* device = nullptr;
    static ID3D11DeviceContext* context = nullptr;
    if(!device){
        sw->GetDevice(__uuidof(ID3D11Device), (void**)&device);
        device->GetImmediateContext(&context);
    }

    ID3D11Texture2D* backBuffer = nullptr;
    sw->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

    static D3D11_TEXTURE2D_DESC desc = {};
    backBuffer->GetDesc(&desc);

    desc.Usage = D3D11_USAGE_STAGING;
    desc.BindFlags = 0;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    desc.MiscFlags = 0;

    static ID3D11Texture2D* stagingTexture = nullptr;
    device->CreateTexture2D(&desc, nullptr, &stagingTexture);

    context->CopyResource(stagingTexture, backBuffer);

    static D3D11_MAPPED_SUBRESOURCE mapped = {};
    context->Map(stagingTexture, 0, D3D11_MAP_READ, 0, &mapped);

    int width = (int) desc.Width;
    int height = (int) desc.Height;
    int pitch = (int) mapped.RowPitch;

    buffer.resize(width * height * 3);

    auto src = (uint8_t*)mapped.pData;
    for (int y = 0; y < height; ++y) {
        uint8_t* row = src + y * pitch;
        for (int x = 0; x < width; ++x) {
            uint8_t R = row[x * 4 + 0];
            uint8_t G = row[x * 4 + 1];
            uint8_t B = row[x * 4 + 2];

            int dstIndex = (y * width + x) * 3;
            buffer[dstIndex + 0] = (char) B;
            buffer[dstIndex + 1] = (char) G;
            buffer[dstIndex + 2] = (char) R;
        }
    }

    context->Unmap(stagingTexture, 0);
    stagingTexture->Release();
    backBuffer->Release();

    return buffer;
}