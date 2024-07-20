#pragma once

#include <rad/Core/Platform.h>
#include <boost/container/small_vector.hpp>

namespace rad
{

template<class T, std::size_t N>
using SmallVector = boost::container::small_vector<T, N>;

}
