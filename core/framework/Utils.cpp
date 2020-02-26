#include "Utils.h"
#include "Engine.h"

#include <fstream>
#include <vector>

std::string Utils::readFile(const std::string &path) {
    std::ifstream ifs(path.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
    std::ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    std::vector<char> bytes(fileSize);
    ifs.read(&bytes[0], fileSize);
    return std::string(&bytes[0], fileSize);
}

std::string Utils::getPathToCore() {
    std::string path_to_core;
    std::string path_to_exe = Engine::get()->getExecutablePath();
    int proj_name_index = path_to_exe.find("Aetna");
    if (proj_name_index >= 0) {
        path_to_core = path_to_exe.substr(0, proj_name_index + strlen("Aetna") + 1) + "core\\";
    }
    return path_to_core;
}
