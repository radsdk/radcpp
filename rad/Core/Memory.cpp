#include <rad/Core/Memory.h>
#include <cassert>
#include <cstdlib>

namespace rad
{

void* AlignedAlloc(std::size_t size, std::size_t alignment)
{
    assert(size % alignment == 0);
#if defined(RAD_OS_WINDOWS)
    return _aligned_malloc(size, alignment);
#else
    return std::aligned_alloc(alignment, size);
#endif
}

void AlignedFree(void* p)
{
#if defined(RAD_OS_WINDOWS)
    _aligned_free(p);
#else
    std::free(p);
#endif
}

} // namespace rad
