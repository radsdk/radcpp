#pragma once

#include "rad/Core/Platform.h"
#include "rad/Core/String.h"
#include "rad/Core/RefCounted.h"
#if defined(RAD_OS_WINDOWS)
#define STBI_WINDOWS_UTF8
#endif
#include "imported/stb/stb_image.h"
#include "imported/stb/stb_image_write.h"
#include "imported/stb/stb_image_resize2.h"

namespace rad
{

// Image is a rectangle of pixels stored from left-to-right, top-to-bottom.

bool GetImageInfo(std::string_view filename, int& width, int& height, int& channelCount);

// 8-bits unorm data.
class ImageU8
{
public:
    ImageU8();
    ~ImageU8();

    bool GetInfo(std::string_view filename);
    // Load image into 8-bits unorm format, gamma and scale for hdr to ldr convertion.
    bool Load(std::string_view filename, int channelCount, float gamma, float scale);

    bool Malloc(size_t size);
    bool Realloc(size_t size);
    void Free();

    bool WritePNG(std::string_view filename) const;
    // PNG can write a sub-region.
    bool WritePNG(std::string_view filename, int left, int top, int right, int bottom) const;
    bool WriteBMP(std::string_view filename) const;
    bool WriteTGA(std::string_view filename) const;
    // JPEG does ignore alpha channels in input data; quality is between 1 and 100.
    bool WriteJPG(std::string_view filename, int quality) const;

    unsigned char* m_data = nullptr;
    int m_width = 0;
    int m_height = 0;
    int m_channelCount = 0;
    bool m_isHdr = false;
}; // class ImageU8

// FP32 HDR data.
class ImageFP32
{
public:
    ImageFP32();
    ~ImageFP32();
    bool GetInfo(std::string_view filename);
    // Load image into (linear) floats to preserve the full dynamic range,
    // gamma and scale for ldr to hdr convertion.
    bool Load(std::string_view filename, int channelCount, float gamma, float scale);

    bool Malloc(size_t size);
    bool Realloc(size_t size);
    void Free();

    // HDR expects linear float data. Since the format is always 32-bit rgb(e) data, alpha (if provided) is discarded,
    // and for monochrome data it is replicated across all three channels.
    bool WriteHDR(std::string_view filename) const;

    float* m_data = nullptr;
    int m_width = 0;
    int m_height = 0;
    int m_channelCount = 0;
    bool m_isHdr = false;
}; // class ImageFP32

} // namespace rad
