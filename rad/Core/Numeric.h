#pragma once

#include <rad/Core/Platform.h>
#include <numeric>
#include <boost/numeric/conversion/cast.hpp>

namespace rad
{

using boost::numeric_cast;
using boost::numeric::bad_numeric_cast;
using boost::numeric::positive_overflow;
using boost::numeric::negative_overflow;

} // namespace rad
