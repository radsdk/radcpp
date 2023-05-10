#pragma once

#include "rad/Core/Common.h"
#include "rad/Core/String.h"
#include <cassert>
#include <cstdint>

#include "cpu_features_macros.h"

#if defined(CPU_FEATURES_ARCH_X86)
#include "cpuinfo_x86.h"
#elif defined(CPU_FEATURES_ARCH_ARM)
#include "cpuinfo_arm.h"
#elif defined(CPU_FEATURES_ARCH_AARCH64)
#include "cpuinfo_aarch64.h"
#elif defined(CPU_FEATURES_ARCH_MIPS)
#include "cpuinfo_mips.h"
#elif defined(CPU_FEATURES_ARCH_PPC)
#include "cpuinfo_ppc.h"
#elif defined(CPU_FEATURES_ARCH_S390X)
#include "cpuinfo_s390x.h"
#elif defined(CPU_FEATURES_ARCH_RISCV)
#include "cpuinfo_riscv.h"
#endif
