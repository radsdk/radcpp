#pragma once

#include <rad/Vulkan/VulkanCommon.h>

namespace rad
{

class VulkanCommandPool : public VulkanObject
{
public:
    VulkanCommandPool(rad::Ref<VulkanDevice> device, const VkCommandPoolCreateInfo& createInfo);
    ~VulkanCommandPool();

    VkCommandPool GetHandle() const { return m_handle; }

    rad::Ref<VulkanCommandBuffer> Allocate(VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    // Recycles unused memory.
    void Trim();
    // Recycles all of the resources from all of the command buffers allocated.
    void Reset(VkCommandPoolResetFlags flags = 0);

private:
    rad::Ref<VulkanDevice>      m_device;
    VkCommandPool               m_handle = VK_NULL_HANDLE;

}; // class VulkanCommandPool

class VulkanCommandBuffer : public VulkanObject
{
public:
    VulkanCommandBuffer(
        rad::Ref<VulkanDevice>          device,
        rad::Ref<VulkanCommandPool>     commandPool,
        VkCommandBufferLevel            level);
    ~VulkanCommandBuffer();

    VkCommandBuffer GetHandle() const { return m_handle; }

    void CreateCheckpoint(
        VkCommandBufferUsageFlags flags = 0,
        const VkCommandBufferInheritanceInfo* pInheritanceInfo = nullptr);
    void End();
    void Reset(VkCommandBufferResetFlags flags = 0);

    void BeginRenderPass(
        const VkRenderPassBeginInfo& beginInfo,
        VkSubpassContents contents);
    void BeginRenderPass(
        VulkanRenderPass* renderPass,
        VulkanFramebuffer* framebuffer,
        const VkRect2D& renderArea,
        rad::Span<VkClearValue> clearValues,
        VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE);
    void BeginRenderPass(
        VulkanRenderPass* renderPass,
        VulkanFramebuffer* framebuffer,
        rad::Span<VkClearValue> clearValues);
    void EndRenderPass();
    void NextSubpass(VkSubpassContents contents);

    void BeginRendering(
        const VkRenderingInfoKHR& renderingInfo);
    void BeginRendering(
        rad::Span<VulkanImageView*> colorViews,
        const VkClearColorValue* clearColor,
        VulkanImageView* depthStencilView = nullptr,
        const VkClearDepthStencilValue* clearDepthStencil = nullptr);
    void EndRendering();

    void BindPipeline(VulkanPipeline* pipeline);
    void BindDescriptorSets(
        VulkanPipeline* pipeline,
        VulkanPipelineLayout* layout,
        uint32_t firstSet,
        rad::Span<VulkanDescriptorSet*> descSets,
        rad::Span<uint32_t> dynamicOffsets = {});

    void SetScissors(rad::Span<VkRect2D> scissors, uint32_t first = 0);
    void SetViewports(rad::Span<VkViewport> viewports, uint32_t first = 0);

    void SetDepthBounds(float min, float max);

    void SetDepthStencilCompareMask(VkStencilFaceFlags faceMask, uint32_t compareMask);
    void SetStencilWriteMask(VkStencilFaceFlags faceMask, uint32_t writeMask);
    void SetStencilReference(VkStencilFaceFlags faceMask, uint32_t reference);
    void SetBlendConstants(float r, float g, float b, float a);

    // Rasterization

    void SetLineWidth(float width);
    void SetDepthBias(float constantFactor, float clamp, float slopeFactor);

    void BindIndexBuffer(VulkanBuffer* buffer, VkDeviceSize offset, VkIndexType indexType);
    void BindVertexBuffers(
        uint32_t firstBinding,
        rad::Span<VulkanBuffer*> buffers,
        rad::Span<VkDeviceSize> offsets);
    void BindVertexBuffers(uint32_t firstBinding, rad::Span<VulkanBuffer*> buffers);

    // Draw

