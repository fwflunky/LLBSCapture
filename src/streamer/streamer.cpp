//
// Created by user on 15.05.2025.
//

#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include "streamer.h"
#include "../converter/converter.h"

HRESULT streamer::d3d11Present1(IDXGISwapChain *t, UINT SyncInterval, UINT Flags, void *p) {
    t->GetDesc(&desc);

    if(desc.BufferDesc.Width != SIZE_X && desc.BufferDesc.Height != SIZE_Y){
        if(!isResizingNow) {
            isResizingNow = true;
            std::cout << "trying to resize window\n";

            std::thread([hwnd = desc.OutputWindow]() {
                static RECT rect = {0, 0, SIZE_X, SIZE_Y};
                AdjustWindowRect(&rect, GetWindowLongA(hwnd, GWL_STYLE), FALSE);
                SetWindowPos(hwnd, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOACTIVATE);

                isResizingNow = false;

                std::cout << "window resized\n";
            }).detach();
        }
    } else if(isBufferExist) {
        auto& conv = converter::convertToBGR24(t);
        InterlockedIncrement(&shared->frame_id);
        memcpy(shared->pixels, conv.data(), conv.size());
    }

    return 0;
}

std::string streamer::createBuffer() {
    std::string name = "Global\\llscapture" + std::to_string(duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()); //unique

    auto hMapFile = CreateFileMappingA(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, sizeof(SharedVideoBuffer), name.c_str());
    if(hMapFile) {
        shared = (SharedVideoBuffer*) MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SharedVideoBuffer));
        if (shared) {
            auto sb = new SharedVideoBuffer();
            memcpy(shared, sb, sizeof(SharedVideoBuffer));
            delete sb;
            isBufferExist = true;
        }
    }

    return shared ? name : "";
}
