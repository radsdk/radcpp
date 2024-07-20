#pragma once

#include <rad/Core/Platform.h>
#include <rad/Core/Integer.h>
#include <rad/Core/TypeTraits.h>
#include <compare>

namespace rad
{

// Inspired by vk::Flags: https://github.com/KhronosGroup/Vulkan-Hpp
template <std::integral Mask, Enumeration Bits>
class Flags
{
public:
    constexpr Flags() noexcept : m_mask(0) {}

    constexpr explicit Flags(Mask mask) noexcept : m_mask(mask) {}
    constexpr Flags(Bits bit) noexcept : m_mask(static_cast<Mask>(bit)) {}
    constexpr Flags(Flags<Mask, Bits> const& rhs) noexcept = default;

    Mask GetMask() const
    {
        return m_mask;
    }

    operator Mask() const { return m_mask; }
    Mask* operator&() { return &m_mask; }
    const Mask* operator&() const { return &m_mask; }

    void SetMask(Mask mask)
    {
        m_mask = mask;
    }

    void SetBit(Bits bit)
    {
        assert(std::has_single_bit(rad::ToUnderlying(bit)));
        m_mask |= bit;
    }

    void Clear()
    {
        m_mask = 0;
    }

    constexpr bool HasBits(Mask bits) const noexcept
    {
        return ((m_mask & bits) == bits);
    }

    constexpr bool HasNoBits(Mask bits) const noexcept
    {
        return ((m_mask & bits) == 0);
    }

    constexpr bool HasAnyBits(Mask bits) const noexcept
    {
        return ((m_mask & bits) != 0);
    }

    int Popcount() const
    {
        return std::popcount(m_mask);
    }

    // relational operators
#if __cpp_lib_three_way_comparison >= 201907L
    auto operator<=>(Flags<Mask, Bits> const&) const = default;
#else
    constexpr bool operator<(Flags<Mask, Bits> const& rhs) const noexcept
    {
        return m_mask < rhs.m_mask;
    }

    constexpr bool operator<=(Flags<Mask, Bits> const& rhs) const noexcept
    {
        return m_mask <= rhs.m_mask;
    }

    constexpr bool operator>(Flags<Mask, Bits> const& rhs) const noexcept
    {
        return m_mask > rhs.m_mask;
    }

    constexpr bool operator>=(Flags<Mask, Bits> const& rhs) const noexcept
    {
        return m_mask >= rhs.m_mask;
    }

    constexpr bool operator==(Flags<Mask, Bits> const& rhs) const noexcept
    {
        return m_mask == rhs.m_mask;
    }

    constexpr bool operator!=(Flags<Mask, Bits> const& rhs) const noexcept
    {
        return m_mask != rhs.m_mask;
    }
#endif

    // logical operator
    constexpr bool operator!() const noexcept
    {
        return !m_mask;
    }

    // bitwise operators
    constexpr Flags<Mask, Bits> operator&(Flags<Mask, Bits> const& rhs) const noexcept
    {
        return Flags<Mask, Bits>(m_mask & rhs.m_mask);
    }

    constexpr Flags<Mask, Bits> operator|(Flags<Mask, Bits> const& rhs) const noexcept
    {
        return Flags<Mask, Bits>(m_mask | rhs.m_mask);
    }

    constexpr Flags<Mask, Bits> operator^(Flags<Mask, Bits> const& rhs) const noexcept
    {
        return Flags<Mask, Bits>(m_mask ^ rhs.m_mask);
    }

    constexpr Flags<Mask, Bits> operator~() const noexcept
    {
        return Flags<Mask, Bits>(~m_mask);
    }

    // assignment operators
    constexpr Flags<Mask, Bits>& operator=(Flags<Mask, Bits> const& rhs) noexcept = default;

    constexpr Flags<Mask, Bits>& operator|=(Flags<Mask, Bits> const& rhs) noexcept
    {
        m_mask |= rhs.m_mask;
        return *this;
    }

    constexpr Flags<Mask, Bits>& operator&=(Flags<Mask, Bits> const& rhs) noexcept
    {
        m_mask &= rhs.m_mask;
        return *this;
    }

    constexpr Flags<Mask, Bits>& operator^=(Flags<Mask, Bits> const& rhs) noexcept
    {
        m_mask ^= rhs.m_mask;
        return *this;
    }

    // cast operators
    constexpr explicit operator bool() const noexcept
    {
        return !!m_mask;
    }

private:
    Mask m_mask;

}; // class Flags

template<typename Bits>
using Flags32 = Flags<uint32_t, Bits>;

template<typename Bits>
using Flags64 = Flags<uint64_t, Bits>;

#if __cpp_lib_three_way_comparison >= 201907L

// relational operators only needed for pre C++20
template <typename Mask, typename Bits>
constexpr bool operator<(Bits bit, Flags<Mask, Bits> const& flags) noexcept
{
    return flags.operator>(bit);
}

template <typename Mask, typename Bits>
constexpr bool operator<=(Bits bit, Flags<Mask, Bits> const& flags) noexcept
{
    return flags.operator>=(bit);
}

template <typename Mask, typename Bits>
constexpr bool operator>(Bits bit, Flags<Mask, Bits> const& flags) noexcept
{
    return flags.operator<(bit);
}

template <typename Mask, typename Bits>
constexpr bool operator>=(Bits bit, Flags<Mask, Bits> const& flags) noexcept
{
    return flags.operator<=(bit);
}

template <typename Mask, typename Bits>
constexpr bool operator==(Bits bit, Flags<Mask, Bits> const& flags) noexcept
{
    return flags.operator==(bit);
}

template <typename Mask, typename Bits>
constexpr bool operator!=(Bits bit, Flags<Mask, Bits> const& flags) noexcept
{
    return flags.operator!=(bit);
}

#endif // __cpp_lib_three_way_comparison >= 201907L

// bitwise operators
template <typename Mask, typename Bits>
constexpr Flags<Mask, Bits> operator&(Bits bit, Flags<Mask, Bits> const& flags) noexcept
{
    return flags.operator&(bit);
}

template <typename Mask, typename Bits>
constexpr Flags<Mask, Bits> operator|(Bits bit, Flags<Mask, Bits> const& flags) noexcept
{
    return flags.operator|(bit);
}

template <typename Mask, typename Bits>
constexpr Flags<Mask, Bits> operator^(Bits bit, Flags<Mask, Bits> const& flags) noexcept
{
    return flags.operator^(bit);
}

} // namespace rad
