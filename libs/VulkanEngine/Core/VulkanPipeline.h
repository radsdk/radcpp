#ifndef VULKAN_PIPELINE_H
#define VULKAN_PIPELINE_H

#include "VulkanCommon.h"

class VulkanPipelineLayout : public VulkanObject
{
public:
    VulkanPipelineLayout(rad::Ref<VulkanDevice> device,
        const VkPipelineLayoutCreateInfo& createInfo);
    ~VulkanPipelineLayout();

    VkPipelineLayout GetHandle() const { return m_handle; }

private:
    rad::Ref<VulkanDevice>      m_device;
    VkPipelineLayout            m_handle = VK_NULL_HANDLE;

}; // class VulkanPipelineLayout

// @TODO: PipelineCache
class VulkanPipeline : public VulkanObject
{
public:
    VulkanPipeline(rad::Ref<VulkanDevice> device, VkPipelineBindPoint bindPoint);
    ~VulkanPipeline();

    operator VkPipeline() const { return m_handle; }
    VkPipeline GetHandle() const { return m_handle; }

    VkPipelineBindPoint GetBindPoint() const;

protected:
    bool SaveCacheToFile(VkPipelineCache cache, const rad::IO::Path& filePath);

    rad::Ref<VulkanDevice>      m_device;
    VkPipeline                  m_handle = VK_NULL_HANDLE;
    VkPipelineBindPoint         m_bindPoint;

}; // class VulkanPipeline

class VulkanGraphicsPipeline : public VulkanPipeline
{
public:
    VulkanGraphicsPipeline(rad::Ref<VulkanDevice> device,
        const VulkanGraphicsPipelineCreateInfo& createInfo);
    ~VulkanGraphicsPipeline();

}; // class VulkanGraphicsPipeline

class VulkanComputePipeline : public VulkanPipeline
{
public:
    VulkanComputePipeline(rad::Ref<VulkanDevice> device,
        const VulkanComputePipelineCreateInfo& createInfo);
    ~VulkanComputePipeline();

}; // class VulkanComputePipeline

#endif // VULKAN_PIPELINE_H
