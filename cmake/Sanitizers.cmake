# https://github.com/google/sanitizers
option(ENABLE_ASAN "Enable AddressSanitizer")
option(ENABLE_LSAN "Enable LeakSanitizer")
option(ENABLE_TSAN "Enable ThreadSanitizer")
option(ENABLE_MSAN "Enable MemorySanitizer")
option(ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer")

if (MSVC)
    if (ENABLE_ASAN)
        # https://learn.microsoft.com/en-us/cpp/sanitizers/asan?view=msvc-170
        add_compile_options(/fsanitize=address)
        add_link_options(/INCREMENTAL:NO)
    endif()
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
    if (ENABLE_ASAN)
        # Turn off ASLR on Linux: setarch `uname -m` -R ./prog
        add_compile_options(-fsanitize=address -O0 -fno-optimize-sibling-calls -fno-ipa-icf)
    endif()
    if (ENABLE_LSAN)
        add_compile_options(-fsanitize=leak)
    endif()
    if (ENABLE_TSAN)
        add_compile_options(-fsanitize=thread)
    endif()
    if (ENABLE_UBSAN)
        add_compile_options(-fsanitize=undefined)
    endif()
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if (ENABLE_ASAN)
        # https://clang.llvm.org/docs/AddressSanitizer.html
        add_compile_options(-fsanitize=address -O1 -g -fno-omit-frame-pointer -fno-optimize-sibling-calls)
    endif()
    if (ENABLE_LSAN)
        # https://clang.llvm.org/docs/LeakSanitizer.html
        add_compile_options(-fsanitize=leak)
    endif()
    if (ENABLE_TSAN)
        # https://clang.llvm.org/docs/ThreadSanitizer.html
        add_compile_options(-fsanitize=thread -O1 -g)
    endif()
    if (ENABLE_MSAN)
        # https://clang.llvm.org/docs/MemorySanitizer.html
        add_compile_options(-fsanitize=memory -O1 -g -fno-omit-frame-pointer -fno-optimize-sibling-calls)
    endif()
    if (ENABLE_UBSAN)
        # https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
        add_compile_options(-fsanitize=undefined)
    endif()
endif()
