set(WINDOWTEST_SOURCE_FILES
    main.cpp
    WindowTest.h
    WindowTest.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${WINDOWTEST_SOURCE_FILES})

add_executable(WindowTest ${WINDOWTEST_SOURCE_FILES})

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(WindowTest
    PRIVATE DirectMedia
    PRIVATE SDL2::SDL2main
)

find_package(Backward CONFIG REQUIRED)
add_backward(WindowTest)
target_link_libraries(WindowTest PRIVATE Backward::Backward)

set_target_properties(WindowTest PROPERTIES
    FOLDER "samples"
    VS_DPI_AWARE "PerMonitor"
)
