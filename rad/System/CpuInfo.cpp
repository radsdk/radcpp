#include <rad/System/CpuInfo.h>

namespace rad
{

#if defined(CPU_FEATURES_ARCH_X86)
const X86Info g_X86Info = GetX86Info();
const CacheInfo g_CacheInfo = GetX86CacheInfo();
#elif defined(CPU_FEATURES_ARCH_ARM)
const ArmInfo g_ArmInfo = GetArmInfo();
#elif defined(CPU_FEATURES_ARCH_AARCH64)
const Aarch64Info g_Aarch64Info = GetAarch64Info();
#elif defined(CPU_FEATURES_ARCH_MIPS)
const MipsInfo g_MipsInfo = GetMipsInfo();
#elif defined(CPU_FEATURES_ARCH_PPC)
const PPCInfo g_PPCInfo = GetPPCInfo();
const PPCPlatformStrings g_PPCPlatformStrings = GetPPCPlatformStrings();
#elif defined(CPU_FEATURES_ARCH_S390X)
const S390XInfo g_S390XInfo = GetS390XInfo();
const S390XPlatformStrings g_S390XPlatformStrings = GetS390XPlatformStrings();
#elif defined(CPU_FEATURES_ARCH_RISCV)
const RiscvInfo g_RiscvInfo = GetRiscvInfo();
#elif defined(CPU_FEATURES_ARCH_LOONGARCH)
const LoongArchInfo g_LoongArchInfo = GetLoongArchInfo();
#endif

} // namespace rad
