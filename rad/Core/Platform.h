#pragma once

// Pre-defined C/C++ compiler macros:
// https://github.com/cpredef/predef/tree/master
// https://github.com/google/cpu_features/blob/main/include/cpu_features_macros.h

////////////////////////////////////////////////////////////////////////////////
// Architectures
////////////////////////////////////////////////////////////////////////////////

#if defined(__pnacl__) || defined(__CLR_VER)
#define RAD_ARCH_VM
#endif

#if (defined(_M_IX86) || defined(__i386__)) && !defined(RAD_ARCH_VM)
#define RAD_ARCH_X86_32
#endif

#if (defined(_M_X64) || defined(__x86_64__)) && !defined(RAD_ARCH_VM)
#define RAD_ARCH_X86_64
#endif

#if defined(RAD_ARCH_X86_32) || defined(RAD_ARCH_X86_64)
#define RAD_ARCH_X86
#endif

#if (defined(__arm__) || defined(_M_ARM))
#define RAD_ARCH_ARM
#endif

#if (defined(__aarch64__) || defined(_M_ARM64))
#define RAD_ARCH_AARCH64
#endif

#if (defined(RAD_ARCH_AARCH64) || defined(RAD_ARCH_ARM))
#define RAD_ARCH_ANY_ARM
#endif

#if defined(__mips64)
#define RAD_ARCH_MIPS64
#endif

#if defined(__mips__) && !defined(__mips64)  // mips64 also declares __mips__
#define RAD_ARCH_MIPS32
#endif

#if defined(RAD_ARCH_MIPS32) || defined(RAD_ARCH_MIPS64)
#define RAD_ARCH_MIPS
#endif

#if defined(__powerpc__)
#define RAD_ARCH_PPC
#endif

#if defined(__s390x__)
#define RAD_ARCH_S390X
#endif

#if defined(__riscv)
#define RAD_ARCH_RISCV
#endif

#if defined(__riscv) && defined(__riscv_xlen) && __riscv_xlen == 32
#define RAD_ARCH_RISCV32
#endif

#if defined(__riscv) && defined(__riscv_xlen) && __riscv_xlen == 64
#define RAD_ARCH_RISCV64
#endif

#if defined(__riscv) && defined(__riscv_xlen) && __riscv_xlen == 128
#define RAD_ARCH_RISCV128
#endif

#if defined(__loongarch64)
#define RAD_ARCH_LOONGARCH
#endif

////////////////////////////////////////////////////////////////////////////////
// OS
////////////////////////////////////////////////////////////////////////////////

#if (defined(__freebsd__) || defined(__FreeBSD__))
#define RAD_OS_FREEBSD
#endif

#if defined(__ANDROID__)
#define RAD_OS_ANDROID
#endif

#if defined(__linux__) && !defined(RAD_OS_FREEBSD) && \
    !defined(RAD_OS_ANDROID)
#define RAD_OS_LINUX
#endif

#if (defined(_WIN64) || defined(_WIN32))
#define RAD_OS_WINDOWS
#endif

#if (defined(__apple__) || defined(__APPLE__) || defined(__MACH__))
// From https://stackoverflow.com/a/49560690
#include "TargetConditionals.h"
#if defined(TARGET_OS_OSX)
#define RAD_OS_MACOS
#endif
#if defined(TARGET_OS_IPHONE)
// This is set for any non-Mac Apple products (IOS, TV, WATCH)
#define RAD_OS_IPHONE
#endif
#endif

////////////////////////////////////////////////////////////////////////////////
// Compilers
////////////////////////////////////////////////////////////////////////////////

#if defined(__clang__)
#define RAD_COMPILER_CLANG
#endif

#if defined(__GNUC__) && !defined(__clang__)
#define RAD_COMPILER_GCC
#endif

// https://learn.microsoft.com/en-us/cpp/overview/compiler-versions?view=msvc-170
#if defined(_MSC_VER) || defined(_MSC_FULL_VER)
#define RAD_COMPILER_MSVC
#endif

////////////////////////////////////////////////////////////////////////////////
// Compiler flags
////////////////////////////////////////////////////////////////////////////////

// Check if a feature is known to be available at compile time.

