cmake_minimum_required(VERSION 3.10)

project(radcpp
    VERSION 0.0.0.0
    DESCRIPTION "Great C++ Collections"
    HOMEPAGE_URL "https://github.com/radsdk/radcpp"
    LANGUAGES C CXX
)

option(RADCPP_BUILD_LIBS "Build additional libraries" ON)
option(RADCPP_BUILD_TESTS "Build tests" ON)

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
add_subdirectory(libs/OpenCompute)
set_target_properties(OpenCompute PROPERTIES FOLDER "libs")
add_subdirectory(libs/SystemInfo)
set_target_properties(SystemInfo PROPERTIES FOLDER "libs")
add_subdirectory(libs/SimpleDirectMedia)
set_target_properties(SimpleDirectMedia PROPERTIES FOLDER "libs")
endif()

if (RADCPP_BUILD_TESTS)
add_subdirectory(tests/sp1)
set_target_properties(sp1 PROPERTIES FOLDER "tests")
endif()
