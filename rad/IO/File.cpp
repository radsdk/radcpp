#include "File.h"
#include <cstdarg>
#include <errno.h>

NAMESPACE_RAD_BEGIN

namespace IO
{

File::File()
{
}

File::~File()
{
    if (m_handle)
    {
        Close();
    }
}

bool File::Open(const Path& filePath, FileAccessFlags flags)
{
    const char* mode = "r";
    switch (flags)
    {
    case FileAccessRead:
        mode = "r";
        break;
    case FileAccessWrite:
        mode = "w";
        break;
    case FileAccessAppend:
        mode = "a";
        break;
    case (FileAccessRead | FileAccessAppend):
        mode = "a+";
        break;
    case (FileAccessRead | FileAccessWrite):
        mode = "w+";
        break;
    case (FileAccessRead | FileAccessBinary):
        mode = "rb";
        break;
    case (FileAccessWrite | FileAccessBinary):
        mode = "wb";
        break;
    case (FileAccessAppend | FileAccessBinary):
        mode = "ab";
        break;
    case (FileAccessRead | FileAccessAppend | FileAccessBinary):
        mode = "ab+";
        break;
    case (FileAccessRead | FileAccessWrite | FileAccessBinary):
        mode = "wb+";
        break;
    }

#if defined(_WIN32) || defined(_WIN64)
    errno_t err = fopen_s(&m_handle, (const char*)filePath.u8string().c_str(), mode);
#else
    m_handle = fopen((const char*)filePath.u8string().c_str(), mode);
    error_t err = 0;
    if (m_handle == nullptr)
    {
        err = errno;
    }
#endif
    if ((err == 0) && (m_handle != nullptr))
    {
        m_path = filePath;
        return true;
    }
    else
    {
        m_handle = nullptr;
        return false;
    }
}

void File::Close()
{
    fclose(m_handle);
    m_handle = nullptr;
}

bool File::IsOpen()
{
    return (m_handle != nullptr);
}

void File::Flush()
{
    fflush(m_handle);
}

size_t File::Read(void* buffer, size_t sizeInBytes, size_t elementCount)
{
    return fread(buffer, sizeInBytes, elementCount, m_handle);
}

size_t File::ReadLine(void* buffer, size_t bufferSize)
{
    size_t bytesRead = 0;
    char* str = static_cast<char*>(buffer);
    while (bytesRead < bufferSize)
    {
        int32_t c = GetChar();
        if ((c == '\n') || (c == EOF))
        {
            break;
        }
        str[bytesRead] = static_cast<char>(c);
        bytesRead++;
    }

    const size_t endIndex = ((bytesRead < bufferSize) ? bytesRead : (bufferSize - 1));
    str[endIndex] = '\0';

    return bytesRead;
}

size_t File::ReadLine(std::string& buffer)
{
    size_t bytesRead = 0;
    while (true)
    {
        int32_t c = GetChar();
        if ((c == '\n') || (c == EOF))
        {
            break;
        }
        buffer.push_back(static_cast<char>(c));
        bytesRead++;
    }
    return bytesRead;
}

size_t File::Write(const void* buffer, size_t sizeInBytes, size_t count)
{
    return fwrite(buffer, sizeInBytes, count, m_handle);
}

int File::Print(const char* format, ...)
{
    int ret = 0;
    va_list args;
    va_start(args, format);
    ret = vfprintf(m_handle, format, args);
    va_end(args);
    return ret;
}

int32_t File::GetChar()
{
    return std::getc(m_handle);
}

int64_t File::Seek(int64_t offset, int origin)
{
    return fseek(m_handle, static_cast<long>(offset), origin);
}

int64_t File::Rseek(int64_t offset)
{
    return fseek(m_handle, static_cast<long>(offset), SEEK_END);
}

void File::Rewind()
{
    std::rewind(m_handle);
}

void File::FastForward()
{
    Rseek(0);
}

int64_t File::GetSize()
{
    return static_cast<uint64_t>(GetFileSize(m_path));
}

int64_t File::Tell()
{
    return ftell(m_handle);
}

std::string File::ReadAll(const Path& path, FileAccessFlags flags)
{
    File file;
    std::string buffer;
    if (file.Open(path, flags))
    {
        int64_t fileSize = file.GetSize();
        buffer.resize(fileSize);
        file.Read(buffer.data(), fileSize);
        return buffer;
    }
    return buffer;
}

std::vector<std::string> File::ReadLines(const Path& path)
{
    File file;
    std::vector<std::string> lines;
    if (file.Open(path, FileAccessRead))
    {
        std::string line;
        while (true)
        {
            int32_t c = file.GetChar();
            if ((c != '\n') && (c != EOF))
            {
                line.push_back(static_cast<char>(c));
            }
            else
            {
                lines.push_back(line);
                line.clear();
                if (c == EOF)
                {
                    break;
                }
            }
        }
    }
    return lines;
}

} // namespace IO

NAMESPACE_RAD_END
