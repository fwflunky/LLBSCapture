//
// Created by user on 04.05.2025.
//

#ifndef LLBSCAPTURE_CONVERTER_H
#define LLBSCAPTURE_CONVERTER_H


#include <string>
#include <windows.h>
#include <dxgi.h>

class converter {
public:
    static std::string& convertToBGR24(IDXGISwapChain* sw);
};


#endif //LLBSCAPTURE_CONVERTER_H
