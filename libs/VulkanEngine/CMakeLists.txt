set(VULKAN_ENGINE_SOURCE_FILES
    VulkanCore/internal/vk_enum_string_helper.h
    VulkanCore/internal/vk_format_utils.cpp
    VulkanCore/internal/vk_format_utils.h
    VulkanCore/internal/vk_layer_data.h
    VulkanCore/VulkanBuffer.cpp
    VulkanCore/VulkanBuffer.h
    VulkanCore/VulkanCommandBuffer.cpp
    VulkanCore/VulkanCommandBuffer.h
    VulkanCore/VulkanCommandPool.cpp
    VulkanCore/VulkanCommandPool.h
    VulkanCore/VulkanCommon.cpp
    VulkanCore/VulkanCommon.h
    VulkanCore/VulkanConfig.h
    VulkanCore/VulkanDescriptorPool.cpp
    VulkanCore/VulkanDescriptorPool.h
    VulkanCore/VulkanDescriptorSet.cpp
    VulkanCore/VulkanDescriptorSet.h
    VulkanCore/VulkanDevice.cpp
    VulkanCore/VulkanDevice.h
    VulkanCore/VulkanEvent.cpp
    VulkanCore/VulkanEvent.h
    VulkanCore/VulkanFence.cpp
    VulkanCore/VulkanFence.h
    VulkanCore/VulkanFormat.cpp
    VulkanCore/VulkanFormat.h
    VulkanCore/VulkanFramebuffer.cpp
    VulkanCore/VulkanFramebuffer.h
    VulkanCore/VulkanImage.cpp
    VulkanCore/VulkanImage.h
    VulkanCore/VulkanInstance.cpp
    VulkanCore/VulkanInstance.h
    VulkanCore/VulkanObject.cpp
    VulkanCore/VulkanObject.h
    VulkanCore/VulkanPhysicalDevice.cpp
    VulkanCore/VulkanPhysicalDevice.h
    VulkanCore/VulkanPipeline.cpp
    VulkanCore/VulkanPipeline.h
    VulkanCore/VulkanQueue.cpp
    VulkanCore/VulkanQueue.h
    VulkanCore/VulkanRenderPass.cpp
    VulkanCore/VulkanRenderPass.h
    VulkanCore/VulkanSampler.cpp
    VulkanCore/VulkanSampler.h
    VulkanCore/VulkanSemaphore.cpp
    VulkanCore/VulkanSemaphore.h
    VulkanCore/VulkanShader.cpp
    VulkanCore/VulkanShader.h
    VulkanCore/VulkanSwapchain.cpp
    VulkanCore/VulkanSwapchain.h
    VulkanCore.h
    VulkanWindow.cpp
    VulkanWindow.h
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${VULKAN_ENGINE_SOURCE_FILES})

add_library(VulkanEngine
    ${VULKAN_ENGINE_SOURCE_FILES}
)

target_include_directories(VulkanEngine
    PUBLIC ${RADCPP_SOURCE_DIR}/3rdparty/Vulkan-Headers/include/
    PUBLIC ${RADCPP_SOURCE_DIR}/3rdparty/volk/
    PUBLIC ${RADCPP_SOURCE_DIR}/3rdparty/VulkanMemoryAllocator/include/
)

find_package(meshoptimizer CONFIG REQUIRED)
target_link_libraries(VulkanEngine
    PUBLIC rad
    PUBLIC DirectMedia
    PUBLIC meshoptimizer::meshoptimizer
)

if (WIN32)
target_compile_definitions(rad
    PRIVATE WIN32_LEAN_AND_MEAN
)
endif()
