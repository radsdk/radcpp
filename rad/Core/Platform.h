#pragma once

// Pre-defined C/C++ compiler macros:
// https://github.com/cpredef/predef/tree/master

#if defined(_WIN32) || defined(_WIN64)
#define RAD_OS_WINDOWS 1
#endif

#if defined(__linux__)
#define RAD_OS_LINUX 1
#endif

#if defined(__ANDROID__)
#define RAD_OS_ANDROID 1
#endif

#if defined(__APPLE__) && defined(__MACH__)
    #define RAD_OS_APPLE 1
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE == 1
        #define RAD_OS_IOS 1
    #elif TARGET_OS_MAC == 1
        #define RAD_OS_MACOS 1
    #endif
#endif

#if defined(_MSC_VER) && !defined(__clang__) // MSVC
#define RAD_COMPILER_MSVC 1
#endif

#if defined(__GNUC__)
#define RAD_COMPILER_GCC 1
#endif

#if defined(__clang__)
#define RAD_COMPILER_CLANG 1
#endif

#if defined(__GNUC__) && defined(__GNUC_MINOR__) && defined(__GNUC_PATCHLEVEL__)
#define RAD_GNUC_PREREQ(major, minor, patch) \
            ((__GNUC__ << 20) + (__GNUC_MINOR__ << 10) + __GNUC_PATCHLEVEL__ >= \
                ((major) << 20) + ((minor) << 10) + (patch))
#elif defined(__GNUC__) && defined(__GNUC_MINOR__)
#define RAD_GNUC_PREREQ(major, minor, patch) \
            ((__GNUC__ << 20) + (__GNUC_MINOR__ << 10) >= ((major) << 20) + ((minor) << 10))
#else
#define RAD_GNUC_PREREQ(major, minor, patch) 0
#endif

#if defined(_MSC_VER) && !defined(__clang__) // MSVC
#define RAD_DEBUG_BREAK() [[unlikely]] __debugbreak()
#define RAD_ASSUME(expr) __assume(expr)
#define RAD_UNREACHABLE __assume(false)
#else
#include <signal.h>
#define RAD_DEBUG_BREAK() [[unlikely]] raise(SIGTRAP)
#define RAD_ASSUME(expr) __attribute__((assume(expr)))
#define RAD_UNREACHABLE __builtin_unreachable()
#endif

namespace rad
{

} // namespace rad
