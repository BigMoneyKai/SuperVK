#pragma once

#include "core/io/serializer/serializer.h"
#include "core/io/stream/istream.h"
#include "platform/file.h"

class FileInputStream : public IStream {
public:
  FileHandle *open(const char *path, FileMode mode = FileMode::write);
  u64 read(void *buf, u64 size) override;

  b32 is_open() const override;
  u64 size() const override;
  u64 position() const override;
  b32 seek(u64 offset,
           FileSeekOrigin origin = FileSeekOrigin::current) override;
  void close() override;

  template <typename T> FileInputStream &operator>>(T &val) {
    deserialize(*this, val);
    return *this;
  }

private:
  FileHandle *m_file;
};
