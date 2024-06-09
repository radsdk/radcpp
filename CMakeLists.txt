cmake_minimum_required(VERSION 3.10)

project(radcpp VERSION 0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(RADCPP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
    ${RADCPP_SOURCE_DIR}
    ${RADCPP_SOURCE_DIR}/imported
)

add_subdirectory(rad)

add_subdirectory(tests/HelloWorld)
set_target_properties(HelloWorld PROPERTIES FOLDER "tests")
