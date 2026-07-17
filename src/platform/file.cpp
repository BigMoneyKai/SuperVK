#include "file.h"

#if defined(SV_PLATFORM_LINUX) || defined(SV_PLATFORM_APPLE)
#include <sys/stat.h>
#include <cstdio>

struct File {
    FILE* fp;
};

static const char* mode_str(FileMode m) {
    switch (m) {
        case FileMode::FILE_MODE_READ:       return "rb";
        case FileMode::FILE_MODE_WRITE:      return "wb";
        case FileMode::FILE_MODE_APPEND:     return "ab";
        case FileMode::FILE_MODE_READ_WRITE: return "w+b";
        default:                             return "rb";
    }
}

File* file_open(const char* path, FileMode mode) {
    File* f = new File{};
    f->fp   = fopen(path, mode_str(mode));
    if (!f->fp) {
        delete f;
        return nullptr;
    }
    return f;
}

void file_close(File* f) {
    if (f) {
        if (f->fp)
            fclose(f->fp);
        delete f;
    }
}

u64 file_read(File* f, void* buf, u64 size) {
    return f ? fread(buf, 1, size, f->fp) : 0;
}

u64 file_write(File* f, const void* data, u64 size) {
    return f ? fwrite(data, 1, size, f->fp) : 0;
}

i32 file_seek(File* f, i64 offset, FileSeekOrigin origin) {
    if (!f) return -1;
    i32 whence;
    switch (origin) {
        case FileSeekOrigin::FILE_SEEK_BEGIN:   whence = SEEK_SET; break;
        case FileSeekOrigin::FILE_SEEK_CURRENT: whence = SEEK_CUR; break;
        case FileSeekOrigin::FILE_SEEK_END:     whence = SEEK_END; break;
        default:                return -1;
    }
    return fseeko(f->fp, offset, whence) == 0 ? 0 : -1;
}

u64 file_tell(const File* f) {
    if (!f) return 0;
    return (u64)ftello(f->fp);
}

u64 file_size(const File* f) {
    if (!f) return 0;
    struct stat st;
    if (fstat(fileno(f->fp), &st) != 0)
        return 0;
    return (u64)st.st_size;
}

i32 file_is_open(const File* f) {
    return f && f->fp ? 1 : 0;
}

i32 file_is_eof(const File* f) {
    if (!f) return 1;
    return feof(f->fp) ? 1 : 0;
}

// ============================================================================
// convenience
// ============================================================================

i32 fs_read_file(const char* path, void** out_data, u64* out_size) {
    if (!path) { *out_data = nullptr; *out_size = 0; return -1; }

    struct stat st;
    if (stat(path, &st) != 0) { *out_data = nullptr; *out_size = 0; return -1; }

    FILE* fp = fopen(path, "rb");
    if (!fp) { *out_data = nullptr; *out_size = 0; return -1; }

    u64 sz    = (u64)st.st_size;
    void* buf = new u8[sz];
    u64 read  = fread(buf, 1, sz, fp);
    fclose(fp);

    if (read != sz) {
        delete[] (u8*)buf;
        *out_data = nullptr;
        *out_size = 0;
        return -1;
    }

    *out_data = buf;
    *out_size = sz;
    return 0;
}

i32 fs_write_file(const char* path, const void* data, u64 size) {
    if (!path || !data) return -1;
    FILE* fp = fopen(path, "wb");
    if (!fp) return -1;
    u64 written = fwrite(data, 1, size, fp);
    fclose(fp);
    return written == size ? 0 : -1;
}

i32 fs_file_exists(const char* path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISREG(st.st_mode);
}

u64 fs_file_size(const char* path) {
    struct stat st;
    if (stat(path, &st) != 0) return 0;
    return (u64)st.st_size;
}

i32 fs_file_delete(const char* path) {
    return remove(path) == 0 ? 0 : -1;
}

i32 fs_directory_create(const char* path) {
    return mkdir(path, 0755) == 0 ? 0 : -1;
}

i32 fs_directory_exists(const char* path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

#elif defined(SV_PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct File {
    HANDLE h;
};

static DWORD win_access(FileMode m) {
    switch (m) {
        case FileMode::FILE_MODE_READ:       return GENERIC_READ;
        case FileMode::FILE_MODE_WRITE:      return GENERIC_WRITE;
        case FileMode::FILE_MODE_APPEND:     return GENERIC_WRITE;
        case FileMode::FILE_MODE_READ_WRITE: return GENERIC_READ | GENERIC_WRITE;
        default:                             return GENERIC_READ;
    }
}

static DWORD win_creation(FileMode m) {
    switch (m) {
        case FileMode::FILE_MODE_READ:       return OPEN_EXISTING;
        case FileMode::FILE_MODE_WRITE:      return CREATE_ALWAYS;
        case FileMode::FILE_MODE_APPEND:     return OPEN_ALWAYS;
        case FileMode::FILE_MODE_READ_WRITE: return CREATE_ALWAYS;
        default:                             return OPEN_EXISTING;
    }
}

