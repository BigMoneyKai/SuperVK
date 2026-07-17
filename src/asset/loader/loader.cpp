#include "asset/loader/loader.h"

#include <fstream>
#include <sstream>
#include <cstring>

b32 AssetLoader::loadFile(const String& path, Array<u8>& output) {
    std::ifstream file(path.c_str(), std::ios::binary | std::ios::ate);
    if (!file.is_open()) return false;
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    output.resize(static_cast<u64>(size));
    file.read(reinterpret_cast<char*>(output.data()), size);
    return b32(file.good());
}

b32 AssetLoader::loadText(const String& path, String& output) {
    std::ifstream file(path.c_str());
    if (!file.is_open()) return false;
    std::stringstream ss;
    ss << file.rdbuf();
    output = ss.str().c_str();
    return b32(!output.empty());
}

b32 AssetLoader::saveFile(const String& path, std::span<const u8> data) {
    std::ofstream file(path.c_str(), std::ios::binary);
    if (!file.is_open()) return false;
    file.write(reinterpret_cast<const char*>(data.data()),
               static_cast<std::streamsize>(data.size()));
    return b32(file.good());
}
