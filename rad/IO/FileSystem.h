#pragma once

#include "rad/Core/Common.h"
#include "rad/Core/String.h"
#include <filesystem>

NAMESPACE_RAD_BEGIN

namespace IO
{

using FileSystemError = std::filesystem::filesystem_error;

using Path = std::filesystem::path;

using DirectoryEntry = std::filesystem::directory_entry;
using DirectoryIterator = std::filesystem::directory_iterator;
using RecursiveDirectorIterator = std::filesystem::recursive_directory_iterator;
using DirectoryOptions = std::filesystem::directory_options;

using FileType = std::filesystem::file_type;
using FileStatus = std::filesystem::file_status;
using FileTime = std::filesystem::file_time_type;

using SpaceInfo = std::filesystem::space_info;
using Permissions = std::filesystem::perms;
using PermissionOptions = std::filesystem::perm_options;
using CopyOptions = std::filesystem::copy_options;

Path GetCurrentPath();
Path GetWorkingDirectory();
void SetCurrentPath(const Path& p);
Path GetTempDirectory();

Path MakePathAbsolute(const Path& p);
Path MakePathRelative(const Path& p, const Path& base);
Path MakePathProximate(const Path& p, const Path& base);
Path MakePathCanonical(const Path& p);
Path MakePathWeaklyCanonical(const Path& p);

bool Exists(const Path& p);
bool IsPathEquivalent(const Path& p1, const Path& p2);

std::uintmax_t GetFileSize(const Path& p);
FileStatus GetFileStatus(const Path& p);
FileStatus GetSymlinkStatus(const Path& p);
bool Exists(FileStatus s);
std::uintmax_t GetHardLinkCount(const Path& p);
FileTime GetLastWriteTime(const Path& p);
void SetLastWriteTime(const Path& p, FileTime t);

void SetPermissions(const Path& p, Permissions permissions,
    PermissionOptions options = PermissionOptions::replace);

Path ReadSymlink(const Path& p);

bool CreateDirectory(const Path& p);
bool CreateDirectory(const Path& p, const Path& copyAttribs);
bool CreateDirectories(const Path& p);

void CreateHardLink(const Path& target, const Path& link);
void CreateSymlink(const Path& target, const Path& link);
void CreateDirectorySymlink(const Path& target, const Path& link);

void Copy(const Path& from, const Path& to, CopyOptions options = CopyOptions::none);
void CopyFile(const Path& from, const Path& to, CopyOptions options = CopyOptions::none);
void CopySymlink(const Path& from, const Path& to);

// Delete file or empty directory, the same as POSIX remove.
// Symlinks are not followed (symlink is removed, not its target).
// Return true if the file was deleted, false if it did not exist.
bool Remove(const Path& p);

// Delete file or directory with all its subdirectories, recursively.
// Symlinks are not followed (symlink is removed, not its target).
// Return the number of files and directories that were deleted.
std::uintmax_t RemoveAll(const Path& p);

void Rename(const Path& oldPath, const Path& newPath);

void ResizeFile(const Path& p, std::uintmax_t newSize);

// Return the space info, the same as POSIX statvfs.
SpaceInfo GetSpaceInfo(const Path& p);

std::vector<Path> GetRootDirectories();
Path GetCurrentProcessPath();

// The string length cannot exceeds 1024.
std::string GetPathTimeString(const Path& path, std::string_view format = "%F %T");

} // namespace IO

NAMESPACE_RAD_END