File* file_open(const char* path, FileMode mode) {
    File* f = new File{};
    f->h = CreateFileA(path, win_access(mode), FILE_SHARE_READ,
                       nullptr, win_creation(mode),
                       FILE_ATTRIBUTE_NORMAL, nullptr);
    if (f->h == INVALID_HANDLE_VALUE) {
        delete f;
        return nullptr;
    }
    if (mode == FileMode::FILE_MODE_APPEND)
        SetFilePoi32er(f->h, 0, nullptr, FILE_END);
    return f;
}

void file_close(File* f) {
    if (f) {
        if (f->h)
            CloseHandle(f->h);
        delete f;
    }
}

u64 file_read(File* f, void* buf, u64 size) {
    if (!f) return 0;
    DWORD read = 0;
    if (!ReadFile(f->h, buf, (DWORD)size, &read, nullptr))
        return 0;
    return read;
}

u64 file_write(File* f, const void* data, u64 size) {
    if (!f) return 0;
    DWORD written = 0;
    if (!WriteFile(f->h, data, (DWORD)size, &written, nullptr))
        return 0;
    return written;
}

i32 file_seek(File* f, i64 offset, FileSeekOrigin origin) {
    if (!f) return -1;
    DWORD method;
    switch (origin) {
        case FileSeekOrigin::FILE_SEEK_BEGIN:   method = FILE_BEGIN;   break;
        case FileSeekOrigin::FILE_SEEK_CURRENT: method = FILE_CURRENT; break;
        case FileSeekOrigin::FILE_SEEK_END:     method = FILE_END;     break;
        default:    return -1;
    }
    LARGE_i32EGER li;
    li.QuadPart = offset;
    return SetFilePoi32erEx(f->h, li, nullptr, method) ? 0 : -1;
}

u64 file_tell(const File* f) {
    if (!f) return 0;
    LARGE_i32EGER li = {}, pos = {};
    return SetFilePoi32erEx(f->h, li, &pos, FILE_CURRENT) ? (u64)pos.QuadPart : 0;
}

u64 file_size(const File* f) {
    if (!f) return 0;
    LARGE_i32EGER sz;
    return GetFileSizeEx(f->h, &sz) ? (u64)sz.QuadPart : 0;
}

i32 file_is_open(const File* f) {
    return f && f->h ? 1 : 0;
}

i32 file_is_eof(const File* f) {
    if (!f) return 1;
    LARGE_i32EGER li = {}, pos = {}, sz = {};
    if (!SetFilePoi32erEx(f->h, li, &pos, FILE_CURRENT)) return 1;
    if (!GetFileSizeEx(f->h, &sz)) return 1;
    return (u64)pos.QuadPart >= (u64)sz.QuadPart ? 1 : 0;
}

i32 fs_read_file(const char* path, void** out_data, u64* out_size) {
    *out_data = nullptr;
    *out_size = 0;
    if (!path) return -1;

    HANDLE h = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ,
                           nullptr, OPEN_EXISTING,
                           FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) return -1;

    LARGE_i32EGER li;
    if (!GetFileSizeEx(h, &li)) { CloseHandle(h); return -1; }

    u64 sz    = (u64)li.QuadPart;
    void* buf = new u8[sz];
    DWORD read = 0;
    BOOL  ok   = ReadFile(h, buf, (DWORD)sz, &read, nullptr);
    CloseHandle(h);

    if (!ok || read != sz) {
        delete[] (u8*)buf;
        return -1;
    }
    *out_data = buf;
    *out_size = sz;
    return 0;
}

i32 fs_write_file(const char* path, const void* data, u64 size) {
    HANDLE h = CreateFileA(path, GENERIC_WRITE, 0, nullptr,
                           CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) return -1;
    DWORD written = 0;
    BOOL  ok      = WriteFile(h, data, (DWORD)size, &written, nullptr);
    CloseHandle(h);
    return (ok && written == size) ? 0 : -1;
}

i32 fs_file_exists(const char* path) {
    DWORD attr = GetFileAttributesA(path);
    return (attr != INVALID_FILE_ATTRIBUTES &&
            !(attr & FILE_ATTRIBUTE_DIRECTORY)) ? 1 : 0;
}

u64 fs_file_size(const char* path) {
    WIN32_FILE_ATTRIBUTE_DATA info;
    if (!GetFileAttributesExA(path, GetFileExInfoStandard, &info))
        return 0;
    LARGE_i32EGER li;
    li.LowPart  = info.nFileSizeLow;
    li.HighPart = info.nFileSizeHigh;
    return (u64)li.QuadPart;
}

i32 fs_file_delete(const char* path) {
    return DeleteFileA(path) ? 0 : -1;
}

i32 fs_directory_create(const char* path) {
    return CreateDirectoryA(path, nullptr) ? 0 : -1;
}

i32 fs_directory_exists(const char* path) {
    DWORD attr = GetFileAttributesA(path);
    return (attr != INVALID_FILE_ATTRIBUTES &&
            (attr & FILE_ATTRIBUTE_DIRECTORY)) ? 1 : 0;
}
#endif
