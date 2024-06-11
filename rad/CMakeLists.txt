set(rad_SOURCES
    Core/Platform.h
    Core/Platform.cpp
    Core/Integer.h
    Core/Integer.cpp
    Core/Float.h
    Core/Float.cpp
    Core/Float16.h
    Core/Float16.cpp
    Core/BFloat16.h
    Core/BFloat16.cpp
    Core/Float8.h
    Core/Float8.cpp
    Core/SafeCast.h
    Core/SafeCast.cpp
    Core/String.h
    Core/String.cpp
    Core/Memory.h
    Core/Memory.cpp
    Core/RefCounted.h
    Core/Time.h
    Core/Time.cpp
    Core/TypeTraits.h
    Core/Flags.h
    Core/Sort.h
    Core/Math.h
    Core/Math.cpp
    Container/Span.h
    Container/SmallVector.h
    IO/FileSystem.h
    IO/FileSystem.cpp
    IO/File.h
    IO/File.cpp
    IO/Json.h
    IO/Json.cpp
    IO/Logging.h
    IO/Logging.cpp
    System/OS.h
    System/OS.cpp
    System/CpuInfo.h
    System/CpuInfo.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${rad_SOURCES})

add_library(rad STATIC
    ${rad_SOURCES}
)

find_package(Boost REQUIRED json)
find_package(Imath CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(CpuFeatures CONFIG REQUIRED)

target_include_directories(rad
    PUBLIC ${Boost_INCLUDE_DIRS}
)

target_link_libraries(rad
    PUBLIC ${Boost_LIBRARIES}
    PUBLIC Imath::Imath Imath::ImathConfig
    PUBLIC spdlog::spdlog
    PUBLIC CpuFeatures::cpu_features
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
