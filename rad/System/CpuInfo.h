#pragma once

#include <rad/Core/Platform.h>

// https://github.com/google/cpu_features
#include <cpu_features_macros.h>

#if defined(CPU_FEATURES_ARCH_X86)
#include <cpuinfo_x86.h>
#elif defined(CPU_FEATURES_ARCH_ARM)
#include <cpuinfo_arm.h>
#elif defined(CPU_FEATURES_ARCH_AARCH64)
#include <cpuinfo_aarch64.h>
#elif defined(CPU_FEATURES_ARCH_MIPS)
#include <cpuinfo_mips.h>
#elif defined(CPU_FEATURES_ARCH_PPC)
#include <cpuinfo_ppc.h>
#elif defined(CPU_FEATURES_ARCH_S390X)
#include <cpuinfo_s390x.h>
#elif defined(CPU_FEATURES_ARCH_RISCV)
#include <cpuinfo_riscv.h>
#elif defined(CPU_FEATURES_ARCH_LOONGARCH)
#include <cpuinfo_loongarch.h>
#endif

namespace rad
{

using namespace cpu_features;

#if defined(CPU_FEATURES_ARCH_X86)
extern const X86Info g_X86Info;
extern const CacheInfo g_CacheInfo;
#elif defined(CPU_FEATURES_ARCH_ARM)
extern const ArmInfo g_ArmInfo;
#elif defined(CPU_FEATURES_ARCH_AARCH64)
extern const Aarch64Info g_Aarch64Info;
#elif defined(CPU_FEATURES_ARCH_MIPS)
extern const MipsInfo g_MipsInfo;
#elif defined(CPU_FEATURES_ARCH_PPC)
extern const PPCInfo g_PPCInfo;
extern const PPCPlatformStrings g_PPCPlatformStrings;
#elif defined(CPU_FEATURES_ARCH_S390X)
extern const S390XInfo g_S390XInfo;
extern const S390XPlatformStrings g_S390XPlatformStrings;
#elif defined(CPU_FEATURES_ARCH_RISCV)
extern const RiscvInfo g_RiscvInfo;
#elif defined(CPU_FEATURES_ARCH_LOONGARCH)
extern const LoongArchInfo g_LoongArchInfo;
#endif

} // namespace rad
