set(rad_SOURCES
    Core/Platform.h
    Core/Integer.h
    Core/Integer.cpp
    Core/Float.h
    Core/Float.cpp
    Core/Numeric.h
    Core/Float16.h
    Core/Float16.cpp
    Core/BFloat16.h
    Core/BFloat16.cpp
    Core/Float8.h
    Core/Float8.cpp
    Core/Memory.h
    Core/Memory.cpp
    Core/RefCounted.h
    Core/TypeTraits.h
    Core/Sort.h
    Core/String.h
    Core/String.cpp
    Core/Flags.h
    Core/Math.h
    Core/Math.cpp
    Core/Time.h
    Core/Time.cpp
    Container/Span.h
    Container/SmallVector.h
    IO/File.h
    IO/File.cpp
    IO/FileSystem.h
    IO/FileSystem.cpp
    IO/Json.h
    IO/Json.cpp
    IO/Logging.h
    IO/Logging.cpp
    IO/ImageIO.h
    IO/ImageIO.cpp
    System/StackTrace.h
    System/StackTrace.cpp
    System/Program.h
    System/Program.cpp
    System/CpuInfo.h
    System/CpuInfo.cpp
)

if (RAD_BUILD_GUI)
set(rad_SOURCES ${rad_SOURCES}
    Gui/Application.h
    Gui/Application.cpp
    Gui/EventHandler.h
    Gui/EventHandler.cpp
    Gui/Window.h
    Gui/Window.cpp
    Gui/Renderer.h
    Gui/Renderer.cpp
    Gui/Surface.h
    Gui/Surface.cpp
    Gui/Texture.h
    Gui/Texture.cpp
    Gui/GuiContext.h
    Gui/GuiContext.cpp
)
endif()

if (RAD_BUILD_MEDIA)
set(rad_SOURCES ${rad_SOURCES}
    Media/Camera.h
    Media/Camera.cpp
    Media/Audio.h
    Media/Audio.cpp
    Media/AudioMixer.h
    Media/AudioMixer.cpp
)
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${rad_SOURCES})

add_library(rad
    ${rad_SOURCES}
)

find_package(Boost REQUIRED filesystem json)
find_package(Imath CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(CpuFeatures CONFIG REQUIRED)
find_package(Backward CONFIG REQUIRED)
target_link_libraries(rad
    PUBLIC ${Boost_LIBRARIES}
    PUBLIC Imath::Imath Imath::ImathConfig
    PUBLIC spdlog::spdlog
    PUBLIC CpuFeatures::cpu_features
    PUBLIC Backward::Backward
)

if (RAD_BUILD_GUI OR RAD_BUILD_MEDIA)
find_package(SDL3 CONFIG REQUIRED PATHS ${RADCPP_ROOT}/imported/installed/SDL)
target_link_libraries(rad
    PUBLIC SDL3::SDL3
)
target_include_directories(rad
    PUBLIC ${RADCPP_ROOT}/imported/imgui
)
endif()

if (RAD_BUILD_MEDIA)
find_package(SDL3_mixer CONFIG REQUIRED PATHS ${RADCPP_ROOT}/imported/installed/SDL_mixer)
target_link_libraries(rad
    PUBLIC SDL3_mixer::SDL3_mixer
)
endif()

if (WIN32)
target_compile_definitions(rad
    PRIVATE WIN32_LEAN_AND_MEAN=1
    PRIVATE NOMINMAX=1
)
target_compile_options(rad
    PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
)
endif()
