set(SP1_SOURCE_FILES
    main.cpp
    Test.Core.Common.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SP1_SOURCE_FILES})

add_executable(sp1 ${SP1_SOURCE_FILES})

find_package(GTest CONFIG REQUIRED)
target_link_libraries(sp1
    PRIVATE rad
    PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
)
