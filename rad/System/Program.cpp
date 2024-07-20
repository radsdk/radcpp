#include <rad/System/Program.h>
#include <rad/System/StackTrace.h>
#include <backward.hpp>

namespace rad
{

Program::Program(int argc, char** argv)
{
}

Program::~Program()
{
}

bool Program::InstallDefaultSignalHandlers()
{
    backward::SignalHandling signalHandling;
    return signalHandling.loaded();
}

} // namespace rad
