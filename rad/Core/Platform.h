#pragma once

// Pre-defined C/C++ compiler macros:
// https://github.com/cpredef/predef/tree/master
// https://github.com/google/cpu_features/blob/main/include/cpu_features_macros.h

////////////////////////////////////////////////////////////////////////////////
// Architectures
////////////////////////////////////////////////////////////////////////////////

#if defined(__pnacl__) || defined(__CLR_VER)
#define RAD_ARCH_VM 1
#endif

#if (defined(_M_IX86) || defined(__i386__)) && !defined(RAD_ARCH_VM)
#define RAD_ARCH_X86_32 1
#endif

#if (defined(_M_X64) || defined(__x86_64__)) && !defined(RAD_ARCH_VM)
#define RAD_ARCH_X86_64 1
#endif

#if defined(RAD_ARCH_X86_32) || defined(RAD_ARCH_X86_64)
#define RAD_ARCH_X86 1
#endif

#if (defined(__arm__) || defined(_M_ARM))
#define RAD_ARCH_ARM 1
#endif

#if (defined(__aarch64__) || defined(_M_ARM64))
#define RAD_ARCH_AARCH64 1
#endif

#if (defined(RAD_ARCH_AARCH64) || defined(RAD_ARCH_ARM))
#define RAD_ARCH_ANY_ARM 1
#endif

#if defined(__mips64)
#define RAD_ARCH_MIPS64 1
#endif

#if defined(__mips__) && !defined(__mips64)  // mips64 also declares __mips__
#define RAD_ARCH_MIPS32 1
#endif

#if defined(RAD_ARCH_MIPS32) || defined(RAD_ARCH_MIPS64)
#define RAD_ARCH_MIPS 1
#endif

#if defined(__powerpc__)
#define RAD_ARCH_PPC 1
#endif

#if defined(__s390x__)
#define RAD_ARCH_S390X 1
#endif

#if defined(__riscv)
#define RAD_ARCH_RISCV 1
#endif

#if defined(__riscv) && defined(__riscv_xlen) && __riscv_xlen == 32
#define RAD_ARCH_RISCV32 1
#endif

#if defined(__riscv) && defined(__riscv_xlen) && __riscv_xlen == 64
#define RAD_ARCH_RISCV64 1
#endif

#if defined(__riscv) && defined(__riscv_xlen) && __riscv_xlen == 128
#define RAD_ARCH_RISCV128 1
#endif

#if defined(__loongarch64)
#define RAD_ARCH_LOONGARCH 1
#endif

////////////////////////////////////////////////////////////////////////////////
// OS
////////////////////////////////////////////////////////////////////////////////

#if (defined(__freebsd__) || defined(__FreeBSD__))
#define RAD_OS_FREEBSD 1
#endif

#if defined(__ANDROID__)
#define RAD_OS_ANDROID 1
#endif

#if defined(__linux__) && !defined(RAD_OS_FREEBSD) && !defined(RAD_OS_ANDROID)
#define RAD_OS_LINUX 1
#endif

#if (defined(_WIN32) || defined(_WIN64))
#define RAD_OS_WINDOWS 1
#include <SDKDDKVer.h>
#endif

#if (defined(__apple__) || defined(__APPLE__) || defined(__MACH__))
    #define RAD_OS_APPLE 1
    #include <TargetConditionals.h>
    // https://stackoverflow.com/a/49560690
    #if defined(TARGET_OS_OSX) && (TARGET_OS_OSX  == 1)
        #define RAD_OS_MACOS 1
    #endif
    #if defined(TARGET_OS_IPHONE) && (TARGET_OS_IPHONE == 1)
        #define RAD_OS_IPHONE 1 // for any non-Mac Apple products (IOS, TV, WATCH).
    #endif
#endif

////////////////////////////////////////////////////////////////////////////////
// Compilers
////////////////////////////////////////////////////////////////////////////////

#if defined(__clang__)
#define RAD_COMPILER_CLANG 1
#endif

#if defined(__GNUC__) && !defined(__clang__)
#define RAD_COMPILER_GCC 1
#endif

#if defined(_MSC_VER) && !defined(__clang__)
#define RAD_COMPILER_MSVC 1
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

#include <boost/version.hpp>

namespace rad
{

} // namespace rad
