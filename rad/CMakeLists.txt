set(rad_SOURCES
    Core/Platform.h
    Core/Platform.cpp
    Core/Integer.h
    Core/Integer.cpp
    Core/Float.h
    Core/Float.cpp
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
    Core/Sort.h
    Container/Span.h
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${rad_SOURCES})

add_library(rad STATIC
    ${rad_SOURCES}
)

find_package(Boost)

target_include_directories(rad
    PUBLIC ${Boost_INCLUDE_DIRS}
)

target_link_libraries(rad
    PUBLIC ${Boost_LIBRARIES}
)
