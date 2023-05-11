set(OpenCompute_SOURCE_FILES
    Core.cpp
    Core.h
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${OpenCompute_SOURCE_FILES})

add_library(OpenCompute ${OpenCompute_SOURCE_FILES})

target_link_libraries(OpenCompute
    PUBLIC rad
)

find_package(OpenCL REQUIRED)
target_link_libraries(OpenCompute
    PUBLIC OpenCL::OpenCL
)

target_compile_definitions(OpenCompute
    PUBLIC CL_TARGET_OPENCL_VERSION=300
)