#if defined(RAD_ARCH_X86)

#if defined(__AES__)
#define RAD_COMPILED_X86_AES 1
#else
#define RAD_COMPILED_X86_AES 0
#endif  //  defined(__AES__)

#if defined(__F16C__)
#define RAD_COMPILED_X86_F16C 1
#else
#define RAD_COMPILED_X86_F16C 0
#endif  //  defined(__F16C__)

#if defined(__BMI__)
#define RAD_COMPILED_X86_BMI 1
#else
#define RAD_COMPILED_X86_BMI 0
#endif  //  defined(__BMI__)

#if defined(__BMI2__)
#define RAD_COMPILED_X86_BMI2 1
#else
#define RAD_COMPILED_X86_BMI2 0
#endif  //  defined(__BMI2__)

#if (defined(__SSE__) || (_M_IX86_FP >= 1))
#define RAD_COMPILED_X86_SSE 1
#else
#define RAD_COMPILED_X86_SSE 0
#endif

#if (defined(__SSE2__) || (_M_IX86_FP >= 2))
#define RAD_COMPILED_X86_SSE2 1
#else
#define RAD_COMPILED_X86_SSE2 0
#endif

#if defined(__SSE3__)
#define RAD_COMPILED_X86_SSE3 1
#else
#define RAD_COMPILED_X86_SSE3 0
#endif  //  defined(__SSE3__)

#if defined(__SSSE3__)
#define RAD_COMPILED_X86_SSSE3 1
#else
#define RAD_COMPILED_X86_SSSE3 0
#endif  //  defined(__SSSE3__)

#if defined(__SSE4_1__)
#define RAD_COMPILED_X86_SSE4_1 1
#else
#define RAD_COMPILED_X86_SSE4_1 0
#endif  //  defined(__SSE4_1__)

#if defined(__SSE4_2__)
#define RAD_COMPILED_X86_SSE4_2 1
#else
#define RAD_COMPILED_X86_SSE4_2 0
#endif  //  defined(__SSE4_2__)

#if defined(__AVX__)
#define RAD_COMPILED_X86_AVX 1
#else
#define RAD_COMPILED_X86_AVX 0
#endif  //  defined(__AVX__)

#if defined(__AVX2__)
#define RAD_COMPILED_X86_AVX2 1
#else
#define RAD_COMPILED_X86_AVX2 0
#endif  //  defined(__AVX2__)

#endif  // defined(RAD_ARCH_X86)

#if defined(RAD_ARCH_ANY_ARM)
#if defined(__ARM_NEON__)
#define RAD_COMPILED_ANY_ARM_NEON 1
#else
#define RAD_COMPILED_ANY_ARM_NEON 0
#endif  //  defined(__ARM_NEON__)
#endif  //  defined(RAD_ARCH_ANY_ARM)

#if defined(RAD_ARCH_MIPS)
#if defined(__mips_msa)
#define RAD_COMPILED_MIPS_MSA 1
#else
#define RAD_COMPILED_MIPS_MSA 0
#endif  //  defined(__mips_msa)
#if defined(__mips3d)
#define RAD_COMPILED_MIPS_MIPS3D 1
#else
#define RAD_COMPILED_MIPS_MIPS3D 0
#endif
#endif  //  defined(RAD_ARCH_MIPS)

