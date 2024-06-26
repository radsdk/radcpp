option(RAD_DISABLE_LOGGING "Disable logging." OFF)

set(rad_SOURCES
    Core/Platform.h
    Core/Platform.cpp
    Core/Integer.h
    Core/Integer.cpp
    Core/Float.h
    Core/Float.cpp
    Core/Float16.h
    Core/Float16.cpp
    Core/BFloat16.h
    Core/BFloat16.cpp
    Core/Float8.h
    Core/Float8.cpp
    Core/SafeCast.h
    Core/SafeCast.cpp
    Core/String.h
    Core/String.cpp
    Core/Memory.h
    Core/Memory.cpp
    Core/MemoryDebug.h
    Core/MemoryDebug.cpp
    Core/RefCounted.h
    Core/Time.h
    Core/Time.cpp
    Core/TypeTraits.h
    Core/Flags.h
    Core/Sort.h
    Core/Math.h
    Core/Math.cpp
    Container/Span.h
    Container/SmallVector.h
    IO/FileSystem.h
    IO/FileSystem.cpp
    IO/File.h
    IO/File.cpp
    IO/Json.h
    IO/Json.cpp
    IO/Logging.h
    IO/Logging.cpp
    IO/ImageIO.h
    IO/ImageIO.cpp
    System/OS.h
    System/OS.cpp
    System/CpuInfo.h
    System/CpuInfo.cpp
    System/Process.h
    System/Process.cpp
    System/StackTrace.h
    System/StackTrace.cpp
    System/Timer.h
    System/Timer.cpp
    System/Power.h
    System/Power.cpp
)

set(rad_GUI_SOURCES
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
)

set(rad_SOURCES ${rad_SOURCES} ${rad_GUI_SOURCES})

set(rad_MEDIA_SOURCES
    Media/Camera.h
    Media/Camera.cpp
    Media/Audio.h
    Media/Audio.cpp
    Media/AudioMixer.h
    Media/AudioMixer.cpp
)

set(rad_SOURCES ${rad_SOURCES} ${rad_MEDIA_SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${rad_SOURCES})

add_library(rad STATIC
    ${rad_SOURCES}
)

find_package(Boost REQUIRED filesystem json)
find_package(Imath CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(CpuFeatures CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED PATHS ${RADCPP_ROOT}/imported/installed/SDL)
find_package(SDL3_mixer CONFIG REQUIRED PATHS ${RADCPP_ROOT}/imported/installed/SDL_mixer)

target_include_directories(rad
    PUBLIC ${Boost_INCLUDE_DIRS}
    PUBLIC ${RADCPP_ROOT}/imported/installed/SDL/include
    PUBLIC ${RADCPP_ROOT}/imported/installed/SDL_mixer/include
)

target_link_libraries(rad
    PUBLIC ${Boost_LIBRARIES}
    PUBLIC Imath::Imath Imath::ImathConfig
    PUBLIC spdlog::spdlog
    PUBLIC CpuFeatures::cpu_features
    PUBLIC Backward::Backward
    PUBLIC SDL3::SDL3
    PUBLIC SDL3_mixer::SDL3_mixer
)

if (RAD_DISABLE_LOGGING)
target_compile_definitions(rad
    PUBLIC RAD_DISABLE_LOGGING=1
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
