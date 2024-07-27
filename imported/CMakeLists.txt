if (RAD_BUILD_GUI)

set(imgui_SOURCES
    imgui/imconfig.h
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_internal.h
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui.cpp
    imgui/imgui.h
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h
    imgui/backends/imgui_impl_sdl3.cpp
    imgui/backends/imgui_impl_sdl3.h
    imgui/backends/imgui_impl_sdlrenderer3.cpp
    imgui/backends/imgui_impl_sdlrenderer3.h
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${imgui_SOURCES})

add_library(imgui
    ${imgui_SOURCES}
)

target_include_directories(imgui
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Vulkan-Headers/include
)

find_package(SDL3 CONFIG REQUIRED PATHS ${RADCPP_ROOT}/imported/installed/SDL)
target_link_libraries(imgui
    PUBLIC SDL3::SDL3
)

set_target_properties(imgui PROPERTIES FOLDER "imported")

endif()

if (RAD_BUILD_VULKAN)
add_library(spvgen SHARED IMPORTED GLOBAL)
if (WIN32)
    set_target_properties(spvgen PROPERTIES
        IMPORTED_LOCATION "${RADCPP_ROOT}/imported/spvgen/build/Release/spvgen.dll"
        IMPORTED_IMPLIB "${RADCPP_ROOT}/imported/spvgen/build/Release/spvgen.lib"
    )
endif()
endif()
