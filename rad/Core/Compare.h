#pragma once

#include "Config.h"
#include <compare>

#if (__has_include(<compare>) && (__cpp_lib_three_way_comparison >= 201907L))
#define RAD_HAS_SPACESHIP_OPERATOR 1
#endif
