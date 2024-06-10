#include "OS.h"
#if defined(RAD_OS_WINDOWS)
#include <Windows.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace rad
{

boost::process::native_environment g_environ = boost::this_process::environment();


std::vector<std::string> get_exec_path()
{
    return g_environ.find("Path")->to_vector();
}

uint32_t getpid()
{
#if defined(RAD_OS_WINDOWS)
    return ::GetCurrentProcessId();
#else
    return ::getpid();
#endif
}

FilePath GetCurrentProcessPath()
{
#if defined(RAD_OS_WINDOWS)
    std::wstring buffer;
    buffer.resize(256);
    do {
        unsigned int len = ::GetModuleFileNameW(NULL, &buffer[0], static_cast<DWORD>(buffer.size()));
        if (len < buffer.size())
        {
            buffer.resize(len);
            break;
        }
        buffer.resize(buffer.size() * 2);
    } while (buffer.size() < 65536);
    return FilePath(buffer);
#else
    std::error_code ec;
    if (std::filesystem::exists("/proc/self/exe", ec))
    {
        return std::filesystem::read_symlink("/proc/self/exe", ec);
    }
    if (std::filesystem::exists("/proc/curproc/file", ec))
    {
        return std::filesystem::read_symlink("/proc/curproc/file", ec);
    }
    if (std::filesystem::exists("/proc/curproc/exe", ec))
    {
        return std::filesystem::read_symlink("/proc/curproc/exe", ec);
    }
    return std::to_string(getpid());
#endif
}

std::string GetCurrentProcessName()
{
    return (const char*)GetCurrentProcessPath().filename().u8string().c_str();
}

std::vector<FilePath> listdrives()
{
    std::vector<FilePath> drives;
#if defined(RAD_OS_WINDOWS)
    DWORD mask = ::GetLogicalDrives();
    std::string drive = "A:\\";
    for (DWORD i = 0; i < sizeof(DWORD) * 8; i++)
    {
        if (mask & (1 << i))
        {
            drive[0] = 'A' + char(i);
            drives.push_back(drive);
        }
    }
#else
    drives.push_back(FilePath("/"));
#endif
    return drives;
}

bool SetConsoleOutputEncodingUTF8()
{
#if defined(RAD_OS_WINDOWS)
    return ::SetConsoleOutputCP(65001);
#else
    return true;
#endif
}

} // namespace rad
