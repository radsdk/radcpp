#include <rad/Core/Integer.h>
#if defined(RAD_OS_WINDOWS)
#include <intrin.h>
#endif

namespace rad
{

uint32_t BitScanReverse32(uint32_t mask)
{
    assert(mask != 0);
#if defined(_WIN32)
    unsigned long index = 0;
    _BitScanReverse(&index, static_cast<uint32_t>(mask));
    return static_cast<uint32_t>(index);
#elif defined(__cpp_lib_bitops)
    static_assert(sizeof(uint32_t) == sizeof(unsigned int));
    return static_cast<uint32_t>(31 - std::countl_zero(static_cast<uint32_t>(mask)));
#else // fallback
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
    _BitScanReverse64(&index, mask);
    return static_cast<uint32_t>(index);
#elif defined(_WIN32)
    unsigned long index = 0;
    const uint32_t highPart = HighPart64(mask);
    _BitScanReverse(&index, (highPart != 0) ? highPart : LowPart64(mask));
    return (highPart != 0) ? (index + 32u) : index;
#elif defined(__cpp_lib_bitops)
    static_assert(sizeof(uint64_t) == sizeof(unsigned long long));
    return static_cast<uint32_t>(63 - std::countl_zero(mask));
#else // fallback
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
#if defined(__cpp_lib_bitops)
    return std::popcount(x);
#else
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (((x + (x >> 4)) & 0x0F0F0F0F) * 0x01010101) >> ((sizeof(uint32_t) - 1) << 3);
    return x;
#endif
}

uint64_t CountBits(uint64_t x)
{
#if defined(__cpp_lib_bitops)
    return std::popcount(x);
#else
    // https://en.wikipedia.org/wiki/Hamming_weight
    const uint64_t m1 = 0x5555555555555555;     // binary: 0101...
    const uint64_t m2 = 0x3333333333333333;     // binary: 00110011..
    const uint64_t m4 = 0x0f0f0f0f0f0f0f0f;     // binary:  4 zeros,  4 ones ...
    const uint64_t m8 = 0x00ff00ff00ff00ff;     // binary:  8 zeros,  8 ones ...
    const uint64_t m16 = 0x0000ffff0000ffff;    // binary: 16 zeros, 16 ones ...
    const uint64_t m32 = 0x00000000ffffffff;    // binary: 32 zeros, 32 ones
    const uint64_t h01 = 0x0101010101010101;    // the sum of 256 to the power of 0,1,2,3,...
    x = x - (x >> 1) & m1;          // put count of each 2 bits into those 2 bits
    x = (x & m2) + ((x >> 2) & m2); // put count of each 4 bits into those 4 bits
    x = (x + (x >> 4)) & m4;        // put count of each 8 bits into those 8 bits
    return (x * h01) >> 56;         // returns left 8 bits of x + (x<<8) + (x<<16) + (x<<24) + ...
#endif
}

uint32_t RoundUpToNextPow2(uint32_t x)
{
#if defined(__cpp_lib_int_pow2)
    return uint32_t(1) << std::bit_width(x);
#else
    assert(x < 0x80000000);
    if (x > 0)
    {
#if defined(RAD_OS_WINDOWS) || defined(__cpp_lib_bitops)
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
#endif
}

uint64_t RoundUpToNextPow2(uint64_t x)
{
#if defined(__cpp_lib_int_pow2)
    return uint64_t(1) << std::bit_width(x);
#else
    assert(x < 0x8000000000000000);
    if (x > 0)
    {
#if defined(RAD_OS_WINDOWS) || defined(__cpp_lib_bitops)
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
#endif
}

uint32_t RoundUpToPow2(uint32_t x)
{
#if defined(__cpp_lib_int_pow2)
    return std::bit_ceil(x);
#else
    if (x > 1)
    {
        return RoundUpToNextPow2(x - 1);
    }
    else
    {
        return 1;
    }
#endif
}

uint64_t RoundUpToPow2(uint64_t x)
{
#if defined(__cpp_lib_int_pow2)
    return std::bit_ceil(x);
#else
    if (x > 1)
    {
        return RoundUpToNextPow2(x - 1);
    }
    else
    {
        return 1;
    }
#endif
}

} // namespace rad
