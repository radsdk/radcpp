#pragma once

#include <rad/Vulkan/VulkanCommon.h>

namespace rad
{

class VulkanDescriptorPool : public VulkanObject
{
public:
    VulkanDescriptorPool(
        rad::Ref<VulkanDevice> device,
        const VkDescriptorPoolCreateInfo& createInfo);
    ~VulkanDescriptorPool();

    VkDescriptorPool GetHandle() const { return m_handle; }

    rad::Ref<VulkanDescriptorSet> Allocate(VulkanDescriptorSetLayout* layout);
    void Reset();

private:
    rad::Ref<VulkanDevice>      m_device;
    VkDescriptorPool            m_handle = VK_NULL_HANDLE;

}; // class VulkanDescriptorPool

class VulkanDescriptorSetLayout : public VulkanObject
{
public:
    VulkanDescriptorSetLayout(
        rad::Ref<VulkanDevice> device,
        const VkDescriptorSetLayoutCreateInfo& createInfo);
    ~VulkanDescriptorSetLayout();

    VkDescriptorSetLayout GetHandle() const { return m_handle; }

private:
    rad::Ref<VulkanDevice>      m_device;
    VkDescriptorSetLayout       m_handle = VK_NULL_HANDLE;

}; // class VulkanDescriptorSetLayout

class VulkanDescriptorSet : public VulkanObject
{
public:
    VulkanDescriptorSet(
        rad::Ref<VulkanDevice> device,
        rad::Ref<VulkanDescriptorPool> descriptorPool,
        rad::Ref<VulkanDescriptorSetLayout> layout);
    ~VulkanDescriptorSet();

    VkDescriptorSet GetHandle() const { return m_handle; }

    void Update(rad::Span<VkWriteDescriptorSet> writes,
        rad::Span<VkCopyDescriptorSet> copies = {});
    void UpdateBuffers(
        uint32_t                        binding,
        uint32_t                        arrayElement,
        VkDescriptorType                type,
        rad::Span<VkDescriptorBufferInfo> bufferInfos);
    void UpdateUniformBuffers(
        uint32_t                        binding,
        uint32_t                        arrayElement,
        rad::Span<VkDescriptorBufferInfo> bufferInfos);
    void UpdateImages(
        uint32_t                        binding,
        uint32_t                        arrayElement,
        VkDescriptorType                type,
        rad::Span<VulkanImageView*>     imageViews,
        rad::Span<VkImageLayout>        layouts);
    void UpdateCombinedImageSamplers(
        uint32_t                        binding,
        uint32_t                        arrayElement,
        rad::Span<VulkanImageView*>     imageViews,
        rad::Span<VkImageLayout>        layouts,
        rad::Span<VulkanSampler*>       samplers);

private:
    rad::Ref<VulkanDevice>              m_device;
    rad::Ref<VulkanDescriptorPool>      m_descriptorPool;
    rad::Ref<VulkanDescriptorSetLayout> m_layout;
    VkDescriptorSet                     m_handle = VK_NULL_HANDLE;

}; // class VulkanDescriptorSet

} // namespace rad
