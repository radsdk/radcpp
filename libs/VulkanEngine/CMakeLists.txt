set(VULKAN_ENGINE_SOURCE_FILES
    Core/internal/vk_enum_string_helper.h
    Core/internal/vk_format_utils.cpp
    Core/internal/vk_format_utils.h
    Core/internal/vk_layer_data.h
    Core/VulkanBuffer.cpp
    Core/VulkanBuffer.h
    Core/VulkanCommandBuffer.cpp
    Core/VulkanCommandBuffer.h
    Core/VulkanCommandPool.cpp
    Core/VulkanCommandPool.h
    Core/VulkanCommon.cpp
    Core/VulkanCommon.h
    Core/VulkanConfig.h
    Core/VulkanDescriptorPool.cpp
    Core/VulkanDescriptorPool.h
    Core/VulkanDescriptorSet.cpp
    Core/VulkanDescriptorSet.h
    Core/VulkanDevice.cpp
    Core/VulkanDevice.h
    Core/VulkanEvent.cpp
    Core/VulkanEvent.h
    Core/VulkanFence.cpp
    Core/VulkanFence.h
    Core/VulkanFormat.cpp
    Core/VulkanFormat.h
    Core/VulkanFramebuffer.cpp
    Core/VulkanFramebuffer.h
    Core/VulkanImage.cpp
    Core/VulkanImage.h
    Core/VulkanInstance.cpp
    Core/VulkanInstance.h
    Core/VulkanObject.cpp
    Core/VulkanObject.h
    Core/VulkanPhysicalDevice.cpp
    Core/VulkanPhysicalDevice.h
    Core/VulkanPipeline.cpp
    Core/VulkanPipeline.h
    Core/VulkanQueue.cpp
    Core/VulkanQueue.h
    Core/VulkanRenderPass.cpp
    Core/VulkanRenderPass.h
    Core/VulkanSampler.cpp
    Core/VulkanSampler.h
    Core/VulkanSemaphore.cpp
    Core/VulkanSemaphore.h
    Core/VulkanShader.cpp
    Core/VulkanShader.h
    Core/VulkanSwapchain.cpp
    Core/VulkanSwapchain.h
    Core.h
    Scene/VulkanCamera.cpp
    Scene/VulkanCamera.h
    Scene/VulkanLight.cpp
    Scene/VulkanLight.h
    Scene/VulkanMaterial.cpp
    Scene/VulkanMaterial.h
    Scene/VulkanMesh.cpp
    Scene/VulkanMesh.h
    Scene/VulkanScene.cpp
    Scene/VulkanScene.h
    Scene/VulkanSceneImporter.cpp
    Scene/VulkanSceneImporter.h
    Scene/VulkanSceneNode.cpp
    Scene/VulkanSceneNode.h
    Gui/VulkanWindow.cpp
    Gui/VulkanWindow.h
    Gui/VulkanFrame.cpp
    Gui/VulkanFrame.h
    Gui/VulkanGuiContext.cpp
    Gui/VulkanGuiContext.h
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
