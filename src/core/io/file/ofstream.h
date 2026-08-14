#pragma once

#include "core/io/serializer/serializer.h"
#include "core/io/stream/ostream.h"
#include "platform/file.h"

class FileOutputStream : public OStream {
public:
  FileHandle *open(const char *path, FileMode mode = FileMode::write);
  b32 write(const void *buf, u64 size) override;
  b32 is_open() const override;
  u64 size() const override;
  u64 position() const override;
  b32 seek(u64 offset,
           FileSeekOrigin origin = FileSeekOrigin::current) override;
  void close() override;

  template <typename T> FileOutputStream &operator<<(const T &val) {
    serialize(*this, val);
    return *this;
  }

private:
  FileHandle *m_file{nullptr};
};
