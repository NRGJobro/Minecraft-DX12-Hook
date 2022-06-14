#pragma once

#include <fstream>
#ifndef Log
#define Log(...) Logger::log(__VA_ARGS__)
#endif
class Logger {
public:
    static void log(std::string logMsg) {
        /*
        std::string path = getenv("APPDATA") + std::string("\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\MC_Dx12_Logs.txt");
        std::ofstream ofs(path.c_str(), std::ios_base::out | std::ios_base::app);
        ofs << logMsg << '\n';
        ofs.close();
        */
    };
};