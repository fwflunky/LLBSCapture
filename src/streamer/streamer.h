//
// Created by user on 15.05.2025.
//

#ifndef UMPDC_STREAMER_H
#define UMPDC_STREAMER_H


#include <dxgi.h>
#include <windows.h>
#include <atomic>
#include "../../hardcoded.h"

class streamer {
public:
#pragma pack(push, 1)
    struct SharedVideoBuffer {
        volatile long frame_id = 0;
        int width = SIZE_X;
        int height = SIZE_Y;
        unsigned char pixels[SIZE_X * SIZE_Y * 3] = {}; //bgr
    };
#pragma pack(pop)

    static std::string createBuffer();

    static HRESULT d3d11Present1(IDXGISwapChain *t, UINT SyncInterval, UINT Flags, void* p);
private:
    static inline DXGI_SWAP_CHAIN_DESC desc = {};
    static inline std::atomic_bool isResizingNow = false;
    static inline std::atomic_bool isBufferExist = false;

    static inline SharedVideoBuffer* shared = nullptr;
};


#endif //UMPDC_STREAMER_H
