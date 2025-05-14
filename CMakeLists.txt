cmake_minimum_required(VERSION 3.27)
project(UMPDC)

set(CMAKE_CXX_STANDARD 23)
include_directories("D:\\MinHook_133_lib\\include")

add_library(UMPDC SHARED library.cpp
        src/converter/converter.cpp
        src/converter/converter.h
        src/d3d11Hook/d3d11Hook.cpp
        src/d3d11Hook/d3d11Hook.h
        src/streamer/streamer.cpp
        src/streamer/streamer.h
        hardcoded.h
        src/util/util.cpp
        src/util/util.h
)

target_compile_options(UMPDC PRIVATE /MP /arch:AVX512 /EHsc /D_HAS_EXCEPTIONS=0 /MD /Qvec-report:1 /Gz /Qpar-report:1 /GR- /wd4005 /Zi /Gy /DNDEBUG)

target_link_libraries(UMPDC PUBLIC "D:\\MinHook_133_lib\\lib\\libMinHook-x64-v141-mt.lib")