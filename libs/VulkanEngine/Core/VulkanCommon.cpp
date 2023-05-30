#define VOLK_IMPLEMENTATION
#define VMA_IMPLEMENTATION

#include "VulkanCommon.h"
#include "VulkanBuffer.h"
#include "VulkanCommandBuffer.h"
#include "VulkanCommandPool.h"
#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSet.h"
#include "VulkanDevice.h"
#include "VulkanEvent.h"
#include "VulkanFence.h"
#include "VulkanFormat.h"
#include "VulkanFramebuffer.h"
#include "VulkanImage.h"
#include "VulkanInstance.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanPipeline.h"
#include "VulkanQueue.h"
#include "VulkanRenderPass.h"
#include "VulkanSampler.h"
#include "VulkanSemaphore.h"
#include "VulkanShader.h"
#include "VulkanSwapchain.h"


RAD_LOG_CATEGORY_DEFINE(Vulkan);

void LogVulkanError(VkResult result, const char* function, const char* file, uint32_t line)
{
    LogVulkan(Error, "%s failed with VkResult=%s(%d).",
        function, string_VkResult(result), result, file, line);
    throw VulkanError(result);
}

std::string GetVulkanVersionString(uint32_t versionNumber)
{
    return rad::StrPrint("%u.%u.%u",
        VK_VERSION_MAJOR(versionNumber),
        VK_VERSION_MINOR(versionNumber),
        VK_VERSION_PATCH(versionNumber)
    );
}

VulkanGraphicsPipelineCreateInfo::VulkanGraphicsPipelineCreateInfo()
{
    VkStencilOpState stencilOpState = {};
    stencilOpState.failOp = VK_STENCIL_OP_KEEP;
    stencilOpState.passOp = VK_STENCIL_OP_KEEP;
    stencilOpState.depthFailOp;
    stencilOpState.compareOp = VK_COMPARE_OP_ALWAYS;
    stencilOpState.compareMask;
    stencilOpState.writeMask;
    stencilOpState.reference;

    m_depthStencil.front = stencilOpState;
    m_depthStencil.back = stencilOpState;
}

VulkanGraphicsPipelineCreateInfo::~VulkanGraphicsPipelineCreateInfo()
{
}

void VulkanGraphicsPipelineCreateInfo::AddVertexBinding(
    uint32_t binding, uint32_t stride, VkVertexInputRate inputRate)
{
    VkVertexInputBindingDescription vertexBindingDesc = {};
    vertexBindingDesc.binding = binding;
    vertexBindingDesc.stride = stride;
    vertexBindingDesc.inputRate = inputRate;
    m_vertexInput.bindings.push_back(vertexBindingDesc);
}

void VulkanGraphicsPipelineCreateInfo::AddVertexBinding(
    uint32_t binding, rad::Span<VkFormat> attribFormats)
{
    VkVertexInputBindingDescription vertexBindingDesc = {};
    vertexBindingDesc.binding = binding;
    vertexBindingDesc.stride = 0;
    vertexBindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    uint32_t location = 0;
    for (VkFormat format : attribFormats)
    {
        VkVertexInputAttributeDescription vertexAttribDesc = {};
        vertexAttribDesc.location = location;
        vertexAttribDesc.binding = binding;
        vertexAttribDesc.format = format;
        vertexAttribDesc.offset = vertexBindingDesc.stride;
        m_vertexInput.attributes.push_back(vertexAttribDesc);

        location++;
        vertexBindingDesc.stride += VulkanFormat(format).GetElementSize();
    }

    m_vertexInput.bindings.push_back(vertexBindingDesc);
}

void VulkanGraphicsPipelineCreateInfo::AddVertexAttribute(
    uint32_t location, uint32_t binding, VkFormat format, uint32_t offset)
{
    VkVertexInputAttributeDescription vertexAttribDesc = {};
    vertexAttribDesc.location = location;
    vertexAttribDesc.binding = binding;
    vertexAttribDesc.format = format;
    vertexAttribDesc.offset = offset;
    m_vertexInput.attributes.push_back(vertexAttribDesc);
}

void VulkanGraphicsPipelineCreateInfo::AddColorBendAttachmentState_Disabled()
{
    VkPipelineColorBlendAttachmentState attachState = {};
    attachState.blendEnable = VK_FALSE;
    attachState.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT |
        VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT;
    m_colorBlend.attachments.push_back(attachState);
}
