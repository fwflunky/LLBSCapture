//
// Created by user on 14.05.2025.
//

#ifndef UMPDC_D3D11HOOK_H
#define UMPDC_D3D11HOOK_H

#include <d3d11.h>
#include <dxgi.h>

using demangled = HRESULT(*)(IDXGISwapChain* t, UINT SyncInterval, UINT Flags, void* v);

class d3d11Hook {
public:
    static bool tryHook(demangled recv);
private:
    static HRESULT hooked(IDXGISwapChain* t, UINT SyncInterval, UINT Flags, void* v);

    static inline demangled original = nullptr;
    static inline demangled receiver = nullptr;

    static inline ID3D11Device* device = nullptr;
    static inline IDXGISwapChain* swapChain = nullptr;
};


#endif //UMPDC_D3D11HOOK_H
