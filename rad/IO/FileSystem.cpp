#include <rad/IO/FileSystem.h>
#include <cerrno>
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
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace rad
{

void Swap(FilePath& lhs, FilePath& rhs)
{
    std::filesystem::swap(lhs, rhs);
}

size_t Hash(const FilePath& p)
{
    return std::filesystem::hash_value(p);
}

FilePath GetCurrentPath()
{
    return std::filesystem::current_path();
}

FilePath GetWorkingDirectory()
{
    return std::filesystem::current_path();
}

void SetCurrentPath(const FilePath& p)
{
    std::filesystem::current_path(p);
}

FilePath GetTempDirectory()
{
    return std::filesystem::temp_directory_path();
}

FilePath MakeAbsolute(const FilePath& p)
{
    return std::filesystem::absolute(p);
}

FilePath MakeRelative(const FilePath& p, const FilePath& base)
{
    return std::filesystem::relative(p, base);
}

FilePath MakeProximate(const FilePath& p, const FilePath& base)
{
    return std::filesystem::proximate(p, base);
}

FilePath MakeCanonical(const FilePath& p)
{
    return std::filesystem::canonical(p);
}

FilePath MakeWeaklyCanonical(const FilePath& p)
{
    return std::filesystem::weakly_canonical(p);
}

bool Exists(const FilePath& p)
{
    return std::filesystem::exists(p);
}

bool IsEquivalent(const FilePath& p1, const FilePath& p2)
{
    return std::filesystem::equivalent(p1, p2);
}

std::uintmax_t GetFileSize(const FilePath& p)
{
    return std::filesystem::file_size(p);
}

FileStatus GetFileStatus(const FilePath& p)
{
    return std::filesystem::status(p);
}

FileStatus GetSymlinkStatus(const FilePath& p)
{
    return std::filesystem::symlink_status(p);
}

bool Exists(FileStatus s)
{
    return std::filesystem::exists(s);
}

std::uintmax_t GetHardLinkCount(const FilePath& p)
{
    return std::filesystem::hard_link_count(p);
}

FileTime GetLastWriteTime(const FilePath& p)
{
    return std::filesystem::last_write_time(p);
}

void SetLastWriteTime(const FilePath& p, FileTime t)
{
    std::filesystem::last_write_time(p, t);
}

void SetPermissions(const FilePath& p, FilePerms permissions, FilePermOptions options)
{
    std::filesystem::permissions(p, permissions, options);
}

FilePath ReadSymlink(const FilePath& p)
{
    return std::filesystem::read_symlink(p);
}

bool CreateDirectory(const FilePath& p)
{
    return std::filesystem::create_directory(p);
}

bool CreateDirectory(const FilePath& p, const FilePath& copyAttribs)
{
    return std::filesystem::create_directory(p, copyAttribs);
}

bool CreateDirectories(const FilePath& p)
{
    return std::filesystem::create_directories(p);
}

void CreateHardLink(const FilePath& target, const FilePath& link)
{
    return std::filesystem::create_hard_link(target, link);
}

void CreateSymlink(const FilePath& target, const FilePath& link)
{
    return std::filesystem::create_symlink(target, link);
}

void CreateDirectorySymlink(const FilePath& target, const FilePath& link)
{
    return std::filesystem::create_directory_symlink(target, link);
}

void Copy(const FilePath& from, const FilePath& to, FileCopyOptions options)
{
    std::filesystem::copy(from, to, options);
}

void CopyFile(const FilePath& from, const FilePath& to, FileCopyOptions options)
{
    std::filesystem::copy_file(from, to, options);
}

void CopySymlink(const FilePath& from, const FilePath& to)
{
    std::filesystem::copy_symlink(from, to);
}

bool Remove(const FilePath& p)
{
    return std::filesystem::remove(p);
}

std::uintmax_t RemoveAll(const FilePath& p)
{
    return std::filesystem::remove_all(p);
}

void Rename(const FilePath& oldPath, const FilePath& newPath)
{
    std::filesystem::rename(oldPath, newPath);
}

void ResizeFile(const FilePath& p, std::uintmax_t newSize)
{
    std::filesystem::resize_file(p, newSize);
}

// Return the space info, the same as POSIX statvfs.
FileSpaceInfo GetSpaceInfo(const FilePath& p)
{
    return std::filesystem::space(p);
}

bool IsBlockFile(const FilePath& p)
{
    return std::filesystem::is_block_file(p);
}

bool IsCharacterFile(const FilePath& p)
{
    return std::filesystem::is_character_file(p);
}

bool IsDirectory(const FilePath& p)
{
    return std::filesystem::is_directory(p);
}

bool IsEmpty(const FilePath& p)
{
    return std::filesystem::is_empty(p);
}

bool IsFIFO(const FilePath& p)
{
    return std::filesystem::is_fifo(p);
}

bool IsOther(const FilePath& p)
{
    return std::filesystem::is_other(p);
}

bool IsRegularFile(const FilePath& p)
{
    return std::filesystem::is_regular_file(p);
}

bool IsSocket(const FilePath& p)
{
    return std::filesystem::is_socket(p);
}

bool IsSymlink(const FilePath& p)
{
    return std::filesystem::is_symlink(p);
}

bool IsStatusKnown(const FileStatus& s)
{
    return std::filesystem::status_known(s);
}

std::string GetFilePathTimeString(const FilePath& path, std::string_view format)
{
    std::tm dateTime = {};

#if defined(RAD_OS_WINDOWS)
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
    if (bytesWritten != 0)
    {
        buffer.resize(bytesWritten);
    }
    else
    {
        buffer.clear();
    }
    return buffer;
}

} // namespace rad
