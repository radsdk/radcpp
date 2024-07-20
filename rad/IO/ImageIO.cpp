#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#if defined(_MSC_VER)
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <rad/IO/ImageIO.h>

namespace rad
{

bool GetImageInfo(std::string_view filename, int& width, int& height, int& channelCount)
{
    int result = stbi_info(filename.data(), &width, &height, &channelCount);
    return (result == 1);
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
    return GetImageInfo(filename, m_width, m_height, m_channelCount);
}

bool ImageU8::Load(std::string_view filename, int channelCount, float gamma, float scale)
{
    assert(m_data == nullptr);
    stbi_hdr_to_ldr_gamma(gamma);
    stbi_hdr_to_ldr_scale(scale);
    m_data = stbi_load(filename.data(), &m_width, &m_height, &m_channelCount, channelCount);
    if (m_data)
    {
        m_sizeInBytes = size_t(m_width) * size_t(m_height) *
            size_t(m_channelCount) * sizeof(unsigned char);
        return true;
    }
    else
    {
        return false;
    }
}

bool ImageU8::Allocate(int width, int height, int channelCount)
{
    assert((m_data == nullptr) && (width > 0) && (height > 0) && (channelCount > 0));
    unsigned char* data = nullptr;
    size_t sizeInBytes = size_t(width) * size_t(height) *
        size_t(channelCount) * sizeof(unsigned char);
    if (m_data == nullptr)
    {
        data = (unsigned char*)STBI_MALLOC(sizeInBytes);
    }
    else
    {
        data = (unsigned char*)STBI_REALLOC(m_data, sizeInBytes);
    }

    if (data)
    {
        m_data = data;
        m_sizeInBytes = sizeInBytes;
        m_width = width;
        m_height = height;
        m_channelCount = channelCount;
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
        m_sizeInBytes = 0;
        m_width = 0;
        m_height = 0;
        m_channelCount = 0;
    }
}

bool ImageU8::WritePNG(std::string_view filename) const
{
    int result = stbi_write_png(filename.data(), m_width, m_height, m_channelCount, m_data, m_width * m_channelCount);
    return (result != 0); // returns 0 on failure and non-0 on success.
}

bool ImageU8::WritePNG(std::string_view filename, int left, int top, int right, int bottom) const
{
    assert(left > 0 && left < m_width && left < right && right < m_width);
    assert(top > 0 && top < m_height && top < bottom && bottom < m_height);
    int width = right - left;
    int height = bottom - top;
    int offset = (top * m_width + left) * m_channelCount;
    int result = stbi_write_png(filename.data(), width, height, m_channelCount,
        m_data + offset, m_width * m_channelCount);
    return (result != 0); // returns 0 on failure and non-0 on success.
}

bool ImageU8::WriteBMP(std::string_view filename) const
{
    int result = stbi_write_bmp(filename.data(), m_width, m_height, m_channelCount, m_data);
    return (result != 0); // returns 0 on failure and non-0 on success.
}

bool ImageU8::WriteTGA(std::string_view filename) const
{
    int result = stbi_write_tga(filename.data(), m_width, m_height, m_channelCount, m_data);
    return (result != 0); // returns 0 on failure and non-0 on success.
}

bool ImageU8::WriteJPG(std::string_view filename, int quality) const
{
    int result = stbi_write_png(filename.data(), m_width, m_height, m_channelCount, m_data, quality);
    return (result != 0); // returns 0 on failure and non-0 on success.
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
    return GetImageInfo(filename, m_width, m_height, m_channelCount);
}

bool ImageFP32::Load(std::string_view filename, int channelCount, float gamma, float scale)
{
    assert(m_data == nullptr);
    stbi_ldr_to_hdr_gamma(gamma);
    stbi_ldr_to_hdr_scale(scale);
    m_data = stbi_loadf(filename.data(), &m_width, &m_height, &m_channelCount, channelCount);
    if (m_data)
    {
        m_sizeInBytes = size_t(m_width) * size_t(m_height) *
            size_t(m_channelCount) * sizeof(float);
        return true;
    }
    else
    {
        return false;
    }
}

bool ImageFP32::Allocate(int width, int height, int channelCount)
{
    assert((m_data == nullptr) && (width > 0) && (height > 0) && (channelCount > 0));
    float* data = nullptr;
    size_t sizeInBytes = size_t(width) * size_t(height) *
        size_t(channelCount) * sizeof(float);
    if (m_data == nullptr)
    {
        data = (float*)STBI_MALLOC(sizeInBytes);
    }
    else
    {
        data = (float*)STBI_REALLOC(m_data, sizeInBytes);
    }

    if (data)
    {
        m_data = data;
        m_sizeInBytes = sizeInBytes;
        m_width = width;
        m_height = height;
        m_channelCount = channelCount;
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
        m_sizeInBytes = 0;
        m_width = 0;
        m_height = 0;
        m_channelCount = 0;
    }
}

bool ImageFP32::WriteHDR(std::string_view filename) const
{
    int result = stbi_write_hdr(filename.data(), m_width, m_height, m_channelCount, m_data);
    return (result != 0); // returns 0 on failure and non-0 on success.
}

} // namespace rad
