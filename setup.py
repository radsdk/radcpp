import os
import sys
import subprocess
import shutil

script_root = os.path.dirname(os.path.realpath(__file__))
working_dir_stack = list()

def pushd(dir):
    working_dir_stack.append(os.getcwd())
    os.chdir(dir)
    print("Working dir:", os.getcwd())

def popd():
    os.chdir(working_dir_stack.pop())
    print("Working dir:", os.getcwd())

def shell(command : str, env = os.environ):
    print("Execute:", command)
    subprocess.run(command, shell=True, env=env)

def remove_dir(dir : str):
    if os.path.isdir(str):
        shutil.rmtree(dir)

def build_SDL():
    pushd("imported")
    if not os.path.exists("SDL"):
        shell("git clone https://github.com/libsdl-org/SDL.git")
        pushd("SDL")
        shell("git reset --hard f244f08165d08bc9cbef460aaec11e70ba43d8b6")
        popd()
    build_dir = script_root + "/imported/build/SDL"
    install_dir = script_root + "/imported/installed/SDL"
    shell(f"cmake -S SDL -B \"{build_dir}\" -DCMAKE_INSTALL_PREFIX=\"{install_dir}\"")
    shell(f"cmake --build \"{build_dir}\" --target install --config Release")
    popd()

def build_SDL_mixer():
    pushd("imported")
    if not os.path.exists("SDL_mixer"):
        shell("git clone https://github.com/libsdl-org/SDL_mixer.git")
        pushd("SDL_mixer")
        shell("git reset --hard 96ea69fce472dcef7c638199aef8e3c81200573a")
        popd()
    pushd("SDL_mixer")
    shell("git submodule update --init --recursive")
    popd() # imported/
    build_dir = script_root + "/imported/build/SDL_mixer"
    install_dir = script_root + "/imported/installed/SDL_mixer"
    sdl3_dir = script_root + "/imported/installed/SDL"
    shell(f"cmake -S SDL_mixer -B \"{build_dir}\" -DSDL3MIXER_MP3_MPG123=ON -DCMAKE_INSTALL_PREFIX=\"{install_dir}\"",
                   env=dict(os.environ, SDL3_DIR=sdl3_dir))
    shell(f"cmake --build \"{build_dir}\" --target install --config Release")
    popd()

def clone_imgui():
    pushd("imported")
    if not os.path.exists("imgui"):
        shell("git clone --depth=1 https://github.com/ocornut/imgui.git")
        pushd("imgui")
        shell("git reset --hard d42fa46dc6b68863046c22467297933dbe39bb55")
        popd()
    popd()


def clone_vulkan_libraries():
    pushd("imported")
    if not os.path.exists("Vulkan-Headers"):
        shell("git clone https://github.com/KhronosGroup/Vulkan-Headers.git")
        pushd("Vulkan-Headers")
        shell("git reset --hard fabe9e2672334fdb9a622d42a2e8f94578952082")
        popd()
    if not os.path.exists("volk"):
        shell("git clone https://github.com/zeux/volk.git")
        pushd("volk")
        shell("git reset --hard 12e006f60f6f10bc92205612d6875ee539c354ad")
        popd()
    if not os.path.exists("VulkanMemoryAllocator"):
        shell("git clone https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git")
        pushd("VulkanMemoryAllocator")
        shell("git reset --hard 871913da6a4b132b567d7b65c509600363c0041e")
        popd()
    popd()

def build_spvgen():
    pushd("imported")
    if not os.path.exists("spvgen"):
        shell("git clone https://github.com/GPUOpen-Drivers/spvgen.git")
        pushd("spvgen")
        shell("git reset --hard e044c05c3e8694aa57bc438a3c24883c8decba4f")
        popd()
    pushd("spvgen/external")
    shell("python fetch_external_sources.py")
    popd()
    pushd("spvgen")
    shell("cmake -S . -B build")
    shell("cmake --build build/ --target spvgen --config Release")
    popd()
    popd()

def generate_project_files():
    if sys.platform == "win32":
        vcpkg_root = os.environ["VCPKG_ROOT"]
        vcpkg_toolchain_file = vcpkg_root + "/scripts/buildsystems/vcpkg.cmake"
        shell(f"cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=\"{vcpkg_toolchain_file}\"")

def main() -> int:
    try:
        pushd(script_root)
        tasks = ["SDL", "SDL_mixer", "vulkan"]
        if (len(sys.argv) > 1) and (not "all" in sys.argv):
            tasks = sys.argv[1:]
        if "SDL" in tasks:
            build_SDL()
        if "SDL_mixer" in tasks:
            build_SDL_mixer()
        if "imgui" in tasks:
            clone_imgui()
        if "vulkan" in tasks:
            clone_vulkan_libraries()
            build_spvgen()
        generate_project_files()
        popd()
        return 0
    except Exception as e:
        print(e)
        return -1

if __name__ == '__main__':
    sys.exit(main())
