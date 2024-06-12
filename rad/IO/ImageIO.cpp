#if defined(_MSC_VER) && !defined(__clang__)
#define _CRT_SECURE_NO_WARNINGS
#endif
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "ImageIO.h"

namespace rad
{

bool GetImageInfo(std::string_view filename, int& width, int& height, int& channelCount)
{
    int res = stbi_info(filename.data(), &width, &height, &channelCount);
    return (res == 1);
}

ImageU8::ImageU8()
{
}

ImageU8::~ImageU8()
{
    Free();
}

bool ImageU8::GetInfo(std::string_view filename)
{
    int res = GetImageInfo(filename, m_width, m_height, m_channelCount);
    m_isHdr = stbi_is_hdr(filename.data());
    return res;
}

bool ImageU8::Load(std::string_view filename, int channelCount, float gamma, float scale)
{
    assert(m_data == nullptr);
    stbi_hdr_to_ldr_gamma(gamma);
    stbi_hdr_to_ldr_scale(scale);
    m_data = stbi_load(filename.data(), &m_width, &m_height, &m_channelCount, channelCount);
    return (m_data != nullptr);
}

bool ImageU8::Malloc(size_t size)
{
    assert(m_data == nullptr);
    m_data = (unsigned char*)STBI_MALLOC(size);
    return (m_data != nullptr);
}

bool ImageU8::Realloc(size_t size)
{
    assert(m_data != nullptr);
    if (unsigned char* data = (unsigned char*)STBI_REALLOC(m_data, size))
    {
        m_data = data;
        return true;
    }
    else
    {
        return false;
    }
}

void ImageU8::Free()
{
    if (m_data)
    {
        stbi_image_free(m_data);
        m_data = nullptr;
    }
}

bool ImageU8::WritePNG(std::string_view filename) const
{
    int res = stbi_write_png(filename.data(), m_width, m_height, m_channelCount, m_data, m_width * m_channelCount);
    return res; // returns 0 on failure and non-0 on success.
}

bool ImageU8::WritePNG(std::string_view filename, int left, int top, int right, int bottom) const
{
    assert(left > 0 && left < m_width && left < right && right < m_width);
    assert(top > 0 && top < m_height && top < bottom && bottom < m_height);
    int width = right - left;
    int height = bottom - top;
    int offset = (top * m_width + left) * m_channelCount;
    int res = stbi_write_png(filename.data(), width, height, m_channelCount,
        m_data + offset, m_width * m_channelCount);
    return res; // returns 0 on failure and non-0 on success.
}

bool ImageU8::WriteBMP(std::string_view filename) const
{
    int res = stbi_write_bmp(filename.data(), m_width, m_height, m_channelCount, m_data);
    return res; // returns 0 on failure and non-0 on success.
}

bool ImageU8::WriteTGA(std::string_view filename) const
{
    int res = stbi_write_tga(filename.data(), m_width, m_height, m_channelCount, m_data);
    return res; // returns 0 on failure and non-0 on success.
}

bool ImageU8::WriteJPG(std::string_view filename, int quality) const
{
    int res = stbi_write_png(filename.data(), m_width, m_height, m_channelCount, m_data, quality);
    return res; // returns 0 on failure and non-0 on success.
}

ImageFP32::ImageFP32()
{
}

ImageFP32::~ImageFP32()
{
    Free();
}

bool ImageFP32::GetInfo(std::string_view filename)
{
    int res = GetImageInfo(filename, m_width, m_height, m_channelCount);
    m_isHdr = stbi_is_hdr(filename.data());
    return res;
}

bool ImageFP32::Load(std::string_view filename, int channelCount, float gamma, float scale)
{
    assert(m_data == nullptr);
    stbi_ldr_to_hdr_gamma(gamma);
    stbi_ldr_to_hdr_scale(scale);
    m_data = stbi_loadf(filename.data(), &m_width, &m_height, &m_channelCount, channelCount);
    return (m_data != nullptr);
}

bool ImageFP32::Malloc(size_t size)
{
    assert(m_data == nullptr);
    m_data = (float*)STBI_MALLOC(size);
    return (m_data != nullptr);
}

bool ImageFP32::Realloc(size_t size)
{
    assert(m_data != nullptr);
    if (float* data = (float*)STBI_REALLOC(m_data, size))
    {
        m_data = data;
        return true;
    }
    else
    {
        return false;
    }
}

void ImageFP32::Free()
{
    if (m_data)
    {
        stbi_image_free(m_data);
        m_data = nullptr;
    }
}

bool ImageFP32::WriteHDR(std::string_view filename) const
{
    int res = stbi_write_hdr(filename.data(), m_width, m_height, m_channelCount, m_data);
    return res; // returns 0 on failure and non-0 on success.
}

} // namespace rad
