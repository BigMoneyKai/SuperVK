#include "native_fsbackend.h"

#include "platform/file.h"

NativeFsBackend &default_fsbackend() {
  static NativeFsBackend s_instance;
  return s_instance;
}

b32 NativeFsBackend::exists(const FilePath &path) {
  return fs_file_exists(path.c_str()) ? SV_TRUE : SV_FALSE;
}

b32 NativeFsBackend::open(const FilePath &path, FileState *outState) {
  if (!outState) {
    return SV_FALSE;
  }
  if (fs_directory_exists(path.c_str())) {
    outState->kind = FileKind::directory;
    outState->size = 0;
    outState->modified = fs_file_modified(path.c_str());
    return SV_TRUE;
  }
  if (fs_file_exists(path.c_str())) {
    outState->kind = FileKind::file;
    outState->size = fs_file_size(path.c_str());
    outState->modified = fs_file_modified(path.c_str());
    return SV_TRUE;
  }
  return SV_FALSE;
}

FileHandle *NativeFsBackend::open(const FilePath &path, FileMode mode) {
  return file_open(path.c_str(), mode);
}

b32 NativeFsBackend::readFile(const FilePath &path, Allocator *a,
                              BufferOutputStream *out) {
  if (!out) {
    return SV_FALSE;
  }
  FileHandle *f = file_open(path.c_str(), FileMode::read);
  if (!f) {
    return SV_FALSE;
  }

  u64 size = file_size(f);
  if (size == 0) {
    file_close(f);
    return SV_TRUE;
  }

  Allocator *alloc = a ? a : &default_allocator();
  u8 *buf = static_cast<u8 *>(alloc->allocate(size));
  if (!buf) {
    file_close(f);
    return SV_FALSE;
  }

  u64 got = file_read(f, buf, size);
  file_close(f);

  if (got != size) {
    alloc->deallocate(buf);
    return SV_FALSE;
  }

  b32 ok = out->write(buf, size);
  alloc->deallocate(buf);
  return ok;
}

b32 NativeFsBackend::writeFile(const FilePath &path, const void *data,
                               u64 size) {
  return fs_write_file(path.c_str(), data, size) == 0 ? SV_TRUE : SV_FALSE;
}

b32 NativeFsBackend::deleteFile(const FilePath &path) {
  return fs_file_delete(path.c_str()) == 0 ? SV_TRUE : SV_FALSE;
}
