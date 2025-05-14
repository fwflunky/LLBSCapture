//
// Created by user on 14.05.2025.
//

#include "d3d11Hook.h"
#include <d3dcommon.h>
#include <MinHook.h>

bool d3d11Hook::tryHook(demangled recv) {
    D3D_FEATURE_LEVEL featLevel;
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.Height = 800;
    sd.BufferDesc.Width = 600;
    sd.BufferDesc.RefreshRate = {60, 1};
    sd.OutputWindow = GetForegroundWindow();
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &sd, &swapChain, &device, &featLevel, nullptr)))
        return false;

    receiver = recv;

    MH_CreateHook((*(void***)swapChain)[22], (void*) hooked, (void**) &original);
    return MH_EnableHook((*(void***)swapChain)[22]) == MH_OK;
}

HRESULT d3d11Hook::hooked(IDXGISwapChain *t, UINT SyncInterval, UINT Flags, void *v) {
    receiver(t, SyncInterval, Flags, v);
    return original(t, SyncInterval, Flags, v);
}
