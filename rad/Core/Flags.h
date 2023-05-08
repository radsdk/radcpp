#pragma once

#include "Config.h"
#include "Compare.h"

NAMESPACE_RAD_BEGIN

// Inspired by vk::Flags: https://github.com/KhronosGroup/Vulkan-Hpp
// Specify mask type explicitly.
template <typename Mask, typename Bit>
class Flags
{
public:
    constexpr Flags() noexcept : m_mask(0) {}

    constexpr Flags(Bit bit) noexcept : m_mask(static_cast<Mask>(bit)) {}

    constexpr Flags(Flags<Mask, Bit> const& rhs) noexcept = default;

    constexpr explicit Flags(Mask flags) noexcept : m_mask(flags) {}

    constexpr bool HasBits(Mask bits) noexcept
    {
        return ((m_mask & bits) == bits);
    }

    constexpr bool HasNoBits(Mask bits) noexcept
    {
        return ((m_mask & bits) == 0);
    }

    constexpr bool HasAnyBits(Mask bits) noexcept
    {
        return ((m_mask & bits) != 0);
    }

    // relational operators
#if defined(RAD_HAS_SPACESHIP_OPERATOR)
    auto operator<=>(Flags<Mask, Bit> const&) const = default;
#else
    constexpr bool operator<(Flags<Mask, Bit> const& rhs) const noexcept
    {
        return m_mask < rhs.m_mask;
    }

    constexpr bool operator<=(Flags<Mask, Bit> const& rhs) const noexcept
    {
        return m_mask <= rhs.m_mask;
    }

    constexpr bool operator>(Flags<Mask, Bit> const& rhs) const noexcept
    {
        return m_mask > rhs.m_mask;
    }

    constexpr bool operator>=(Flags<Mask, Bit> const& rhs) const noexcept
    {
        return m_mask >= rhs.m_mask;
    }

    constexpr bool operator==(Flags<Mask, Bit> const& rhs) const noexcept
    {
        return m_mask == rhs.m_mask;
    }

    constexpr bool operator!=(Flags<Mask, Bit> const& rhs) const noexcept
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
    constexpr Flags<Mask, Bit> operator&(Flags<Mask, Bit> const& rhs) const noexcept
    {
        return Flags<Mask, Bit>(m_mask & rhs.m_mask);
    }

    constexpr Flags<Mask, Bit> operator|(Flags<Mask, Bit> const& rhs) const noexcept
    {
        return Flags<Mask, Bit>(m_mask | rhs.m_mask);
    }

    constexpr Flags<Mask, Bit> operator^(Flags<Mask, Bit> const& rhs) const noexcept
    {
        return Flags<Mask, Bit>(m_mask ^ rhs.m_mask);
    }

    constexpr Flags<Mask, Bit> operator~() const noexcept
    {
        return Flags<Mask, Bit>(~m_mask);
    }

    // assignment operators
    constexpr Flags<Mask, Bit>& operator=(Flags<Mask, Bit> const& rhs) noexcept = default;

    constexpr Flags<Mask, Bit>& operator|=(Flags<Mask, Bit> const& rhs) noexcept
    {
        m_mask |= rhs.m_mask;
        return *this;
    }

    constexpr Flags<Mask, Bit>& operator&=(Flags<Mask, Bit> const& rhs) noexcept
    {
        m_mask &= rhs.m_mask;
        return *this;
    }

    constexpr Flags<Mask, Bit>& operator^=(Flags<Mask, Bit> const& rhs) noexcept
    {
        m_mask ^= rhs.m_mask;
        return *this;
    }

    // cast operators
    constexpr operator bool() const noexcept
    {
        return !!m_mask;
    }

    constexpr operator Mask() const noexcept
    {
        return m_mask;
    }

public:
    Mask m_mask;

}; // class Flags

template<typename Bit>
using Flags32 = Flags<uint32_t, Bit>;

template<typename Bit>
using Flags64 = Flags<uint64_t, Bit>;

#if !defined(RAD_HAS_SPACESHIP_OPERATOR)

// relational operators only needed for pre C++20
template <typename Mask, typename Bit>
constexpr bool operator<(Bit bit, Flags<Mask, Bit> const& flags) noexcept
{
    return flags.operator>(bit);
}

template <typename Mask, typename Bit>
constexpr bool operator<=(Bit bit, Flags<Mask, Bit> const& flags) noexcept
{
    return flags.operator>=(bit);
}

template <typename Mask, typename Bit>
constexpr bool operator>(Bit bit, Flags<Mask, Bit> const& flags) noexcept
{
    return flags.operator<(bit);
}

template <typename Mask, typename Bit>
constexpr bool operator>=(Bit bit, Flags<Mask, Bit> const& flags) noexcept
{
    return flags.operator<=(bit);
}

template <typename Mask, typename Bit>
constexpr bool operator==(Bit bit, Flags<Mask, Bit> const& flags) noexcept
{
    return flags.operator==(bit);
}

template <typename Mask, typename Bit>
constexpr bool operator!=(Bit bit, Flags<Mask, Bit> const& flags) noexcept
{
    return flags.operator!=(bit);
}

#endif // !defined(RAD_HAS_SPACESHIP_OPERATOR)

// bitwise operators
template <typename Mask, typename Bit>
constexpr Flags<Mask, Bit> operator&(Bit bit, Flags<Mask, Bit> const& flags) noexcept
{
    return flags.operator&(bit);
}

template <typename Mask, typename Bit>
constexpr Flags<Mask, Bit> operator|(Bit bit, Flags<Mask, Bit> const& flags) noexcept
{
    return flags.operator|(bit);
}

template <typename Mask, typename Bit>
constexpr Flags<Mask, Bit> operator^(Bit bit, Flags<Mask, Bit> const& flags) noexcept
{
    return flags.operator^(bit);
}

NAMESPACE_RAD_END
