//
// Created by user on 15.05.2025.
//

#ifndef UMPDC_UTIL_H
#define UMPDC_UTIL_H


#include <string>
#include <unordered_map>

class util {
public:
    static bool spawnDetachedProcess(const std::string &cwd, const std::string& exePath, const std::string& args);
    static std::string executePowerShell(const std::string &cmd);
    static std::unordered_map<std::string, std::string> parseConfig(const std::string &data);
    static std::vector<std::string> parseArgs();
    static void setupConsole();
};


#endif //UMPDC_UTIL_H
