#include "Integer.h"

#if defined(_WIN32)
#include <intrin.h>
#endif

NAMESPACE_RAD_BEGIN

uint32_t BitScanReverse32(uint32_t mask)
{
    assert(mask != 0);
#if defined(_WIN32)
    unsigned long index = 0;
    ::_BitScanReverse(&index, static_cast<uint32_t>(mask));
    return static_cast<uint32_t>(index);
#elif defined(__GNUC__)
    static_assert(sizeof(uint32_t) == sizeof(unsigned int));
    return (31u - __builtin_clz(static_cast<uint32_t>(mask)));
#else
    uint32_t index = 31u;
    mask |= 0x1; // index=0 if mask=0
    while (((mask >> index) & 0x1) == 0)
    {
        --index;
    }
    return index;
#endif
}

uint32_t BitScanReverse64(uint64_t mask)
{
    assert(mask != 0);
#if defined(_WIN64)
    unsigned long index = 0;
    ::_BitScanReverse64(&index, mask);
    return static_cast<uint32_t>(index);
#elif defined(_WIN32)
    unsigned long index = 0;
    const uint32_t highPart = HighPart64(mask);
    ::_BitScanReverse(&index, (highPart != 0) ? highPart : LowPart64(mask));
    return (highPart != 0) ? (index + 32u) : index;
#elif defined(__GNUC__)
    static_assert(sizeof(uint64_t) == sizeof(unsigned long long));
    return (63u - __builtin_clzll(mask));
#else
    uint32_t index = 63u;
    mask |= 0x1; // index=0 if mask=0
    while (((mask >> index) & 0x1) == 0)
    {
        --index;
    }
    return index;
#endif
}

uint32_t CountBits(uint32_t x)
{
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (((x + (x >> 4)) & 0x0F0F0F0F) * 0x01010101) >> ((sizeof(uint32_t) - 1) << 3);
    return x;
}

uint32_t RoundUpToNextPow2(uint32_t x)
{
    assert(x < 0x80000000);
    if (x > 0)
    {
#if defined(_WIN32) || defined(_WIN64) || defined(__GNUC__)
        return uint32_t(0x2) << BitScanReverse32(x);
#else // fallback
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        x |= x >> 32;
        return x + 1;
#endif
    }
    else
    {
        return 1;
    }
}

uint64_t RoundUpToNextPow2(uint64_t x)
{
    assert(x < 0x8000000000000000);
    if (x > 0)
    {
#if defined(_WIN32) || defined(_WIN64) || defined(__GNUC__)
        return uint64_t(0x2) << BitScanReverse64(x);
#else // fallback
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        x |= x >> 32;
        x |= x >> 64;
        return x + 1;
#endif
    }
    else
    {
        return 1;
    }
}

uint32_t RoundUpToPow2(uint32_t x)
{
    if (x > 1)
    {
        return RoundUpToNextPow2(x - 1);
    }
    else
    {
        return 1;
    }
}

uint64_t RoundUpToPow2(uint64_t x)
{
    if (x > 1)
    {
        return RoundUpToNextPow2(x - 1);
    }
    else
    {
        return 1;
    }
}

NAMESPACE_RAD_END
