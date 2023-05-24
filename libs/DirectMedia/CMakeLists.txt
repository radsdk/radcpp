set(DirectMedia_HEADER_FILES
    Core/Application.h
    Core/EventHandler.h
    Core/Renderer.h
    Core/Surface.h
    Core/Texture.h
    Core/Window.h
    Core/GuiContext.h
    Core/PerfCounter.h
    Core.h
    Font/TrueTypeLibrary.h
    Font/TrueTypeFont.h
    Audio/Mixer.h
)

set(DirectMedia_SOURCE_FILES
    ${DirectMedia_HEADER_FILES}
    Core/Application.cpp
    Core/Renderer.cpp
    Core/Surface.cpp
    Core/Texture.cpp
    Core/Window.cpp
    Core/GuiContext.cpp
    Font/TrueTypeLibrary.cpp
    Font/TrueTypeFont.cpp
    Audio/Mixer.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${DirectMedia_SOURCE_FILES})

add_library(DirectMedia
    ${DirectMedia_SOURCE_FILES}
)

target_link_libraries(DirectMedia
    PUBLIC rad
)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(DirectMedia
    PUBLIC
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

find_package(SDL2_ttf CONFIG REQUIRED)
target_link_libraries(DirectMedia
    PUBLIC $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
)

find_package(SDL2_mixer CONFIG REQUIRED)
target_link_libraries(DirectMedia
    PUBLIC $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
)

target_link_libraries(DirectMedia
    PUBLIC imgui
)
