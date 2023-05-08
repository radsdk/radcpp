set(RAD_SOURCE_FILES
    Core/Config.h
    Core/Platform.h
    Core/Platform.cpp
    Core/Common.h
    Core/Common.cpp
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
