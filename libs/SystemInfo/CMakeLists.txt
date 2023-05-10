set(SystemInfo_SOURCE_FILES
    CpuInfo.h
    CpuInfo.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SystemInfo_SOURCE_FILES})

add_library(SystemInfo
    ${SystemInfo_SOURCE_FILES}
)

find_package(CpuFeatures CONFIG REQUIRED)

target_link_libraries(SystemInfo
    PUBLIC rad
    PUBLIC CpuFeatures::cpu_features
)
