#include "cache_manager.h"

CacheMan::CacheMan(const String& dir) : m_cacheDir(dir) {
    std::filesystem::create_directories(m_cacheDir.c_str());
}

b32 CacheMan::store(const String& key, std::span<const u8> data) {
    String filePath = m_cacheDir;
    filePath += "/";
    filePath += sanitize(key).c_str();

    std::ofstream file(filePath.c_str(), std::ios::binary);
    if (!file.is_open()) return false;
    file.write(reinterpret_cast<const char*>(data.data()),
               static_cast<std::streamsize>(data.size()));
    return b32(file.good());
}

b32 CacheMan::load(const String& key, Array<u8>& out) {
    String filePath = m_cacheDir;
    filePath += "/";
    filePath += sanitize(key).c_str();

    std::ifstream file(filePath.c_str(), std::ios::binary | std::ios::ate);
    if (!file.is_open()) return false;

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    out.resize(static_cast<u64>(size));
    file.read(reinterpret_cast<char*>(out.data()), size);
    return b32(file.good());
}

b32 CacheMan::exists(const String& key) {
    String filePath = m_cacheDir;
    filePath += "/";
    filePath += sanitize(key).c_str();
    return b32(std::filesystem::exists(filePath.c_str()));
}
