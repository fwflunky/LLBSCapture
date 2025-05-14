#include "src/3pp/json/nlohmann/json.hpp"
#include "src/util/util.h"
#include "src/d3d11Hook/d3d11Hook.h"
#include "src/streamer/streamer.h"
#include "src/3pp/base64.h"

#include <windows.h>
#include <MinHook.h>
#include <iostream>
#include <thread>
#include <fstream>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


bool initialize(HMODULE hModule){
    char mname[256];
    GetModuleFileNameA(GetModuleHandleA(nullptr), &mname[0], 256);

    if(!std::string(mname).contains("HD-Player.exe")) {
        FreeLibrary(GetModuleHandleA(nullptr));
        return true;
    }

    std::thread([]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        util::setupConsole();

        MH_Initialize();

        while(!d3d11Hook::tryHook(streamer::d3d11Present1)){
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        auto args = util::parseArgs();
        std::cout << "instance id: " << args[2] << "\n";

        std::string instName;
        std::string instAddr;
        std::string adapt;

        std::ifstream conf(CONF_PATH, std::ios::in | std::ios::binary);
        if (conf) {
            std::string content((std::istreambuf_iterator<char>(conf)), std::istreambuf_iterator<char>());
            auto data = util::parseConfig(content);
            if (data.contains("bst.instance." + args[2] + ".display_name")) {
                instName = data["bst.instance." + args[2] + ".display_name"];
            }
            conf.close();
        }

        if(instName.empty()) {
            std::cout << "instName.empty()\n";
            return;
        }

        std::cout << "instance name: " << instName << "\n";

        for(int i = 0; i < 10; i++) {
            try {
                conf = std::ifstream(INSTANCES_PATH + args[2] + "\\HypervVm.json", std::ios::in | std::ios::binary);
                if (conf) {
                    std::string content((std::istreambuf_iterator<char>(conf)), std::istreambuf_iterator<char>());
                    auto data = nlohmann::json::parse(content);
                    adapt = data["HcsSystem"]["VirtualMachine"]["Devices"]["NetworkAdapters"]["default"]["EndpointId"];
                }
            } catch(...) {}

            if(!adapt.empty())
                break;

            std::this_thread::sleep_for(std::chrono::seconds(2));
        }

        if(adapt.empty()){
            std::cout << "adapt.empty()\n";
            return;
        }

        std::cout << "adapter id: " << adapt << "\n";

        for(int i = 0; i < 10; i++) {
            auto ip = util::executePowerShell("(Get-HnsEndpoint | Where { $_.ID -eq '"+ adapt +"' }).IPAddress");
            if(util::executePowerShell("Test-Connection -ComputerName " + ip + " -Count 1 -Quiet").contains("True")) {
                instAddr = ip;
                break;
            }

            std::cout << "waiting for VM to boot...\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }

        if(instAddr.empty()) {
            std::cout << "instAddr.empty()\n";
            return;
        }

        std::cout << "local instance addr: " << instAddr << "\n";
        instAddr += ":5555"; //+adb port

        auto capturePipe = streamer::createBuffer();
        if(!capturePipe.empty()) {
            util::spawnDetachedProcess(R"(D:\Projects\GolandProjects\cocbot)", R"(D:\Projects\GolandProjects\cocbot\sv2.exe)", anus::Base64::Encode(instName) + " " + instAddr + " " + capturePipe);
        } else {
            std::cout << "capturePipe.empty()\n";
        }
    }).detach();
    return true;
}

#pragma optimize("", off)
[[maybe_unused]] BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, DWORD lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) { //F4696
        return initialize(hModule);
    }
    return FALSE;
}
