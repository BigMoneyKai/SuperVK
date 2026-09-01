#pragma once

#include "core/define/types.h"

namespace Platform {
struct FileHandle;

enum class FileMode : u16 {
  read = 1 << 0,
  write = 1 << 1,
  append = 1 << 2,
  readWrite = (1 << 0) | (1 << 1),
};

enum class FileSeekOrigin : u16 { begin, current, end };

FileHandle *file_open(const char *path, FileMode mode);
void file_close(FileHandle *f);
u64 file_read(FileHandle *f, void *buf, u64 size);
u64 file_write(FileHandle *f, const void *data, u64 size);
i32 file_seek(FileHandle *f, i64 offset, FileSeekOrigin origin);
u64 file_tell(const FileHandle *f);
u64 file_size(const FileHandle *f);
i32 file_is_open(const FileHandle *f);
i32 file_is_eof(const FileHandle *f);

// convenience — one-shot file I/O
i32 fs_read_file(const char *path, void **out_data, u64 *out_size);
i32 fs_write_file(const char *path, const void *data, u64 size);
i32 fs_file_exists(const char *path);
u64 fs_file_size(const char *path);
u64 fs_file_modified(const char *path);
i32 fs_file_delete(const char *path);
i32 fs_directory_create(const char *path);
i32 fs_directory_exists(const char *path);
} // namespace Platform

using FileHandle = Platform::FileHandle;
using FileMode = Platform::FileMode;
using FileSeekOrigin = Platform::FileSeekOrigin;
using Platform::file_open;
using Platform::file_close;
using Platform::file_read;
using Platform::file_write;
using Platform::file_seek;
using Platform::file_tell;
using Platform::file_size;
using Platform::file_is_open;
using Platform::file_is_eof;
using Platform::fs_read_file;
using Platform::fs_write_file;
using Platform::fs_file_exists;
using Platform::fs_file_size;
using Platform::fs_file_modified;
using Platform::fs_file_delete;
using Platform::fs_directory_create;
using Platform::fs_directory_exists;
