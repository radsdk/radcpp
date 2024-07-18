#include <rad/Core/Platform.h>
#include <iostream>
#include <format>

int main(int argc, char* argv[])
{
    for (int i = 0; i < argc; ++i)
    {
        std::cout << std::format("argv[{}]: {}", i, argv[i]) << std::endl;
    }
    std::cout << "Hello, World" << std::endl;

    return 0;
}
