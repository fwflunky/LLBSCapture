//
// Created by user on 15.05.2025.
//

#include "util.h"
#include <windows.h>
#include <sstream>
#include <iomanip>

bool util::spawnDetachedProcess(const std::string &cwd, const std::string &exePath, const std::string &args) {
    HANDLE hJob = CreateJobObjectA(nullptr, nullptr);
    if (!hJob)
        return false;

    JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = {};
    jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    SetInformationJobObject(hJob, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli));

    STARTUPINFO si = {sizeof(STARTUPINFO)};
    PROCESS_INFORMATION pi = {};

    auto cmdLine = "\"" + exePath + "\" " + args;

    if (!CreateProcessA(nullptr, cmdLine.data(), nullptr, nullptr, FALSE, CREATE_NEW_CONSOLE | CREATE_SUSPENDED, nullptr, cwd.c_str(), &si, &pi)) {
        CloseHandle(hJob);
        return false;
    }

    AssignProcessToJobObject(hJob, pi.hProcess);
    ResumeThread(pi.hThread);
    return true;
}

std::string util::executePowerShell(const std::string &cmd) {
    std::string result;

    auto fullCommand = "powershell -Command \"" + cmd + "\"";
    FILE* pipe = _popen(fullCommand.c_str(), "r");
    if (!pipe) {
        return result;
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    _pclose(pipe);
    result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
    return result;
}

std::unordered_map<std::string, std::string> util::parseConfig(const std::string &data) {
    std::unordered_map<std::string, std::string> result;

    std::istringstream stream(data);
    std::string line;

    while (std::getline(stream, line)) {
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty())
            continue;

        size_t eq_pos = line.find('=');
        if (eq_pos == std::string::npos)
            continue;

        std::string key = line.substr(0, eq_pos);
        key.erase(key.find_last_not_of(" \t") + 1);

        std::string value = line.substr(eq_pos + 1);
        value.erase(0, value.find_first_not_of(" \t"));

        if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
            value = value.substr(1, value.size() - 2);
        }

        result[key] = value;
    }

    return result;
}

std::vector<std::string> util::parseArgs() {
    std::istringstream iss(GetCommandLineA());
    std::vector<std::string> args;
    std::string arg;

    while (iss >> std::quoted(arg)) {
        args.push_back(arg);
    }

    return args;
}

void util::setupConsole() {
    AllocConsole();
    SetConsoleTitleA("LLBSCapture v1.0");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    FILE *file{};
    freopen_s(&file, "CONOUT$", "w+", stdout);
}
