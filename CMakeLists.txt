cmake_minimum_required(VERSION 3.10)

project(radcpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(RAD_BUILD_GUI "Build Gui component." ON)

set(RADCPP_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
    ${RADCPP_ROOT}
)

add_subdirectory(rad)
add_subdirectory(test)
