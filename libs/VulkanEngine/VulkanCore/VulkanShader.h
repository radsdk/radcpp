#ifndef VULKAN_SHADER_H
#define VULKAN_SHADER_H

#include "VulkanCommon.h"
#include "spvgen/include/spvgen.h"

class VulkanShaderModule : public VulkanObject
{
public:
    VulkanShaderModule(
        rad::Ref<VulkanDevice> device, const VkShaderModuleCreateInfo& createInfo);
    ~VulkanShaderModule();

    VkShaderModule GetHandle() const { return m_handle; }

private:
    rad::Ref<VulkanDevice> m_device;
    VkShaderModule m_handle;

}; // class VulkanShaderModule

class VulkanShader : public VulkanObject
{
public:
    VulkanShader();
    ~VulkanShader();

    static rad::Ref<VulkanShader> CreateFromFile(
        VkShaderStageFlagBits stage,
        std::string_view fileName,
        std::string_view entryPoint = "main",
        rad::Span<VulkanShaderMacro> macros = {},
        uint32_t options = SpvGenOptionVulkanRules);

    bool Compile(VkShaderStageFlagBits stage, std::string_view fileName, std::string_view source,
        const std::string_view entryPoint, rad::Span<VulkanShaderMacro> macros,
        uint32_t options = SpvGenOptionVulkanRules);
    bool Assemble(VkShaderStageFlagBits stage, std::string_view source);
    bool LoadBinary(VkShaderStageFlagBits stage, void* binary, size_t binarySize);
    bool LoadBinaryFromFile(VkShaderStageFlagBits stage, const std::string_view fileName);

    std::string Disassemble();

    VkShaderStageFlagBits GetStage() const { return m_stage; }
    const char* GetEntryPoint() { return m_entryPoint.c_str(); }
    bool IsEmpty() const { return m_binary.empty(); }

    const std::vector<uint32_t>& GetBinary() const { return m_binary; }
    size_t GetBinarySizeInBytes() const { return (m_binary.size() * sizeof(uint32_t)); }
    const char* GetLog() const { return m_log.c_str(); }

    void Clear() { m_binary.clear(); }

private:
    VkShaderStageFlagBits m_stage = VK_SHADER_STAGE_ALL;
    std::string m_fileName;
    std::string m_source;
    std::string m_entryPoint = "main";
    std::vector<VulkanShaderMacro> m_macros;
    std::vector<uint32_t> m_binary;
    std::string m_log;

}; // class VulkanShader

#endif // VULKAN_SHADER_H
