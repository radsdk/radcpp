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
    Core/String.h
    Core/String.cpp
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${rad_SOURCES})

add_library(rad
    ${rad_SOURCES}
)

find_package(Boost REQUIRED filesystem json)
target_link_libraries(rad
    PUBLIC ${Boost_LIBRARIES}
)
