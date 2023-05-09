#pragma once

#include "rad/Core/Config.h"
#include <cmath>
#include <numbers>
#include <complex>
#include <valarray>
#include <numeric>

// https://www.pbr-book.org/3ed-2018/Shapes/Managing_Rounding_Error#x1-ErrorPropagation
template<typename Float>
constexpr Float GetFloatingErrorGamma(int n)
{
    const Float MachineEpsilon = std::numeric_limits<Float>::epsilon() * 0.5f;
    return (n * MachineEpsilon) / (1 - n * MachineEpsilon);
}
