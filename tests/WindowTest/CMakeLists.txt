set(WindowTest_SOURCES
    main.cpp
    WindowTest.h
    WindowTest.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${WindowTest_SOURCES})

add_executable(WindowTest
    ${WindowTest_SOURCES}
)

target_link_libraries(WindowTest
    PRIVATE rad
)
