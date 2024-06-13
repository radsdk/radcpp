#include <rad/IO/File.h>
#include <cassert>
#include <cerrno>
#include <cstdarg>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>

namespace rad
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

bool File::Open(std::string_view fileName, std::string_view mode)
{
#if defined(RAD_OS_WINDOWS)
    errno_t err = fopen_s(&m_handle, fileName.data(), mode.data());
#else
    m_handle = fopen(fileName.data(), mode.data());
    error_t err = 0;
    if (m_handle == nullptr)
    {
        err = errno;
    }
#endif
    if ((err == 0) && (m_handle != nullptr))
    {
        m_path = fileName;
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

size_t File::Read(void* buffer, size_t elementSize, size_t elementCount)
{
    return fread(buffer, elementSize, elementCount, m_handle);
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

size_t File::Write(const void* buffer, size_t elementSize, size_t elementCount)
{
    return fwrite(buffer, elementSize, elementCount, m_handle);
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

bool File::GetStat(std::string_view fileName, FileStat* pStatus)
{
    assert(pStatus != nullptr);
#if defined(RAD_OS_WINDOWS)
    struct _stat64 status {};
    const int ret = _stat64(fileName.data(), &status);
#else
    struct stat64 status {};
    const int ret = stat64(fileName.data(), &status);
#endif
    if (ret != 0)
    {
        return false;
    }

    static_assert(sizeof(FileStat::size) == sizeof(status.st_size));

    pStatus->size = static_cast<decltype(FileStat::size)>(status.st_size);
    pStatus->ctime = static_cast<decltype(FileStat::ctime)>(status.st_ctime);
    pStatus->atime = static_cast<decltype(FileStat::atime)>(status.st_atime);
    pStatus->mtime = static_cast<decltype(FileStat::mtime)>(status.st_mtime);

    return true;
}

bool File::GetStat(FileStat* pStatus)
{
    return GetStat(m_path, pStatus);
}

uint64_t File::GetSize()
{
    FileStat status = {};
    if (GetStat(&status))
    {
        return status.size;
    }
    return 0;
}

int64_t File::Tell()
{
    return ftell(m_handle);
}

std::string File::ReadAll(std::string_view path)
{
    File file;
    std::string buffer;
    if (file.Open(path, "rb"))
    {
        int64_t fileSize = file.GetSize();
        buffer.resize(fileSize);
        file.Read(buffer.data(), fileSize);
        return buffer;
    }
    return buffer;
}

std::vector<std::string> File::ReadLines(std::string_view path)
{
    File file;
    std::vector<std::string> lines;
    if (file.Open(path, "r"))
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

} // namespace rad
