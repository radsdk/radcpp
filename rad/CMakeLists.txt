set(rad_SOURCES
    Core/Platform.h
    Core/Platform.cpp
    Core/Integer.h
    Core/Integer.cpp
    Core/Float.h
    Core/Float.cpp
    Core/SafeCast.h
    Core/SafeCast.cpp
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
