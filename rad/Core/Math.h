#pragma once

#include <rad/Core/Platform.h>
#include <cmath>
#include <numbers>
// e = e_v<double>;
// log2e = log2e_v<double>;
// log10e = log10e_v<double>;
// pi = pi_v<double>;
// inv_pi = inv_pi_v<double>; // 1/pi
// inv_sqrtpi = inv_sqrtpi_v<double>; // 1/sqrt(pi)
// ln2 = ln2_v<double>;
// ln10 = ln10_v<double>;
// sqrt2 = sqrt2_v<double>;
// sqrt3 = sqrt3_v<double>;
// inv_sqrt3 = inv_sqrt3_v<double>; // 1/sqrt(3)
// egamma = egamma_v<double>; // the Euler-Mascheroni constant: https://en.wikipedia.org/wiki/Euler%27s_constant
// phi = phi_v<double>; // the golden ratio: (1+sqrt(5))/2 = 1.618033988749...

namespace rad
{

// Finds solutions of the quadratic equation at^2 + bt + c = 0; return true if solutions were found.
bool SolveQuadraticEquation(float a, float b, float c, float& t0, float& t1);

// The Right Way to Calculate Stuff: https://www.plunk.org/~hatch/rightway.html

} // namespace rad
