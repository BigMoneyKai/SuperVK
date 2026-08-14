#include "ofstream.h"
#include "platform/file.h"

FileHandle* FileOutputStream::open(const char* path, FileMode mode) {
    m_file = file_open(path, mode);
    return m_file;
}

b32 FileOutputStream::write(const void* buf, u64 size) {
    return file_write(m_file, buf, size);
}

b32 FileOutputStream::is_open() const {
    return file_is_open(m_file);
}

u64 FileOutputStream::size() const {
    return file_size(m_file);
}

u64 FileOutputStream::position() const {
    return file_tell(m_file);
}

b32 FileOutputStream::seek(u64 offset, FileSeekOrigin origin) {
    return file_seek(m_file, static_cast<i64>(offset), origin) == 0 ? SV_TRUE : SV_FALSE;
}

void FileOutputStream::close() {
    if (m_file) {
        file_close(m_file);
        m_file = nullptr;
    }
}
