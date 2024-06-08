#include "rad/Core/Platform.h"
#include <iostream>
#include <format>

int main(int argc, char* argv[])
{
    std::cout << std::format("Hello, {}!", "World") << std::endl;
    return 0;
}
