# radcpp
Great C++ collections.

## Build

- Clone:
    ```PowerShell
    # Git 2.13 and later:
    git clone --recurse-submodules -j8 https://github.com/radsdk/radcpp.git
    # Update submodules:
    git submodule update --init --recursive
    ```
- Setup [vcpkg](https://github.com/microsoft/vcpkg) and install the following packages:
    - boost
    - imath
    - spdlog
    - cpu-features
- Generate project files with CMake:
    ```PowerShell
    # Replace $env:VCPKG_ROOT with your vcpkg path.
    cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
    ```
- Build:
    ```PowerShell
    # Build rad:
    cmake --build build/ --target rad --config Debug
    # Build and run HelloWorld:
    cmake --build build/ --target HelloWorld --config Debug
    build/tests/HelloWorld/Debug/HelloWorld
    ```
