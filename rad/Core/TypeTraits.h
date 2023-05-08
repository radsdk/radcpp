#pragma once

#include "Config.h"
#include <type_traits>

NAMESPACE_RAD_BEGIN

template<class T>
constexpr auto UnderlyingCast(T t) noexcept
{
    return static_cast<std::underlying_type_t<T>>(t);
}

template <typename...>
struct MaxSizeof;

template <>
struct MaxSizeof<>
{
    static constexpr uint32_t value = 0;
};

template <typename T, typename... Rest>
struct MaxSizeof<T, Rest...>
{
    static constexpr uint32_t value = sizeof(T) > MaxSizeof<Rest...>::value ? sizeof(T) : MaxSizeof<Rest...>::value;
};

NAMESPACE_RAD_END
