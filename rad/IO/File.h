#pragma once

#include "rad/Core/Common.h"
#include "rad/Core/String.h"
#include "FileSystem.h"

NAMESPACE_RAD_BEGIN

namespace IO
{

enum FileAccessBits : uint32_t
{
    FileAccessRead = 0x00000001,
    FileAccessWrite = 0x00000002,
    FileAccessAppend = 0x00000004,
    FileAccessBinary = 0x00000008,
    FileAccessReadWrite = FileAccessRead | FileAccessWrite,
};

using FileAccessFlags = uint32_t;

class File
{
public:
    File();
    ~File();

    const Path& GetPath() const { return m_path; }
    std::FILE* GetHandle() const { return m_handle; }

    bool Open(const Path& filePath, FileAccessFlags flags);
    void Close();
    bool IsOpen();
    void Flush();

    size_t Read(void* buffer, size_t sizeInBytes, size_t elementCount = 1);
    size_t ReadLine(void* buffer, size_t bufferSize);
    size_t ReadLine(std::string& buffer);
    size_t Write(const void* buffer, size_t sizeInBytes, size_t count = 1);

    int Print(const char* format, ...);

    int32_t GetChar();

    // Sets the file position indicator for the file stream stream to the value pointed to by offset.
    // @origin: any of RW_SEEK_SET, RW_SEEK_CUR, RW_SEEK_END;
    int64_t Seek(int64_t offset, int origin);
    int64_t Rseek(int64_t offset);
    void Rewind();
    void FastForward();

    int64_t GetSize();
    int64_t Tell();

    static std::string ReadAll(const Path& path, FileAccessFlags flags = FileAccessRead);
    static std::vector<std::string> ReadLines(const Path& path);

private:
    Path m_path;
    std::FILE* m_handle = nullptr;

}; // class File

} // namespace IO

NAMESPACE_RAD_END
