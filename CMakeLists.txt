cmake_minimum_required(VERSION 3.10)

project(radcpp
    VERSION 0.0.0.0
    DESCRIPTION "Great C++ Collections"
    HOMEPAGE_URL "https://github.com/radsdk/radcpp"
    LANGUAGES C CXX
)

option(RADCPP_BUILD_LIBS "Build additional libraries" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(RADCPP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/
)

add_subdirectory(3rdparty/stb)
set_target_properties(stb PROPERTIES FOLDER "3rdparty")

add_subdirectory(rad)

if (RADCPP_BUILD_LIBS)
add_subdirectory(libs/SystemInfo)
endif()
