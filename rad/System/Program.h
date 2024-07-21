#pragma once

#include <rad/Core/Platform.h>
#include <rad/Core/RefCounted.h>
#include <rad/Core/String.h>

namespace rad
{

class Program : public RefCounted<Program>
{
public:
    Program();
    ~Program();

    bool Init(int argc, char** argv);
    void PrintStackTrace(int depth = 32);

    // Causes normal program termination to occur.
    // Functions registered with std::atexit are called in the reverse order of their registration.
    void Exit(int code);
    // Causes normal program termination to occur without completely cleaning the resources.
    // Functions passed to std::at_quick_exit are called in reverse order of their registration.
    void QuickExit(int code);
    // In any case, std::terminate calls the currently installed std::terminate_handler.
    // The default std::terminate_handler calls std::abort.
    void Terminate();
    // Causes abnormal program termination (without cleaning up)
    // unless SIGABRT is being caught by a signal handler.
    void Abort();

private:
    bool InstallDefaultSignalHandlers();
    bool LogSystemInfo();

}; // Program

Program* GetProgram();

} // namespace rad
