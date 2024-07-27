#include <rad/Vulkan/VulkanPipeline.h>
#include <rad/Vulkan/VulkanPhysicalDevice.h>
#include <rad/Vulkan/VulkanDevice.h>
#include <rad/Vulkan/VulkanRenderPass.h>
#include <rad/IO/File.h>

namespace rad
{

VulkanShaderModule::VulkanShaderModule(
    rad::Ref<VulkanDevice> device,
    const VkShaderModuleCreateInfo& createInfo) :
    m_device(std::move(device))
{
    VK_CHECK(m_device->GetFunctionTable()->
        vkCreateShaderModule(m_device->GetHandle(), &createInfo, nullptr, &m_handle));
}

VulkanShaderModule::~VulkanShaderModule()
{
    m_device->GetFunctionTable()->
        vkDestroyShaderModule(m_device->GetHandle(), m_handle, nullptr);
    m_handle = VK_NULL_HANDLE;
}

VulkanPipelineLayout::VulkanPipelineLayout(rad::Ref<VulkanDevice> device,
    const VkPipelineLayoutCreateInfo& createInfo) :
    m_device(std::move(device))
{
    VK_CHECK(m_device->GetFunctionTable()->
        vkCreatePipelineLayout(m_device->GetHandle(), &createInfo, nullptr, &m_handle));
}

VulkanPipelineLayout::~VulkanPipelineLayout()
{
    m_device->GetFunctionTable()->
        vkDestroyPipelineLayout(m_device->GetHandle(), m_handle, nullptr);
    m_handle = VK_NULL_HANDLE;
}


VulkanPipeline::VulkanPipeline(rad::Ref<VulkanDevice> device, VkPipelineBindPoint bindPoint) :
    m_device(std::move(device)),
    m_bindPoint(bindPoint)
{
}

VulkanPipeline::~VulkanPipeline()
{
    m_device->GetFunctionTable()->
        vkDestroyPipeline(m_device->GetHandle(), m_handle, nullptr);
}

VkPipelineBindPoint VulkanPipeline::GetBindPoint() const
{
    return m_bindPoint;
}

bool VulkanPipeline::SaveCacheToFile(VkPipelineCache cache, std::string_view filePath)
{
    size_t dataSize = 0;

    VK_CHECK(m_device->GetFunctionTable()->
        vkGetPipelineCacheData(m_device->GetHandle(), cache, &dataSize, nullptr));

    if (dataSize > 0)
    {
        std::vector<uint8_t> cacheData;
        cacheData.resize(dataSize);
        VK_CHECK(m_device->GetFunctionTable()->
            vkGetPipelineCacheData(m_device->GetHandle(), cache, &dataSize, cacheData.data()));
        File file;
        if (file.Open(filePath, "wb"))
        {
            file.Write(cacheData.data(), dataSize);
            file.Close();
            return true;
        }
    }
    return false;
}

VulkanGraphicsPipeline::VulkanGraphicsPipeline(rad::Ref<VulkanDevice> device,
    const VkGraphicsPipelineCreateInfo& createInfo) :
    VulkanPipeline(std::move(device), VK_PIPELINE_BIND_POINT_GRAPHICS)
{
    VK_CHECK(m_device->GetFunctionTable()->vkCreateGraphicsPipelines(
        m_device->GetHandle(), VK_NULL_HANDLE, 1, &createInfo, nullptr, &m_handle));
}

VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
{
}

VulkanComputePipeline::VulkanComputePipeline(rad::Ref<VulkanDevice> device,
    const VkComputePipelineCreateInfo& createInfo) :
    VulkanPipeline(std::move(device), VK_PIPELINE_BIND_POINT_COMPUTE)
{
    VK_CHECK(m_device->GetFunctionTable()->vkCreateComputePipelines(
        m_device->GetHandle(), VK_NULL_HANDLE, 1, &createInfo, nullptr, &m_handle));
}

VulkanComputePipeline::~VulkanComputePipeline()
{
}

VulkanGraphicsPipelineCreateInfo::VulkanGraphicsPipelineCreateInfo(VulkanDevice* device) :
    m_device(device)
{
    m_native = {};

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

const VkGraphicsPipelineCreateInfo&
VulkanGraphicsPipelineCreateInfo::Setup()
{
    VulkanPhysicalDevice* physicalDevice = m_device->GetPhysicalDevice();
    m_createInfo = {};
    m_createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    m_createInfo.pNext = nullptr;
    m_createInfo.flags = 0;

    VK_STRUCTURE_CHAIN_BEGIN(m_createInfo);

    m_native.shaderStages.clear();
    m_native.shaderStages.reserve(m_shaderStages.size());
    for (const auto& shaderStage : m_shaderStages)
    {
        VkPipelineShaderStageCreateInfo shaderStageInfo = {};
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.pNext = nullptr;
        shaderStageInfo.flags = 0;
        shaderStageInfo.stage = shaderStage.m_stage;
        shaderStageInfo.module = shaderStage.shader->GetHandle();
        shaderStageInfo.pName = "main";
        if (shaderStage.specialization)
        {
            m_native.specializationInfos.push_back(shaderStage.specialization->GetInfo());
            shaderStageInfo.pSpecializationInfo = &m_native.specializationInfos.back();
        }
        else
        {
            shaderStageInfo.pSpecializationInfo = nullptr;
        }
        m_native.shaderStages.push_back(shaderStageInfo);
    }

    m_createInfo.stageCount = static_cast<uint32_t>(m_native.shaderStages.size());
    m_createInfo.pStages = m_native.shaderStages.data();

    m_native.vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    m_native.vertexInput.pNext = nullptr;
    m_native.vertexInput.flags = 0;
    m_native.vertexInput.vertexBindingDescriptionCount =
        static_cast<uint32_t>(m_vertexInput.bindings.size());
    m_native.vertexInput.pVertexBindingDescriptions =
        m_vertexInput.bindings.data();
    m_native.vertexInput.vertexAttributeDescriptionCount =
        static_cast<uint32_t>(m_vertexInput.attributes.size());
    m_native.vertexInput.pVertexAttributeDescriptions =
        m_vertexInput.attributes.data();
    m_createInfo.pVertexInputState = &m_native.vertexInput;

    m_native.inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    m_native.inputAssembly.pNext = nullptr;
    m_native.inputAssembly.flags = 0;
    m_native.inputAssembly.topology = m_inputAssembly.topology;
    m_native.inputAssembly.primitiveRestartEnable = m_inputAssembly.primitiveRestartEnable;
    m_createInfo.pInputAssemblyState = &m_native.inputAssembly;

    VkShaderStageFlags pipelineStageFlags = 0;
    for (const auto& shaderStage : m_shaderStages)
    {
        pipelineStageFlags |= shaderStage.m_stage;
    }

    if (HasBits<uint32_t>(pipelineStageFlags,
        VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT))
    {
        m_native.tessellation.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
        m_native.tessellation.pNext = nullptr;
        m_native.tessellation.flags = 0;
        m_native.tessellation.patchControlPoints = m_tessellation.patchControlPoints;
        m_createInfo.pTessellationState = &m_native.tessellation;
    }

    m_native.viewport.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    m_native.viewport.pNext = nullptr;
    m_native.viewport.flags = 0;
    m_native.viewport.viewportCount = m_viewportCount;
    m_native.viewport.pViewports = nullptr; // will be set dynamically
    m_native.viewport.scissorCount = m_scissorCount;
    m_native.viewport.pScissors = nullptr; // will be set dynamically
    m_createInfo.pViewportState = &m_native.viewport;

    m_native.rasterization.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    m_native.rasterization.pNext = nullptr;
    m_native.rasterization.flags = 0; // is reserved for future use.
    m_native.rasterization.depthClampEnable = m_rasterization.depthClampEnable;
    m_native.rasterization.rasterizerDiscardEnable = m_rasterization.rasterizerDiscardEnable;
    m_native.rasterization.polygonMode = m_rasterization.polygonMode;
    m_native.rasterization.cullMode = m_rasterization.cullMode;
    m_native.rasterization.frontFace = m_rasterization.frontFace;
    m_native.rasterization.depthBiasEnable = m_rasterization.depthBiasEnable;
    m_native.rasterization.depthBiasConstantFactor = m_rasterization.depthBiasConstantFactor;
    m_native.rasterization.depthBiasClamp = m_rasterization.depthBiasClamp;
    m_native.rasterization.depthBiasSlopeFactor = m_rasterization.depthBiasSlopeFactor;
    m_native.rasterization.lineWidth = m_rasterization.lineWidth;
    m_createInfo.pRasterizationState = &m_native.rasterization;

    m_native.multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    m_native.multisample.pNext = nullptr;
    m_native.multisample.flags = 0;
    m_native.multisample.rasterizationSamples = m_multisample.rasterizationSamples;
    m_native.multisample.sampleShadingEnable = m_multisample.sampleShadingEnable;
    m_native.multisample.minSampleShading = m_multisample.minSampleShading;
    m_native.multisample.pSampleMask = &m_multisample.sampleMask;
    m_native.multisample.alphaToCoverageEnable = m_multisample.alphaToCoverageEnable;
    m_native.multisample.alphaToOneEnable = m_multisample.alphaToOneEnable;
    m_createInfo.pMultisampleState = &m_native.multisample;

    m_native.depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    m_native.depthStencil.pNext = nullptr;
    m_native.depthStencil.flags = 0;
    m_native.depthStencil.depthTestEnable = m_depthStencil.depthTestEnable;
    m_native.depthStencil.depthWriteEnable = m_depthStencil.depthWriteEnable;
    m_native.depthStencil.depthCompareOp = m_depthStencil.depthCompareOp;
    m_native.depthStencil.depthBoundsTestEnable = m_depthStencil.depthBoundsTestEnable;
    m_native.depthStencil.stencilTestEnable = m_depthStencil.stencilTestEnable;
    m_native.depthStencil.front = m_depthStencil.front;
    m_native.depthStencil.back = m_depthStencil.back;
    m_native.depthStencil.minDepthBounds = m_depthStencil.minDepthBounds;
    m_native.depthStencil.maxDepthBounds = m_depthStencil.maxDepthBounds;
    m_createInfo.pDepthStencilState = &m_native.depthStencil;

    m_native.colorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    m_native.colorBlend.pNext = nullptr;
    m_native.colorBlend.flags = 0;
    m_native.colorBlend.logicOpEnable = m_colorBlend.logicOpEnable;
    m_native.colorBlend.logicOp = m_colorBlend.logicOp;
    m_native.colorBlend.attachmentCount = static_cast<uint32_t>(m_colorBlend.attachments.size());
    m_native.colorBlend.pAttachments = m_colorBlend.attachments.data();
    m_native.colorBlend.blendConstants[0] = m_colorBlend.blendConstants[0];
    m_native.colorBlend.blendConstants[1] = m_colorBlend.blendConstants[1];
    m_native.colorBlend.blendConstants[2] = m_colorBlend.blendConstants[2];
    m_native.colorBlend.blendConstants[3] = m_colorBlend.blendConstants[3];
    m_createInfo.pColorBlendState = &m_native.colorBlend;

    m_native.dynamic.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    m_native.dynamic.pNext = nullptr;
    m_native.dynamic.flags = 0;
    m_native.dynamic.dynamicStateCount = static_cast<uint32_t>(m_dynamicStates.size());
    m_native.dynamic.pDynamicStates = m_dynamicStates.data();
    m_createInfo.pDynamicState = &m_native.dynamic;

    m_createInfo.layout = m_layout ? m_layout->GetHandle() : VK_NULL_HANDLE;
    if (m_renderPass)
    {
        m_createInfo.renderPass = m_renderPass->GetHandle();
    }
    else if (physicalDevice->m_vk13Features.dynamicRendering)
    {
        if (m_colorFormats.size() > 0 &&
            (m_native.renderingInfo.sType == VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR))
        {
            VK_STRUCTURE_CHAIN_ADD(m_createInfo, m_native.renderingInfo);
        }
    }
    m_createInfo.subpass = m_subpass;
    m_createInfo.basePipelineHandle = m_basePipeline ?
        m_basePipeline->GetHandle() : VK_NULL_HANDLE;
    m_createInfo.basePipelineIndex = m_basePipelineIndex;

    VK_STRUCTURE_CHAIN_END(m_createInfo);
    return m_createInfo;
}

void VulkanGraphicsPipelineCreateInfo::AddVertexBindingPacked(
    uint32_t binding, rad::Span<VkVertexInputAttrib> attribs, VkVertexInputRate inputRate)
{
    VkVertexInputBindingDescription bindingDesc = {};
    bindingDesc.binding = binding;
    bindingDesc.stride = 0;
    uint32_t location = 0;
    uint32_t offset = 0;
    for (const VkVertexInputAttrib& attrib : attribs)
    {
        VkVertexInputAttributeDescription attribDesc = {};
        attribDesc.location = attrib.location;
        attribDesc.binding = binding;
        attribDesc.format = attrib.format;
        attribDesc.offset = offset;
        m_vertexInput.attributes.push_back(attribDesc);
        location += 1;
        offset += vkuFormatElementSize(attrib.format);
    }
    bindingDesc.stride = offset;
    bindingDesc.inputRate = inputRate;
    m_vertexInput.bindings.push_back(bindingDesc);
}

void VulkanGraphicsPipelineCreateInfo::DisableColorBlend(uint32_t attachCount)
{
    m_colorBlend.attachments.resize(attachCount);
    for (auto& attachment : m_colorBlend.attachments)
    {
        attachment = {};
        attachment.blendEnable = VK_FALSE;
        attachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT |
            VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT;
    }
}

void VulkanGraphicsPipelineCreateInfo::SetRenderingInfo(
    rad::Span<VkFormat> colorFormats, VkFormat depthStencilFormat)
{
    m_native.renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
    m_colorFormats = colorFormats;
    m_native.renderingInfo.colorAttachmentCount = uint32_t(m_colorFormats.size());
    m_native.renderingInfo.pColorAttachmentFormats = m_colorFormats.data();
    m_native.renderingInfo.depthAttachmentFormat = depthStencilFormat;
    m_native.renderingInfo.stencilAttachmentFormat = depthStencilFormat;
}

VulkanComputePipelineCreateInfo::VulkanComputePipelineCreateInfo(VulkanDevice* device) :
    m_device(device)
{
    m_native = {};
}

VulkanComputePipelineCreateInfo::~VulkanComputePipelineCreateInfo()
{
}

const VkComputePipelineCreateInfo&
VulkanComputePipelineCreateInfo::Setup()
{
    m_createInfo = {};
    m_createInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    m_createInfo.pNext = nullptr;
    m_createInfo.flags = 0;
    m_createInfo.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    m_createInfo.stage.pNext = nullptr;
    m_createInfo.stage.flags = 0;
    m_createInfo.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    m_createInfo.stage.module = m_shaderModule->GetHandle();
    m_createInfo.stage.pName = "main";
    if (m_shaderSpecialization)
    {
        m_native.specializationInfo = m_shaderSpecialization->GetInfo();
        m_createInfo.stage.pSpecializationInfo = &m_native.specializationInfo;
    }
    else
    {
        m_createInfo.stage.pSpecializationInfo = nullptr;
    }
    m_createInfo.layout = m_layout ?
        m_layout->GetHandle() : VK_NULL_HANDLE;
    m_createInfo.basePipelineHandle = m_basePipeline ?
        m_basePipeline->GetHandle() : VK_NULL_HANDLE;
    m_createInfo.basePipelineIndex = m_basePipelineIndex;
    return m_createInfo;
}

} // namespace rad
