#pragma once

#include "defines.h"

struct FileHandle;

enum class FileMode : u16 {
  Read = 1 << 0,
  Write = 1 << 1,
  Append = 1 << 2,
  ReadWrite = (1 << 0) | (1 << 1),
};

enum class FileSeekOrigin : u16 { Begin, Current, End };

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
