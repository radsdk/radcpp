#pragma once

#include <rad/Core/Platform.h>

namespace rad
{

class Program
{
public:
    Program(int argc, char** argv);
    ~Program();

    static bool InstallDefaultSignalHandlers();

}; // Program

} // namespace rad
