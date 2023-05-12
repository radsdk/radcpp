#include "VulkanPipeline.h"
#include "VulkanDevice.h"
#include "VulkanShader.h"
#include "VulkanRenderPass.h"

#include "rad/IO/File.h"

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

bool VulkanPipeline::SaveCacheToFile(VkPipelineCache cache, const rad::IO::Path& filePath)
{
    size_t cacheDataSize = 0;

    VK_CHECK(m_device->GetFunctionTable()->
        vkGetPipelineCacheData(m_device->GetHandle(), cache, &cacheDataSize, nullptr));

    if (cacheDataSize > 0)
    {
        std::vector<uint8_t> cacheData;
        cacheData.resize(cacheDataSize);
        VK_CHECK(m_device->GetFunctionTable()->
            vkGetPipelineCacheData(m_device->GetHandle(), cache, &cacheDataSize, cacheData.data()));

        rad::IO::File file;
        if (file.Open(filePath, rad::IO::FileAccessWrite | rad::IO::FileAccessBinary))
        {
            file.Write(cacheData.data(), cacheDataSize);
            file.Close();
            return true;
        }
    }
    return false;
}

VulkanGraphicsPipeline::VulkanGraphicsPipeline(rad::Ref<VulkanDevice> device,
    const VulkanGraphicsPipelineCreateInfo& createInfo) :
    VulkanPipeline(std::move(device), VK_PIPELINE_BIND_POINT_GRAPHICS)
{
    std::vector<rad::Ref<VulkanShaderModule>> shaderModules;

    std::vector<VkDynamicState> dynamicStates =
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
    };

    std::vector<VkPipelineShaderStageCreateInfo> shaderStageInfos;
    std::vector<VkSpecializationInfo> specializationInfos;
    VkPipelineVertexInputStateCreateInfo vertexInputState = {};
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
    VkPipelineTessellationStateCreateInfo tessellationState = {};
    VkPipelineViewportStateCreateInfo viewportState = {};
    VkPipelineRasterizationStateCreateInfo rasterizationState = {};
    VkPipelineMultisampleStateCreateInfo multisampleState = {};
    VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
    VkPipelineColorBlendStateCreateInfo colorBlendState = {};
    VkPipelineDynamicStateCreateInfo dynamicState = {};

    VkGraphicsPipelineCreateInfo createInfoNative = {};
    createInfoNative.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    createInfoNative.pNext = nullptr;
    createInfoNative.flags = 0;

    for (const auto& shader : createInfo.m_shaders)
    {
        assert(!shader->IsEmpty());
        rad::Ref<VulkanShaderModule> shaderModule = m_device->CreateShaderModule(shader->GetBinary());
        shaderModules.push_back(shaderModule);
        VkPipelineShaderStageCreateInfo shaderStageInfo = {};
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.pNext;
        shaderStageInfo.flags;
        shaderStageInfo.stage = shader->GetStage();
        shaderStageInfo.module = shaderModule->GetHandle();
        shaderStageInfo.pName = shader->GetEntryPoint();
        shaderStageInfo.pName = "main";
        shaderStageInfo.pSpecializationInfo = nullptr;
        shaderStageInfos.push_back(shaderStageInfo);
    }

    createInfoNative.stageCount = static_cast<uint32_t>(shaderStageInfos.size());
    createInfoNative.pStages = shaderStageInfos.data();

    const auto& vertexInput = createInfo.m_vertexInput;
    vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputState.pNext = nullptr;
    vertexInputState.flags = 0;
    vertexInputState.vertexBindingDescriptionCount = static_cast<uint32_t>(vertexInput.bindings.size());
    vertexInputState.pVertexBindingDescriptions = vertexInput.bindings.data();
    vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInput.attributes.size());
    vertexInputState.pVertexAttributeDescriptions = vertexInput.attributes.data();
    createInfoNative.pVertexInputState = &vertexInputState;

    const auto& inputAssembly = createInfo.m_inputAssembly;
    inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyState.pNext = nullptr;
    inputAssemblyState.flags = 0;
    inputAssemblyState.topology = inputAssembly.topology;
    inputAssemblyState.primitiveRestartEnable = inputAssembly.primitiveRestartEnable;
    createInfoNative.pInputAssemblyState = &inputAssemblyState;

    VkShaderStageFlags pipelineStageFlags = 0;
    for (const auto& shader : createInfo.m_shaders)
    {
        pipelineStageFlags |= shader->GetStage();
    }

    if (rad::Flags32<VkShaderStageFlagBits>(pipelineStageFlags).HasBits(
        VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT))
    {
        const auto& tessellation = createInfo.m_tessellation;
        tessellationState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
        tessellationState.pNext = nullptr;
        tessellationState.flags = 0;
        tessellationState.patchControlPoints = tessellation.patchControlPoints;
        createInfoNative.pTessellationState = &tessellationState;
    }

    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.pNext = nullptr;
    viewportState.flags = 0;
    viewportState.viewportCount = createInfo.m_viewportCount;
    viewportState.pViewports = nullptr; // will be set dynamically
    viewportState.scissorCount = createInfo.m_scissorCount;
    viewportState.pScissors = nullptr; // will be set dynamically
    createInfoNative.pViewportState = &viewportState;

    const auto& rasterization = createInfo.m_rasterization;
    rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationState.pNext = nullptr;
    rasterizationState.flags = 0; // is reserved for future use.
    rasterizationState.depthClampEnable = rasterization.depthClampEnable;
    rasterizationState.rasterizerDiscardEnable = rasterization.rasterizerDiscardEnable;
    rasterizationState.polygonMode = rasterization.polygonMode;
    rasterizationState.cullMode = rasterization.cullMode;
    rasterizationState.frontFace = rasterization.frontFace;
    rasterizationState.depthBiasEnable = rasterization.depthBiasEnable;
    rasterizationState.depthBiasConstantFactor = rasterization.depthBiasConstantFactor;
    rasterizationState.depthBiasClamp = rasterization.depthBiasClamp;
    rasterizationState.depthBiasSlopeFactor = rasterization.depthBiasSlopeFactor;
    rasterizationState.lineWidth = rasterization.lineWidth;
    createInfoNative.pRasterizationState = &rasterizationState;

    const auto& multisample = createInfo.m_multisample;
    multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleState.pNext = nullptr;
    multisampleState.flags = 0;
    multisampleState.rasterizationSamples = multisample.rasterizationSamples;
    multisampleState.sampleShadingEnable = multisample.sampleShadingEnable;
    multisampleState.minSampleShading = multisample.minSampleShading;
    multisampleState.pSampleMask = &multisample.sampleMask;
    multisampleState.alphaToCoverageEnable = multisample.alphaToCoverageEnable;
    multisampleState.alphaToOneEnable = multisample.alphaToOneEnable;
    createInfoNative.pMultisampleState = &multisampleState;

    const auto& depthStencil = createInfo.m_depthStencil;
    depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilState.pNext = nullptr;
    depthStencilState.flags = 0;
    depthStencilState.depthTestEnable = depthStencil.depthTestEnable;
    depthStencilState.depthWriteEnable = depthStencil.depthWriteEnable;
    depthStencilState.depthCompareOp = depthStencil.depthCompareOp;
    depthStencilState.depthBoundsTestEnable = depthStencil.depthBoundsTestEnable;
    depthStencilState.stencilTestEnable = depthStencil.stencilTestEnable;
    depthStencilState.front = depthStencil.front;
    depthStencilState.back = depthStencil.back;
    depthStencilState.minDepthBounds = depthStencil.minDepthBounds;
    depthStencilState.maxDepthBounds = depthStencil.maxDepthBounds;
    createInfoNative.pDepthStencilState = &depthStencilState;

    const auto& colorBlend = createInfo.m_colorBlend;
    colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendState.pNext = nullptr;
    colorBlendState.flags = 0;
    colorBlendState.logicOpEnable = colorBlend.logicOpEnable;
    colorBlendState.logicOp = colorBlend.logicOp;
    colorBlendState.attachmentCount = static_cast<uint32_t>(colorBlend.attachments.size());
    colorBlendState.pAttachments = colorBlend.attachments.data();
    colorBlendState.blendConstants[0] = colorBlend.blendConstants[0];
    colorBlendState.blendConstants[1] = colorBlend.blendConstants[1];
    colorBlendState.blendConstants[2] = colorBlend.blendConstants[2];
    colorBlendState.blendConstants[3] = colorBlend.blendConstants[3];
    createInfoNative.pColorBlendState = &colorBlendState;

    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pNext = nullptr;
    dynamicState.flags = 0;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();
    createInfoNative.pDynamicState = &dynamicState;

    createInfoNative.layout = createInfo.m_layout ? createInfo.m_layout->GetHandle() : VK_NULL_HANDLE;
    createInfoNative.renderPass = createInfo.m_renderPass->GetHandle();
    createInfoNative.subpass = createInfo.m_subpass;
    createInfoNative.basePipelineHandle = createInfo.m_basePipeline ?
        createInfo.m_basePipeline->GetHandle() : VK_NULL_HANDLE;
    createInfoNative.basePipelineIndex = createInfo.m_basePipelineIndex;

    VK_CHECK(m_device->GetFunctionTable()->
        vkCreateGraphicsPipelines(m_device->GetHandle(), VK_NULL_HANDLE, 1, &createInfoNative, nullptr, &m_handle));
}

VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
{
}

VulkanComputePipeline::VulkanComputePipeline(rad::Ref<VulkanDevice> device,
    const VulkanComputePipelineCreateInfo& createInfo) :
    VulkanPipeline(std::move(device), VK_PIPELINE_BIND_POINT_COMPUTE)
{
    rad::Ref<VulkanShaderModule> shaderModule = m_device->CreateShaderModule(createInfo.shader->GetBinary());

    VkComputePipelineCreateInfo createInfoNative = {};
    createInfoNative.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    createInfoNative.pNext = nullptr;
    createInfoNative.flags = 0;
    createInfoNative.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    createInfoNative.stage.pNext = nullptr;
    createInfoNative.stage.flags = 0;
    createInfoNative.stage.stage = createInfo.shader->GetStage();
    createInfoNative.stage.module = shaderModule->GetHandle();
    createInfoNative.stage.pName = createInfo.shader->GetEntryPoint();
    createInfoNative.stage.pSpecializationInfo = nullptr;;
    createInfoNative.layout = createInfo.layout ? createInfo.layout->GetHandle() : VK_NULL_HANDLE;
    createInfoNative.basePipelineHandle = createInfo.basePipeline ?
        createInfo.basePipeline->GetHandle() : VK_NULL_HANDLE;
    createInfoNative.basePipelineIndex = createInfo.basePipelineIndex;
    VK_CHECK(m_device->GetFunctionTable()->
        vkCreateComputePipelines(m_device->GetHandle(), VK_NULL_HANDLE, 1, &createInfoNative, nullptr, &m_handle));
}

VulkanComputePipeline::~VulkanComputePipeline()
{
}
