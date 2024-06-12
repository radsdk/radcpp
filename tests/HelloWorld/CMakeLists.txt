set(HelloWorld_SOURCES
    main.cpp
    Core/TestInteger.cpp
    Core/TestQuantization.cpp
    Core/TestRefCounted.cpp
    IO/TestImageIO.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HelloWorld_SOURCES})

add_executable(HelloWorld
    ${HelloWorld_SOURCES}
)

find_package(GTest CONFIG REQUIRED)
target_link_libraries(HelloWorld
    PRIVATE rad
    PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
)