    void Draw(
        uint32_t vertexCount, uint32_t instanceCount,
        uint32_t firstVertex, uint32_t firstInstance);
    void DrawIndexed(
        uint32_t indexCount, uint32_t instanceCount,
        uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
    void DrawIndirect(
        VulkanBuffer* buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
    void DrawIndexedIndirect(
        VulkanBuffer* buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);

    // Dispatch

    void Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
    void DispatchIndirect(VulkanBuffer* buffer, VkDeviceSize offset);
    void DispatchBase(
        uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ,
        uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);

    // Clear

    void ClearColorImage(
        VulkanImage* image, VkImageLayout layout,
        const VkClearColorValue& color, rad::Span<VkImageSubresourceRange> ranges);
    void ClearDepthStencilImage(
        VulkanImage* image, VkImageLayout layout,
        const VkClearDepthStencilValue& value, rad::Span<VkImageSubresourceRange> ranges);
    void ClearAttachments(rad::Span<VkClearAttachment> attachments, rad::Span<VkClearRect> rects);
    void FillBuffer(VulkanBuffer* dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data);
    // dataSize must be less than or equal to 65536 bytes.
    void UpdateBuffer(VulkanBuffer* dstBuffer,
        VkDeviceSize offset, VkDeviceSize dataSize, const uint32_t* pData);

    // Copy

    void CopyBuffer(VulkanBuffer* srcBuffer, VulkanBuffer* dstBuffer, rad::Span<VkBufferCopy> regions);
    void CopyImage(
        VulkanImage* srcImage, VkImageLayout srcLayout,
        VulkanImage* dstImage, VkImageLayout dstLayout,
        rad::Span<VkImageCopy> regions);
    void CopyBufferToImage(VulkanBuffer* srcBuffer,
        VulkanImage* dstImage, VkImageLayout dstImageLayout,
        rad::Span<VkBufferImageCopy> regions);
    void CopyImageToBuffer(VulkanImage* srcImage, VkImageLayout srcImageLayout,
        VulkanBuffer* dstBuffer, rad::Span<VkBufferImageCopy> regions);
    void BlitImage(
        VulkanImage* srcImage, VkImageLayout srcImageLayout,
        VulkanImage* dstImage, VkImageLayout dstImageLayout,
        rad::Span<VkImageBlit> regions, VkFilter filter);
    void ResolveImage(
        VulkanImage* srcImage, VkImageLayout srcImageLayout,
        VulkanImage* dstImage, VkImageLayout dstImageLayout,
        rad::Span<VkImageResolve> regions);

    void SetPushConstants(VulkanPipelineLayout* layout, VkShaderStageFlags stageFlags,
        uint32_t offset, uint32_t size, const void* pValues);

    // Khronos Synchronization Examples:
    // https://github.com/KhronosGroup/Vulkan-Docs/wiki/Synchronization-Examples

    void SetPipelineBarrier(
        VkPipelineStageFlags                srcStageMask,
        VkPipelineStageFlags                dstStageMask,
        VkDependencyFlags                   dependencyFlags,
        rad::Span<VkMemoryBarrier>          memoryBarriers,
        rad::Span<VkBufferMemoryBarrier>    bufferMemoryBarriers,
        rad::Span<VkImageMemoryBarrier>     imageMemoryBarriers
    );

    void SetPipelineBarrier2(const VkDependencyInfo& dependencyInfo);
    void SetPipelineBarrier2(
        VkDependencyFlags                   dependencyFlags,
        rad::Span<VkMemoryBarrier2>         memoryBarriers,
        rad::Span<VkBufferMemoryBarrier2>   bufferMemoryBarriers,
        rad::Span<VkImageMemoryBarrier2>    imageMemoryBarriers
    );

    // Global memory barrier covers all resources.
    void SetMemoryBarrier2(
        VkPipelineStageFlags2 srcStageMask, VkAccessFlags2 srcAccessMask,
        VkPipelineStageFlags2 dstStageMask, VkAccessFlags2 dstAccessMask);
    // For comput to compute storage buffer/image read after write.
    void SetMemoryBarrier_ComputeToCompute_ReadAfterWrite2();
    // A pipeline barrier or event without a any access flags is an execution dependency.
    void SetExecutionDependency2(
        VkPipelineStageFlags2 srcStageMask, VkPipelineStageFlags2 dstStageMask);
    void SetExecutionDependency2_ComputeToCompute();

    void TransitLayout(
        VulkanImage*            image,
        VkPipelineStageFlags    srcStageMask,
        VkPipelineStageFlags    dstStageMask,
        VkAccessFlags           srcAccessMask,
        VkAccessFlags           dstAccessMask,
        VkImageLayout           oldLayout,
        VkImageLayout           newLayout,
        const VkImageSubresourceRange* subresourceRange = nullptr);

    void TransitLayoutFromCurrent(
        VulkanImage* image,
        VkPipelineStageFlags    dstStageMask,
        VkAccessFlags           dstAccessMask,
        VkImageLayout           newLayout,
        const VkImageSubresourceRange* subresourceRange = nullptr);

private:
    rad::Ref<VulkanDevice>          m_device;
    rad::Ref<VulkanCommandPool>     m_commandPool;
    VkCommandBuffer                 m_handle = VK_NULL_HANDLE;
    VkCommandBufferLevel            m_level;

}; // class VulkanCommandBuffer

} // namespace rad
