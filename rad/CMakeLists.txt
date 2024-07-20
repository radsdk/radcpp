set(rad_SOURCES
    Core/Platform.h
    Core/Integer.h
    Core/Integer.cpp
    Core/Float.h
    Core/Float.cpp
    Core/Numeric.h
    Core/Float16.h
    Core/Float16.cpp
    Core/BFloat16.h
    Core/BFloat16.cpp
    Core/Float8.h
    Core/Float8.cpp
    Core/Memory.h
    Core/Memory.cpp
    Core/RefCounted.h
    Core/TypeTraits.h
    Core/Sort.h
    Core/String.h
    Core/String.cpp
    Core/Flags.h
    Core/Math.h
    Core/Math.cpp
    Core/Time.h
    Core/Time.cpp
    Container/Span.h
    Container/SmallVector.h
    IO/File.h
    IO/File.cpp
    IO/FileSystem.h
    IO/FileSystem.cpp
    System/StackTrace.h
    System/StackTrace.cpp
    System/Program.h
    System/Program.cpp
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${rad_SOURCES})

add_library(rad
    ${rad_SOURCES}
)

find_package(Boost REQUIRED filesystem json)
find_package(Backward CONFIG REQUIRED)
target_link_libraries(rad
    PUBLIC ${Boost_LIBRARIES}
    PUBLIC Backward::Backward
)

if (WIN32)
target_compile_definitions(rad
    PRIVATE WIN32_LEAN_AND_MEAN=1
    PRIVATE NOMINMAX=1
)
target_compile_options(rad
    PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
)
endif()
