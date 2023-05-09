#include "FileSystem.h"

#include <ctime>

#ifdef _WIN32

#include <Windows.h>

#ifdef CreateDirectory
#undef CreateDirectory
#endif
#ifdef CreateHardLink
#undef CreateHardLink
#endif
#ifdef CopyFile
#undef CopyFile
#endif

#else // Linux/Unix

#include <sys/stat.h>
#include <unistd.h>

#endif

NAMESPACE_RAD_BEGIN

namespace IO
{

Path GetCurrentPath()
{
    return std::filesystem::current_path();
}

Path GetWorkingDirectory()
{
    return std::filesystem::current_path();
}

void SetCurrentPath(const Path& p)
{
    std::filesystem::current_path(p);
}

Path GetTempDirectory()
{
    return std::filesystem::temp_directory_path();
}

Path MakePathAbsolute(const Path& p)
{
    return std::filesystem::absolute(p);
}

Path MakePathRelative(const Path& p, const Path& base)
{
    return std::filesystem::relative(p, base);
}

Path MakePathProximate(const Path& p, const Path& base)
{
    return std::filesystem::proximate(p, base);
}

Path MakePathCanonical(const Path& p)
{
    return std::filesystem::canonical(p);
}

Path MakePathWeaklyCanonical(const Path& p)
{
    return std::filesystem::weakly_canonical(p);
}

bool Exists(const Path& p)
{
    return std::filesystem::exists(p);
}

bool IsPathEquivalent(const Path& p1, const Path& p2)
{
    return std::filesystem::equivalent(p1, p2);
}

std::uintmax_t GetFileSize(const Path& p)
{
    return std::filesystem::file_size(p);
}

FileStatus GetFileStatus(const Path& p)
{
    return std::filesystem::status(p);
}

FileStatus GetSymlinkStatus(const Path& p)
{
    return std::filesystem::symlink_status(p);
}

bool Exists(FileStatus s)
{
    return std::filesystem::exists(s);
}

std::uintmax_t GetHardLinkCount(const Path& p)
{
    return std::filesystem::hard_link_count(p);
}

FileTime GetLastWriteTime(const Path& p)
{
    return std::filesystem::last_write_time(p);
}

void SetLastWriteTime(const Path& p, FileTime t)
{
    std::filesystem::last_write_time(p, t);
}

void SetPermissions(const Path& p, Permissions permissions, PermissionOptions options)
{
    std::filesystem::permissions(p, permissions, options);
}

Path ReadSymlink(const Path& p)
{
    return std::filesystem::read_symlink(p);
}

bool CreateDirectory(const Path& p)
{
    return std::filesystem::create_directory(p);
}

bool CreateDirectory(const Path& p, const Path& copyAttribs)
{
    return std::filesystem::create_directory(p, copyAttribs);
}

bool CreateDirectories(const Path& p)
{
    return std::filesystem::create_directories(p);
}

void CreateHardLink(const Path& target, const Path& link)
{
    return std::filesystem::create_hard_link(target, link);
}

void CreateSymlink(const Path& target, const Path& link)
{
    return std::filesystem::create_symlink(target, link);
}

void CreateDirectorySymlink(const Path& target, const Path& link)
{
    return std::filesystem::create_directory_symlink(target, link);
}

void Copy(const Path& from, const Path& to, CopyOptions options)
{
    std::filesystem::copy(from, to, options);
}

void CopyFile(const Path& from, const Path& to, CopyOptions options)
{
    std::filesystem::copy_file(from, to, options);
}

void CopySymlink(const Path& from, const Path& to)
{
    std::filesystem::copy_symlink(from, to);
}

// Delete file or empty directory, the same as POSIX remove.
// Symlinks are not followed (symlink is removed, not its target).
// Return true if the file was deleted, false if it did not exist.
bool Remove(const Path& p)
{
    return std::filesystem::remove(p);
}

// Delete file or directory with all its subdirectories, recursively.
// Symlinks are not followed (symlink is removed, not its target).
// Return the number of files and directories that were deleted.
std::uintmax_t RemoveAll(const Path& p)
{
    return std::filesystem::remove_all(p);
}

void Rename(const Path& oldPath, const Path& newPath)
{
    std::filesystem::rename(oldPath, newPath);
}

void ResizeFile(const Path& p, std::uintmax_t newSize)
{
    std::filesystem::resize_file(p, newSize);
}

// Return the space info, the same as POSIX statvfs.
SpaceInfo GetSpaceInfo(const Path& p)
{
    return std::filesystem::space(p);
}

std::vector<Path> GetRootDirectories()
{
    std::vector<Path> rootDirectories;
#ifdef _WIN32
    DWORD logicalDrivesBitmask = ::GetLogicalDrives();
    std::string rootDirectory = "A:\\";
    for (DWORD i = 0; i < sizeof(DWORD) * 8; i++)
    {
        if (logicalDrivesBitmask & (1 << i))
        {
            rootDirectory[0] = 'A' + char(i);
            rootDirectories.push_back(rootDirectory);
        }
    }
#else
    rootDirectories.push_back(Path("/"));
#endif
    return rootDirectories;
}

Path GetCurrentProcessPath()
{
#ifdef _WIN32
    std::wstring buffer;
    buffer.resize(1024);
    do
    {
        unsigned int len = GetModuleFileNameW(NULL, &buffer[0], static_cast<DWORD>(buffer.size()));
        if (len < buffer.size())
        {
            buffer.resize(len);
            break;
        }

        buffer.resize(buffer.size() * 2);
    } while (buffer.size() < 65536);

    return Path(buffer);

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

std::string GetPathTimeString(const Path& path, std::string_view format)
{
    std::tm dateTime = {};

#ifdef _WIN32
    struct __stat64 fileStatus = {};
    _wstat64(path.c_str(), &fileStatus);
    localtime_s(&dateTime, &fileStatus.st_mtime);
#else
    struct stat fileStatus = {};
    stat(path.c_str(), &fileStatus);
    localtime_r(&fileStatus.st_mtime, &dateTime);
#endif
    std::string buffer(128, 0);
    size_t bytesWritten = strftime(buffer.data(), buffer.size(), format.data(), &dateTime);
    if (bytesWritten == 0)
    {
        buffer.resize(1024);
        bytesWritten = strftime(buffer.data(), buffer.size(), format.data(), &dateTime);
    }
    return buffer;
}

} // namespace IO

NAMESPACE_RAD_END
