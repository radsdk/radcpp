#pragma once

#include "rad/Core/Common.h"
#include <cmath>

NAMESPACE_RAD_BEGIN

// Finds solutions of the quadratic equation at^2 + bt + c = 0; return true if solutions were found.
bool SolveQuadraticEquation(float a, float b, float c, float& t0, float& t1);

NAMESPACE_RAD_END
