#pragma once

#include "VulkanConfig.h"
#include "VulkanObject.h"

// Helpers
#include "internal/vk_enum_string_helper.h"
#include <exception>

class VulkanInstance;
class VulkanPhysicalDevice;
class VulkanDevice;
class VulkanQueue;
class VulkanCommandBuffer;
class VulkanCommandPool;
class VulkanFence;
class VulkanSemaphore;
class VulkanEvent;
class VulkanRenderPass;
class VulkanFramebuffer;
class VulkanShader;
class VulkanShaderModule;
class VulkanPipeline;
class VulkanComputePipeline;
class VulkanGraphicsPipeline;
class VulkanBuffer;
class VulkanBufferView;
class VulkanImage;
class VulkanImageView;
class VulkanSampler;
class VulkanDescriptorPool;
class VulkanDescriptorSet;
class VulkanDescriptorSetLayout;
class VulkanPipelineLayout;
class VulkanSwapchain;

struct VulkanStructureHeader
{
    VkStructureType sType;
    void* pNext;
};

template<typename Head, typename Last>
void AppendVulkanStructureChain(Head& head, Last& last)
{
    VulkanStructureHeader* p = reinterpret_cast<VulkanStructureHeader*>(&head);
    while (p->pNext != nullptr)
    {
        p = reinterpret_cast<VulkanStructureHeader*>(p->pNext);
    }
    p->pNext = &last;
    last.pNext = nullptr;
}

class VulkanError : public std::exception
{
public:
    VulkanError(VkResult result) : m_result(result) {}
    const char* what() const noexcept override { return string_VkResult(m_result); }
private:
    VkResult m_result;
}; // class VulkanError

RAD_LOG_CATEGORY_DECLARE(Vulkan, Verbose);
#define LogVulkan(Level, Format, ...) RAD_LOG(Vulkan, Level, Format, ##__VA_ARGS__)

// Check Vulkan return code and throw VulkanError if result < 0
void LogVulkanError(VkResult result, const char* function, const char* file, uint32_t line);
#define VK_CHECK(VulkanCall) \
{ const VkResult r = VulkanCall; if (r < 0) { LogVulkanError(r, #VulkanCall, __FILE__, __LINE__); } }

std::string GetVulkanVersionString(uint32_t versionNumber);

enum VulkanQueueFamily : uint32_t
{
    VulkanQueueFamilyUniversal = 0, // Default Family that supports all features
    VulkanQueueFamilyCompute,       // Async Compute Engine (ACE)
    VulkanQueueFamilyTransfer,      // DMA
    VulkanQueueFamilyCount
};

struct VulkanShaderMacro
{
    VulkanShaderMacro()
    {
    }

    VulkanShaderMacro(std::string_view name)
    {
        this->m_name = name;
    }

    VulkanShaderMacro(std::string_view name, std::string_view definition)
    {
        this->m_name = name;
        this->m_definition = definition;
    }

    template<typename T>
    VulkanShaderMacro(std::string_view name, T definition)
    {
        this->m_name = name;
        this->m_definition = std::to_string(definition);
    }

    std::string m_name;
    std::string m_definition;

}; // class VulkanShaderMacro

struct VulkanGraphicsPipelineCreateInfo : public rad::RefCounted<VulkanGraphicsPipelineCreateInfo>
{
    VulkanGraphicsPipelineCreateInfo();
    ~VulkanGraphicsPipelineCreateInfo();

    void AddVertexBinding(uint32_t binding, uint32_t stride,
        VkVertexInputRate inputRate = VK_VERTEX_INPUT_RATE_VERTEX);
    void AddVertexBinding(uint32_t binding, rad::Span<VkFormat> attribFormats);
    void AddVertexAttribute(uint32_t location, uint32_t binding, VkFormat format, uint32_t offset);

    std::vector<rad::Ref<VulkanShader>> m_shaders;

    struct VertexInputInfo
    {
        std::vector<VkVertexInputBindingDescription> bindings;
        std::vector<VkVertexInputAttributeDescription> attributes;
    } m_vertexInput;

    struct InputAssemblyInfo
    {
        VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        VkBool32 primitiveRestartEnable = VK_FALSE;
    } m_inputAssembly;

    struct TessellationInfo
    {
        uint32_t patchControlPoints = 0;
    } m_tessellation;

    // viewports and scissors should be set dynamically.
    uint32_t m_viewportCount = 1;
    uint32_t m_scissorCount = 1;

    struct RasterizationInfo
    {
        VkBool32                depthClampEnable = VK_FALSE;
        VkBool32                rasterizerDiscardEnable = VK_FALSE;
        VkPolygonMode           polygonMode = VK_POLYGON_MODE_FILL;
        VkCullModeFlags         cullMode = VK_CULL_MODE_NONE;
        VkFrontFace             frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        VkBool32                depthBiasEnable = VK_FALSE;
        float                   depthBiasConstantFactor = 0;
        float                   depthBiasClamp = 0;
        float                   depthBiasSlopeFactor = 0;
        float                   lineWidth = 1.0f;
    } m_rasterization;

    struct MultisampleInfo
    {
        VkSampleCountFlagBits   rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        VkBool32                sampleShadingEnable = VK_FALSE;
        float                   minSampleShading = 0.0f;    // must be in the range [0,1]
        VkSampleMask            sampleMask = 0xFFFFFFFF;
        VkBool32                alphaToCoverageEnable = VK_FALSE;
        VkBool32                alphaToOneEnable = VK_FALSE;
    } m_multisample;

    struct DepthStencilInfo
    {
        VkBool32                depthTestEnable = VK_FALSE;
        VkBool32                depthWriteEnable = VK_FALSE;
        VkCompareOp             depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
        VkBool32                depthBoundsTestEnable = VK_FALSE;
        VkBool32                stencilTestEnable = VK_FALSE;
        VkStencilOpState        front;
        VkStencilOpState        back;
        float                   minDepthBounds = 0.0f;
        float                   maxDepthBounds = 1.0f;
    } m_depthStencil;

    struct ColorBlendInfo
    {
        VkBool32                logicOpEnable = VK_FALSE;
        VkLogicOp               logicOp;
        std::vector<VkPipelineColorBlendAttachmentState> attachments;
        float                   blendConstants[4];
    } m_colorBlend;
    void AddColorBendAttachmentState_Disabled();

    rad::Ref<VulkanPipelineLayout>  m_layout;
    rad::Ref<VulkanRenderPass>      m_renderPass;
    uint32_t                        m_subpass = 0;
    rad::Ref<VulkanPipeline>        m_basePipeline;
    int32_t                         m_basePipelineIndex = 0;

}; // struct VulkanGraphicsPipelineCreateInfo

struct VulkanComputePipelineCreateInfo : public rad::RefCounted<VulkanComputePipelineCreateInfo>
{
    rad::Ref<VulkanShader>          shader;
    rad::Ref<VulkanPipelineLayout>  layout;
    rad::Ref<VulkanPipeline>        basePipeline;
    int32_t                         basePipelineIndex = 0;

}; // struct VulkanComputePipelineCreateInfo
