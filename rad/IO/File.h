#pragma once

#include <rad/Core/Platform.h>
#include <rad/Core/String.h>
#include <cstdio>

namespace rad
{

struct FileStat
{
    uint64_t  size;     // Size of the file in bytes.
    uint64_t  ctime;    // Time of creation of the file (not valid on FAT).
    uint64_t  atime;    // Time of last access to the file (not valid on FAT).
    uint64_t  mtime;    // Time of last modification to the file.
};

class File
{
public:
    File();
    ~File();

    const std::string& GetPath() const { return m_path; }
    std::FILE* GetHandle() const { return m_handle; }

    // Read = "r"/"rb"
    // Write = "w"/"wb"
    // Append = "a"/"ab"
    // ReadWrite = "w+"/"wb+R"
    // ReadWriteExist = "r+"/"rb+R"
    // ReadAppend (no overwrite to existing data) = "a+"/"ab+R"
    bool Open(std::string_view fileName, std::string_view mode);
    void Close();
    bool IsOpen();
    void Flush();

    size_t Read(void* buffer, size_t elementSize, size_t elementCount = 1);
    size_t ReadLine(void* buffer, size_t bufferSize);
    size_t ReadLine(std::string& buffer);
    size_t Write(const void* buffer, size_t elementSize, size_t elementCount = 1);

    int Print(const char* format, ...);

    int32_t GetChar();

    // Sets the file position indicator for the file stream stream to the value pointed to by offset.
    // @origin: any of RW_SEEK_SET, RW_SEEK_CUR, RW_SEEK_END;
    int64_t Seek(int64_t offset, int origin);
    int64_t Rseek(int64_t offset);
    void Rewind();
    void FastForward();

    static bool GetStat(std::string_view fileName, FileStat* pStatus);
    bool GetStat(FileStat* pStatus);
    uint64_t GetSize();
    int64_t Tell();

    static std::string ReadAll(std::string_view path);
    static std::vector<std::string> ReadLines(std::string_view path);

private:
    std::string m_path;
    std::FILE* m_handle = nullptr;

}; // class File

} // namespace rad
