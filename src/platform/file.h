#pragma once

#include "defines.h"

typedef struct File File;

enum class FileMode : u16 {
    FILE_MODE_READ       = 0,
    FILE_MODE_WRITE      = 1 << 0,
    FILE_MODE_APPEND     = 1 << 1,
    FILE_MODE_READ_WRITE = 1 << 2,
};

enum class FileSeekOrigin : u16 {
    FILE_SEEK_BEGIN   = 0,
    FILE_SEEK_CURRENT = 1 << 0,
    FILE_SEEK_END     = 1 << 1,
};

File* file_open(const char* path, FileMode mode);
void  file_close(File* f);
u64   file_read(File* f, void* buf, u64 size);
u64   file_write(File* f, const void* data, u64 size);
i32   file_seek(File* f, i64 offset, FileSeekOrigin origin);
u64   file_tell(const File* f);
u64   file_size(const File* f);
i32   file_is_open(const File* f);
i32   file_is_eof(const File* f);

// convenience — one-shot file I/O
i32   fs_read_file(const char* path, void** out_data, u64* out_size);
i32   fs_write_file(const char* path, const void* data, u64 size);
i32   fs_file_exists(const char* path);
u64   fs_file_size(const char* path);
i32   fs_file_delete(const char* path);
i32   fs_directory_create(const char* path);
i32   fs_directory_exists(const char* path);
