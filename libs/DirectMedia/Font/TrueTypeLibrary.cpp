#include "TrueTypeLibrary.h"
#include "TrueTypeFont.h"
#include "rad/IO/Logging.h"

namespace sdl
{

TrueTypeLibrary::TrueTypeLibrary()
{
    Init();
}

TrueTypeLibrary::~TrueTypeLibrary()
{
    if (TTF_WasInit())
    {
        Destroy();
    }
}

rad::Ref<TrueTypeFont> TrueTypeLibrary::CreateTrueTypeFont()
{
    return new TrueTypeFont(this);
}

bool TrueTypeLibrary::Init()
{
    int res = TTF_Init();
    if (res == 0)
    {
        m_pVersion = TTF_Linked_Version();
        LogGlobal(Info, "TrueTypeLibrary initialized: SDL TTF Version %d.%d.%d",
            m_pVersion->major, m_pVersion->minor, m_pVersion->patch);
        return true;
    }
    else
    {
        LogGlobal(Error, "TTF_Init failed: %s", TTF_GetError());
        return false;
    }
}

void TrueTypeLibrary::Destroy()
{
    TTF_Quit();
}

} // namespace sdl
