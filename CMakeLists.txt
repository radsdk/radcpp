cmake_minimum_required(VERSION 3.21)

project(radcpp VERSION 0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(cmake/Sanitizers.cmake)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(RADCPP_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
    ${RADCPP_ROOT}
)

add_subdirectory(imported/backward-cpp)
set_target_properties(backward_object PROPERTIES FOLDER "imported")
set_target_properties(backward PROPERTIES FOLDER "imported")
add_subdirectory(imported/sqlite)
set_target_properties(sqlite PROPERTIES FOLDER "imported")

add_subdirectory(rad)

add_subdirectory(tests/HelloWorld)
set_target_properties(HelloWorld PROPERTIES FOLDER "tests")
add_subdirectory(tests/WindowTest)
set_target_properties(WindowTest PROPERTIES FOLDER "tests")
