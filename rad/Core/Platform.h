#pragma once

// Pre-defined C/C++ Compiler Macros:
// https://github.com/cpredef/predef
// Predefined C/C++ macros for cross-platform development:
// https://dev.to/tenry/predefined-c-c-macros-43id
// Visual Studio Predefined Macros:
// https://learn.microsoft.com/en-us/cpp/preprocessor/predefined-macros

// Linux and Linux-derived
#if defined(__linux__)
#endif

// Android
#if defined(__ANDROID__)
#endif

// Linux (non-Android)
#if defined(__linux__) && !defined(__ANDROID__)
#endif

// Darwin (macOS and iOS)
#if defined(__APPLE__)
#endif

// Windows (32-bit and 64-bit)
#if defined(_WIN32)
#endif

// Windows 64-bit
#if defined(_WIN64)
#endif

// Visual Studio
#if defined(_MSC_VER)
#endif

// gcc
#if defined(__GNUC__)
#endif

// clang
#if defined(__clang__)
#endif

// emscripten
#if defined(__EMSCRIPTEN__)
#endif

// MinGW 32, MinGW-w64 32-bit
#if defined(__MINGW32__)
#endif

// MinGW-w64 64-bit
#if defined(__MINGW64__)
#endif

// Nintendo DS (ARM9)
#if defined(ARM9)
#endif

// Nintendo 3DS
#if defined(_3DS)
#endif

// Nintendo Switch
#if defined(__SWITCH__)
#endif

#ifndef RAD_GNUC_PREREQ
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
#endif // RAD_GNUC_PREREQ
