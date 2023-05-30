#pragma once

#include "VulkanConfig.h"

class VulkanFormat
{
public:
    VulkanFormat(VkFormat format) :
        m_format(format)
    {
    }
    ~VulkanFormat()
    {
    }

    operator VkFormat() const { return m_format; }

    // Numeric
    // Formats with more then one numeric type (VK_FORMAT_D16_UNORM_S8_UINT) will return false
    bool IsUNORM() const;
    bool IsSNORM() const;
    bool IsUSCALED() const;
    bool IsSSCALED() const;
    bool IsUINT() const;
    bool IsSINT() const;
    bool IsSRGB() const;
    bool IsSFLOAT() const;
    bool IsUFLOAT() const;

    // Types from "Interpretation of Numeric Format" table (OpTypeFloat vs OpTypeInt)
    bool IsSampledInt() const;
    bool IsSampledFloat() const;

    // Compressed
    bool IsCompressed_ASTC_HDR() const;
    bool IsCompressed_ASTC_LDR() const;
    bool IsCompressed_BC() const;
    bool IsCompressed_EAC() const;
    bool IsCompressed_ETC2() const;
    bool IsCompressed_PVRTC() const;
    bool IsCompressed() const;

    // Depth/Stencil
    VkImageAspectFlags GetAspectFlags() const;
    bool IsDepthAndStencil() const;
    bool IsDepthOnly() const;
    bool IsStencilOnly() const;
    bool HasDepth() const;
    bool HasStencil() const;
    uint32_t GetDepthSizeInBits() const;
    uint32_t GetStencilSizeInBits() const;

    // Packed
    bool IsPacked() const;

    // YCbCr
    bool RequiresYcbcrConversion() const;
    bool IsXChromaSubsampled() const;
    bool IsYChromaSubsampled() const;

    // Multiplane
    bool IsSinglePlane_422() const;
    uint32_t GetPlaneCount() const;
    bool IsMultiplane() const;
    VkFormat GetMultiplaneCompatibleFormat(VkImageAspectFlags plane_aspect) const;
    VkExtent2D GetMultiplaneExtentDivisors(VkImageAspectFlags plane_aspect) const;

    // Size
    uint32_t GetComponentCount() const;
    VkExtent3D GetTexelBlockExtent() const;
    // Return true if format is 'normal', with one texel per format element
    bool ElementIsTexel() const;
    // Return size, in bytes, of one element of the specified format
    // For uncompressed this is one texel, for compressed it is one block
    uint32_t GetElementSize(VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT) const;
    // Return the size in bytes of one texel of given foramt
    // For compressed or multi-plane, this may be a fractional number
    double GetTexelSize(VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT) const;

    // Components
    bool HasComponentSize(uint32_t size) const;
    bool HasRed() const;
    bool HasGreen() const;
    bool HasBlue() const;
    bool HasAlpha() const;

    // Utils/misc
    bool IsUndefined() const;
    bool IsBlockedImage() const;

    // No official spec definition of "color format"
    // So anything that could NOT be a "color format" is a color format
    bool IsColor() const;

private:
    VkFormat m_format;

}; // class VulkanFormat
