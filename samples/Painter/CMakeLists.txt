set(PAINTER_SOURCE_FILES
    main.cpp
    Painter.h
    Painter.cpp
    Shapes.h
    Shapes.cpp
    Tools.h
    Tools.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PAINTER_SOURCE_FILES})

add_executable(Painter ${PAINTER_SOURCE_FILES})

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(Painter
    PRIVATE DirectMedia
    PRIVATE SDL2::SDL2main
)

find_package(Backward CONFIG REQUIRED)
add_backward(Painter)
target_link_libraries(Painter PRIVATE Backward::Backward)

set_target_properties(Painter PROPERTIES
    FOLDER "samples"
    VS_DPI_AWARE "PerMonitor"
)
