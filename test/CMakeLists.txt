set(test_SOURCES
    main.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${test_SOURCES})

add_executable(test
    ${test_SOURCES}
)

target_link_libraries(test rad)
