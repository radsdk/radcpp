set(rad_SOURCES
    Core/Platform.h
    Core/Platform.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${rad_SOURCES})

add_library(rad STATIC
    ${rad_SOURCES}
)
