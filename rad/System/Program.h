#pragma once

#include <rad/Core/Platform.h>
#include <rad/Core/String.h>

namespace rad
{

class Program
{
public:
    Program(int argc, char** argv);
    ~Program();

    void PrintStackTrace(int depth = 32);

private:
    static bool InstallDefaultSignalHandlers();
    static bool LogSystemInfo();

}; // Program

Program* GetProgram();

} // namespace rad
