#include "VulkanFormat.h"
#include "internal/vk_format_utils.h"

bool VulkanFormat::IsUNORM() const
{
    return FormatIsUNORM(m_format);
}

bool VulkanFormat::IsSNORM() const
{
    return FormatIsSNORM(m_format);
}

bool VulkanFormat::IsUSCALED() const
{
    return FormatIsUSCALED(m_format);
}

bool VulkanFormat::IsSSCALED() const
{
    return FormatIsSSCALED(m_format);
}

bool VulkanFormat::IsUINT() const
{
    return FormatIsUINT(m_format);
}

bool VulkanFormat::IsSINT() const
{
    return FormatIsSINT(m_format);
}

bool VulkanFormat::IsSRGB() const
{
    return FormatIsSRGB(m_format);
}

bool VulkanFormat::IsSFLOAT() const
{
    return FormatIsSFLOAT(m_format);
}

bool VulkanFormat::IsUFLOAT() const
{
    return FormatIsUFLOAT(m_format);
}

bool VulkanFormat::IsSampledInt() const
{
    return FormatIsSampledInt(m_format);
}

bool VulkanFormat::IsSampledFloat() const
{
    return FormatIsSampledFloat(m_format);
}

bool VulkanFormat::IsCompressed_ASTC_HDR() const
{
    return FormatIsCompressed_ASTC_HDR(m_format);
}

bool VulkanFormat::IsCompressed_ASTC_LDR() const
{
    return FormatIsCompressed_ASTC_LDR(m_format);
}

bool VulkanFormat::IsCompressed_BC() const
{
    return FormatIsCompressed_BC(m_format);
}

bool VulkanFormat::IsCompressed_EAC() const
{
    return FormatIsCompressed_EAC(m_format);
}

bool VulkanFormat::IsCompressed_ETC2() const
{
    return FormatIsCompressed_ETC2(m_format);
}

bool VulkanFormat::IsCompressed_PVRTC() const
{
    return FormatIsCompressed_PVRTC(m_format);
}

bool VulkanFormat::IsCompressed() const
{
    return FormatIsCompressed(m_format);
}

VkImageAspectFlags VulkanFormat::GetAspectFlags() const
{
    switch (m_format)
    {
    case VK_FORMAT_D16_UNORM:
    case VK_FORMAT_D32_SFLOAT:
        return VK_IMAGE_ASPECT_DEPTH_BIT;

    case VK_FORMAT_D16_UNORM_S8_UINT:
    case VK_FORMAT_D24_UNORM_S8_UINT:
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
        return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;

    default:
        return VK_IMAGE_ASPECT_COLOR_BIT;
    }
}

bool VulkanFormat::IsDepthAndStencil() const
{
    return FormatIsDepthAndStencil(m_format);
}

bool VulkanFormat::IsDepthOnly() const
{
    return FormatIsDepthOnly(m_format);
}

bool VulkanFormat::IsStencilOnly() const
{
    return FormatIsStencilOnly(m_format);
}

bool VulkanFormat::HasDepth() const
{
    return FormatHasDepth(m_format);
}

bool VulkanFormat::HasStencil() const
{
    return FormatHasStencil(m_format);
}

uint32_t VulkanFormat::GetDepthSizeInBits() const
{
    return FormatDepthSize(m_format);
}

uint32_t VulkanFormat::GetStencilSizeInBits() const
{
    return FormatStencilSize(m_format);
}

bool VulkanFormat::IsPacked() const
{
    return FormatIsPacked(m_format);
}

bool VulkanFormat::RequiresYcbcrConversion() const
{
    return FormatRequiresYcbcrConversion(m_format);
}

bool VulkanFormat::IsXChromaSubsampled() const
{
    return FormatIsXChromaSubsampled(m_format);
}

bool VulkanFormat::IsYChromaSubsampled() const
{
    return FormatIsYChromaSubsampled(m_format);
}

bool VulkanFormat::IsSinglePlane_422() const
{
    return FormatIsSinglePlane_422(m_format);
}

uint32_t VulkanFormat::GetPlaneCount() const
{
    return FormatPlaneCount(m_format);
}

bool VulkanFormat::IsMultiplane() const
{
    return FormatIsMultiplane(m_format);
}

VkFormat VulkanFormat::GetMultiplaneCompatibleFormat(VkImageAspectFlags plane_aspect) const
{
    return FindMultiplaneCompatibleFormat(m_format, plane_aspect);
}

VkExtent2D VulkanFormat::GetMultiplaneExtentDivisors(VkImageAspectFlags plane_aspect) const
{
    return FindMultiplaneExtentDivisors(m_format, plane_aspect);
}

uint32_t VulkanFormat::GetComponentCount() const
{
    return FormatComponentCount(m_format);
}

VkExtent3D VulkanFormat::GetTexelBlockExtent() const
{
    return FormatTexelBlockExtent(m_format);
}

bool VulkanFormat::ElementIsTexel() const
{
    return FormatElementIsTexel(m_format);
}

uint32_t VulkanFormat::GetElementSize(VkImageAspectFlags aspectMask) const
{
    return FormatElementSize(m_format, aspectMask);
}

double VulkanFormat::GetTexelSize(VkImageAspectFlags aspectMask) const
{
    return FormatTexelSize(m_format, aspectMask);
}

bool VulkanFormat::HasComponentSize(uint32_t size) const
{
    return FormatHasComponentSize(m_format, size);
}

bool VulkanFormat::HasRed() const
{
    return FormatHasRed(m_format);
}

bool VulkanFormat::HasGreen() const
{
    return FormatHasGreen(m_format);
}

bool VulkanFormat::HasBlue() const
{
    return FormatHasBlue(m_format);
}

bool VulkanFormat::HasAlpha() const
{
    return FormatHasAlpha(m_format);
}

bool VulkanFormat::IsUndefined() const
{
    return FormatIsUndef(m_format);
}

bool VulkanFormat::IsBlockedImage() const
{
    return FormatIsBlockedImage(m_format);
}

bool VulkanFormat::IsColor() const
{
    return FormatIsColor(m_format);
}
