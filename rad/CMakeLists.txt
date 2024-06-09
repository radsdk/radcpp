set(rad_SOURCES
    Core/Platform.h
    Core/Platform.cpp
    Core/Integer.h
    Core/Integer.cpp
    Core/Float.h
    Core/Float.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${rad_SOURCES})

add_library(rad STATIC
    ${rad_SOURCES}
)
