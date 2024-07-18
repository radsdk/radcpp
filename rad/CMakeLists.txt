set(rad_SOURCES
    Core/Platform.h
    Core/Integer.h
    Core/Integer.cpp
    Core/Float.h
    Core/Float.cpp
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${rad_SOURCES})

add_library(rad
    ${rad_SOURCES}
)