#if defined(RAD_ARCH_RISCV)
#if defined(__riscv_e)
#define RAD_COMPILED_RISCV_E 1
#else
#define RAD_COMPILED_RISCV_E 0
#endif
#if defined(__riscv_i)
#define RAD_COMPILED_RISCV_I 1
#else
#define RAD_COMPILED_RISCV_I 0
#endif
#if defined(__riscv_m)
#define RAD_COMPILED_RISCV_M 1
#else
#define RAD_COMPILED_RISCV_M 0
#endif
#if defined(__riscv_a)
#define RAD_COMPILED_RISCV_A 1
#else
#define RAD_COMPILED_RISCV_A 0
#endif
#if defined(__riscv_f)
#define RAD_COMPILED_RISCV_F 1
#else
#define RAD_COMPILED_RISCV_F 0
#endif
#if defined(__riscv_d)
#define RAD_COMPILED_RISCV_D 1
#else
#define RAD_COMPILED_RISCV_D 0
#endif
#if defined(__riscv_q)
#define RAD_COMPILED_RISCV_Q 1
#else
#define RAD_COMPILED_RISCV_Q 0
#endif
#if defined(__riscv_c)
#define RAD_COMPILED_RISCV_C 1
#else
#define RAD_COMPILED_RISCV_C 0
#endif
#if defined(__riscv_v)
#define RAD_COMPILED_RISCV_V 1
#else
#define RAD_COMPILED_RISCV_V 0
#endif
#if defined(__riscv_zba)
#define RAD_COMPILED_RISCV_ZBA 1
#else
#define RAD_COMPILED_RISCV_ZBA 0
#endif
#if defined(__riscv_zbb)
#define RAD_COMPILED_RISCV_ZBB 1
#else
#define RAD_COMPILED_RISCV_ZBB 0
#endif
#if defined(__riscv_zbc)
#define RAD_COMPILED_RISCV_ZBC 1
#else
#define RAD_COMPILED_RISCV_ZBC 0
#endif
#if defined(__riscv_zbs)
#define RAD_COMPILED_RISCV_ZBS 1
#else
#define RAD_COMPILED_RISCV_ZBS 0
#endif
#if defined(__riscv_zfh)
#define RAD_COMPILED_RISCV_ZFH 1
#else
#define RAD_COMPILED_RISCV_ZFH 0
#endif
#if defined(__riscv_zfhmin)
#define RAD_COMPILED_RISCV_ZFHMIN 1
#else
#define RAD_COMPILED_RISCV_ZFHMIN 0
#endif
#if defined(__riscv_zknd)
#define RAD_COMPILED_RISCV_ZKND 1
#else
#define RAD_COMPILED_RISCV_ZKND 0
#endif
#if defined(__riscv_zkne)
#define RAD_COMPILED_RISCV_ZKNE 1
#else
#define RAD_COMPILED_RISCV_ZKNE 0
#endif
#if defined(__riscv_zknh)
#define RAD_COMPILED_RISCV_ZKNH 1
#else
#define RAD_COMPILED_RISCV_ZKNH 0
#endif
#if defined(__riscv_zksed)
#define RAD_COMPILED_RISCV_ZKSED 1
#else
#define RAD_COMPILED_RISCV_ZKSED 0
#endif
#if defined(__riscv_zksh)
#define RAD_COMPILED_RISCV_ZKSH 1
#else
#define RAD_COMPILED_RISCV_ZKSH 0
#endif
#if defined(__riscv_zkr)
#define RAD_COMPILED_RISCV_ZKR 1
#else
#define RAD_COMPILED_RISCV_ZKR 0
#endif
#endif  //  defined(RAD_ARCH_RISCV)

////////////////////////////////////////////////////////////////////////////////
// Utils
////////////////////////////////////////////////////////////////////////////////

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

#if defined(RAD_COMPILER_CLANG) || defined(RAD_COMPILER_GCC)
#define RAD_ASSUME(expr) __attribute__((assume(expr)))
#define RAD_UNREACHABLE __builtin_unreachable()
#define RAD_DEPRECATED(message) __attribute__((deprecated(message)))
#elif defined(RAD_COMPILER_MSVC)
#define RAD_ASSUME(expr) __assume(expr)
#define RAD_UNREACHABLE __assume(false)
#define RAD_DEPRECATED(message) __declspec(deprecated(message))
#else
#define RAD_ASSUME(expr)
#define RAD_UNREACHABLE()
#define RAD_DEPRECATED(message)
#endif

////////////////////////////////////////////////////////////////////////////////
// Memory Debug
////////////////////////////////////////////////////////////////////////////////

#if defined(RAD_COMPILER_MSVC) && defined(_DEBUG)
// https://learn.microsoft.com/en-us/cpp/c-runtime-library/find-memory-leaks-using-the-crt-library?view=msvc-170
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define RAD_NEW new (_NORMAL_BLOCK , __FILE__ , __LINE__)
#else
#define RAD_NEW new
#endif // RAD_COMPILER_MSVC
