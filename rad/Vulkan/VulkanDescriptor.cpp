#include <rad/Vulkan/VulkanDescriptor.h>
#include <rad/Vulkan/VulkanDevice.h>
#include <rad/Vulkan/VulkanBuffer.h>
#include <rad/Vulkan/VulkanImage.h>
#include <rad/Vulkan/VulkanSampler.h>

namespace rad
{

VulkanDescriptorPool::VulkanDescriptorPool(
    rad::Ref<VulkanDevice> device,
    const VkDescriptorPoolCreateInfo& createInfo) :
    m_device(std::move(device))
{
    VK_CHECK(m_device->GetFunctionTable()->
        vkCreateDescriptorPool(m_device->GetHandle(), &createInfo, nullptr, &m_handle));
}

VulkanDescriptorPool::~VulkanDescriptorPool()
{
    m_device->GetFunctionTable()->
        vkDestroyDescriptorPool(m_device->GetHandle(), m_handle, nullptr);
    m_handle = VK_NULL_HANDLE;
}

rad::Ref<VulkanDescriptorSet> VulkanDescriptorPool::Allocate(VulkanDescriptorSetLayout* layout)
{
    return RAD_NEW VulkanDescriptorSet(m_device, this, layout);
}

void VulkanDescriptorPool::Reset()
{
    VK_CHECK(m_device->GetFunctionTable()->
        vkResetDescriptorPool(m_device->GetHandle(), m_handle, 0));
}

VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(
    rad::Ref<VulkanDevice> device,
    const VkDescriptorSetLayoutCreateInfo& createInfo) :
    m_device(std::move(device))
{
    VK_CHECK(m_device->GetFunctionTable()->
        vkCreateDescriptorSetLayout(m_device->GetHandle(), &createInfo, nullptr, &m_handle));
}

VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
{
    m_device->GetFunctionTable()->
        vkDestroyDescriptorSetLayout(m_device->GetHandle(), m_handle, nullptr);
}

VulkanDescriptorSet::VulkanDescriptorSet(
    rad::Ref<VulkanDevice> device,
    rad::Ref<VulkanDescriptorPool> descriptorPool,
    rad::Ref<VulkanDescriptorSetLayout> layout) :
    m_device(std::move(device)),
    m_descriptorPool(std::move(descriptorPool)),
    m_layout(std::move(layout))
{
    VkDescriptorSetLayout layoutHandle = m_layout->GetHandle();

    VkDescriptorSetAllocateInfo allocateInfo = {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocateInfo.pNext = nullptr;
    allocateInfo.descriptorPool = m_descriptorPool->GetHandle();
    allocateInfo.descriptorSetCount = 1;
    allocateInfo.pSetLayouts = &layoutHandle;

    VK_CHECK(m_device->GetFunctionTable()->
        vkAllocateDescriptorSets(m_device->GetHandle(), &allocateInfo, &m_handle));
}

VulkanDescriptorSet::~VulkanDescriptorSet()
{
    m_device->GetFunctionTable()->
        vkFreeDescriptorSets(m_device->GetHandle(), m_descriptorPool->GetHandle(), 1, &m_handle);
}

void VulkanDescriptorSet::Update(
    rad::Span<VkWriteDescriptorSet> writes,
    rad::Span<VkCopyDescriptorSet> copies)
{
    m_device->GetFunctionTable()->
        vkUpdateDescriptorSets(m_device->GetHandle(),
            static_cast<uint32_t>(writes.size()), writes.data(),
            static_cast<uint32_t>(copies.size()), copies.data());
}

void VulkanDescriptorSet::UpdateBuffers(
    uint32_t binding, uint32_t arrayElement, VkDescriptorType type,
    rad::Span<VkDescriptorBufferInfo> bufferInfos)
{
    VkWriteDescriptorSet write = {};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.pNext = nullptr;
    write.dstSet = m_handle;
    write.dstBinding = binding;
    write.dstArrayElement = arrayElement;
    write.descriptorCount = static_cast<uint32_t>(bufferInfos.size());
    write.descriptorType = type;
    write.pImageInfo = nullptr;
    write.pBufferInfo = bufferInfos.data();
    write.pTexelBufferView = nullptr;

    Update(write);
}

void VulkanDescriptorSet::UpdateUniformBuffers(
    uint32_t binding, uint32_t arrayElement, rad::Span<VkDescriptorBufferInfo> bufferInfos)
{
    UpdateBuffers(binding, arrayElement, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, bufferInfos);
}

void VulkanDescriptorSet::UpdateImages(
    uint32_t binding, uint32_t arrayElement, VkDescriptorType type,
    rad::Span<VulkanImageView*> imageViews, rad::Span<VkImageLayout> layouts)
{
    VkWriteDescriptorSet write = {};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.pNext = nullptr;
    write.dstSet = m_handle;
    write.dstBinding = binding;
    write.dstArrayElement = arrayElement;
    write.descriptorCount = static_cast<uint32_t>(imageViews.size());
    write.descriptorType = type;

    std::vector<VkDescriptorImageInfo> imageInfos(imageViews.size());
    for (size_t i = 0; i < imageViews.size(); i++)
    {
        imageInfos[i].sampler = VK_NULL_HANDLE;
        imageInfos[i].imageView = imageViews[i]->GetHandle();
        imageInfos[i].imageLayout = layouts[i];
    }

    write.pImageInfo = imageInfos.data();
    write.pBufferInfo = nullptr;
    write.pTexelBufferView = nullptr;

    Update(write);
}

void VulkanDescriptorSet::UpdateCombinedImageSamplers(
    uint32_t binding, uint32_t arrayElement,
    rad::Span<VulkanImageView*> imageViews, rad::Span<VkImageLayout> layouts,
    rad::Span<VulkanSampler*> samplers)
{
    VkWriteDescriptorSet write = {};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.pNext = nullptr;
    write.dstSet = m_handle;
    write.dstBinding = binding;
    write.dstArrayElement = arrayElement;
    write.descriptorCount = static_cast<uint32_t>(imageViews.size());
    write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

    std::vector<VkDescriptorImageInfo> imageInfos(imageViews.size());
    for (size_t i = 0; i < imageViews.size(); i++)
    {
        imageInfos[i].sampler = samplers[i]->GetHandle();
        imageInfos[i].imageView = imageViews[i]->GetHandle();
        imageInfos[i].imageLayout = layouts[i];
    }

    write.pImageInfo = imageInfos.data();
    write.pBufferInfo = nullptr;
    write.pTexelBufferView = nullptr;

    Update(write);
}

} // namespace rad
