set(test_SOURCES
    main.cpp
    Core/TestFloat.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${test_SOURCES})

add_executable(test
    ${test_SOURCES}
)

find_package(GTest CONFIG REQUIRED)
target_link_libraries(test
    PRIVATE rad
    PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
)
