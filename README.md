# LLBSCapture

**LLBSCapture** (Low Latency BlueStacks Screen Capture) is a C++ DLL designed to capture the screen of a running BlueStacks emulator instance with minimal latency.

## Requirements

- C++ compiler with CMake support
- [MinHook](https://github.com/TsudaKageyu/minhook) (edit CMakeLists.txt)
- BlueStacks version **5.14.0**
- DirectX screen rendering enabled in BlueStacks
- Programmatic UI rendering enabled in BlueStacks
- Both HD-Player and client started as Administrator to access Global pipe namespace

## Build Instructions

1. **Clone this repository**:
   ```bash
   git clone https://github.com/fwflunky/LLBSCapture.git

2. **Configure**:
 
- Set the correct path to the MinHook library in CMakeLists.txt.
- Open hardcoded.h
- Set your screen size and BlueStacks folder paths as needed.

3. **Build using your compiler**

# Usage
- Copy the built DLL into the BlueStacks folder (C:\Program Files\BlueStacks_nxt\UMPDC.dll). Ensure that screen rendering is set to DirectX and that the UI rendering is set to programmatic, or the capture may not function properly.

# Bonus Feature
The project includes functionality to detect the local IP address of the BlueStacks virtual machine, which can be used for ADB
