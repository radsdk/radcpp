#include "CpuInfo.h"
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
#elif defined(CPU_FEATURES_ARCH_LOONGARCH)
#include "cpuinfo_loongarch.h"
#endif

namespace rad
{

using namespace cpu_features;

#if defined(CPU_FEATURES_ARCH_X86)
static const X86Info g_X86Info = GetX86Info();
static const CacheInfo g_CacheInfo = GetX86CacheInfo();
const char* GetCpuVendor()
{
    return g_X86Info.vendor;
}
const char* GetCpuBrandString()
{
    return g_X86Info.brand_string;
}
#elif defined(CPU_FEATURES_ARCH_ARM)
static const ArmInfo g_ArmInfo = GetArmInfo();
#elif defined(CPU_FEATURES_ARCH_AARCH64)
static const Aarch64Info g_Aarch64Info = GetAarch64Info();
#elif defined(CPU_FEATURES_ARCH_MIPS)
static const MipsInfo g_MipsInfo = GetMipsInfo();
#elif defined(CPU_FEATURES_ARCH_PPC)
static const PPCInfo g_PPCInfo = GetPPCInfo();
static const PPCPlatformStrings g_PPCPlatformStrings = GetPPCPlatformStrings();
#elif defined(CPU_FEATURES_ARCH_S390X)
static const S390XInfo g_S390XInfo = GetS390XInfo();
static const S390XPlatformStrings g_S390XPlatformStrings = GetS390XPlatformStrings();
#elif defined(CPU_FEATURES_ARCH_RISCV)
static const RiscvInfo g_RiscvInfo = GetRiscvInfo();
#elif defined(CPU_FEATURES_ARCH_LOONGARCH)
static const LoongArchInfo g_LoongArchInfo = GetLoongArchInfo();
#endif

} // namespace rad
