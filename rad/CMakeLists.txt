set(RAD_SOURCE_FILES
    Core/Config.h
    Core/Platform.h
    Core/Platform.cpp
    Core/Common.h
    Core/Common.cpp
    Core/Compare.h
    Core/Flags.h
    Core/TypeTraits.h
    Core/String.h
    Core/String.cpp
    Core/Memory.h
    Core/Memory.cpp
    Core/RefCounted.h
    Core/Span.h
    IO/FileSystem.h
    IO/FileSystem.cpp
    IO/File.h
    IO/File.cpp
    IO/Logging.h
    IO/Logging.cpp
    IO/JsonDoc.h
    IO/JsonDoc.cpp
    Math/Math.h
    Math/Math.cpp
    Perf/Stopwatch.h
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${RAD_SOURCE_FILES})

add_library(rad
    ${RAD_SOURCE_FILES}
)

find_package(Boost)

target_include_directories(rad
    PUBLIC ${Boost_INCLUDE_DIRS}
)

target_link_libraries(rad
    PUBLIC ${Boost_LIBRARIES}
)

if (WIN32)
target_compile_definitions(rad
    PUBLIC WIN32_LEAN_AND_MEAN
)
endif()
