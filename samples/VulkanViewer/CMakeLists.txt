set(VulkanViewer_SOURCE_FILES
    main.cpp
    VulkanViewer.h
    VulkanViewer.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${VulkanViewer_SOURCE_FILES})

add_executable(VulkanViewer ${VulkanViewer_SOURCE_FILES})

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(VulkanViewer
    PRIVATE DirectMedia
    PRIVATE SDL2::SDL2main
    PRIVATE VulkanEngine
)

find_package(Backward CONFIG REQUIRED)
add_backward(VulkanViewer)
target_link_libraries(VulkanViewer PRIVATE Backward::Backward)

set_target_properties(VulkanViewer PROPERTIES
    FOLDER "samples"
    VS_DPI_AWARE "PerMonitor"
)

add_custom_command(TARGET VulkanViewer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Copy resources ..."
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${RADCPP_SOURCE_DIR}/libs/VulkanEngine/Shaders/ ${CMAKE_CURRENT_BINARY_DIR}/Shaders/
)

if(WIN32)
add_custom_command(TARGET VulkanViewer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Copy dependencies ..."
    COMMAND ${CMAKE_COMMAND} -E copy ${RADCPP_SOURCE_DIR}/3rdparty/build/spvgen/Release/spvgen.dll ${CMAKE_CURRENT_BINARY_DIR}
)
endif()
