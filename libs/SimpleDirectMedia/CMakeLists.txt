set(SimpleDirectMedia_HEADER_FILES
    Core/Application.h
    Core/EventHandler.h
    Core/Renderer.h
    Core/Surface.h
    Core/Texture.h
    Core/Window.h
)

set(SimpleDirectMedia_SOURCE_FILES
    ${SimpleDirectMedia_HEADER_FILES}
    Core/Application.cpp
    Core/Renderer.cpp
    Core/Surface.cpp
    Core/Texture.cpp
    Core/Window.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SimpleDirectMedia_SOURCE_FILES})

add_library(SimpleDirectMedia
    ${SimpleDirectMedia_SOURCE_FILES}
)

target_include_directories(SimpleDirectMedia
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(SimpleDirectMedia
    PUBLIC rad
)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(SimpleDirectMedia
    PUBLIC
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)
