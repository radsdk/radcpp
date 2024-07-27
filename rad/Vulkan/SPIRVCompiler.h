#pragma once

#include <rad/Vulkan/VulkanCommon.h>
#include <spvgen.h>

namespace rad
{

class SPIRVCompiler : public RefCounted<SPIRVCompiler>
{
public:
    SPIRVCompiler();
    ~SPIRVCompiler();

    std::vector<uint32_t> Assemble(
        VkShaderStageFlagBits stage, std::string_view fileName, std::string_view source);
    std::vector<uint32_t> CompileGLSL(
        VkShaderStageFlagBits stage, std::string_view fileName, std::string_view source,
        std::string_view entryPoint = "main", Span<VulkanMacro> macros = {},
        uint32_t options = SpvGenOptionVulkanRules);

    const char* GetLog() const { return m_log.c_str(); }

private:
    std::string m_log;

}; // class SPIRVCompiler

} // namespace rad
