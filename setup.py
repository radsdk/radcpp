import os
import sys
import subprocess
import shutil

working_dir_before_execute = os.getcwd()
script_root = os.path.dirname(os.path.realpath(__file__))
working_dir_stack = list()

def chdir(path: str):
    os.chdir(path)
    print("Working dir:", os.getcwd())

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
    chdir(script_root + "/" + "imported")
    if not os.path.exists("SDL"):
        shell("git clone https://github.com/libsdl-org/SDL.git")
    chdir(script_root + "/" + "imported/SDL")
    shell("git reset --hard f244f08165d08bc9cbef460aaec11e70ba43d8b6")
    chdir(script_root + "/" + "imported")
    build_dir = script_root + "/imported/build/SDL"
    install_dir = script_root + "/imported/installed/SDL"
    shell(f"cmake -S SDL -B \"{build_dir}\" -DCMAKE_INSTALL_PREFIX=\"{install_dir}\"")
    shell(f"cmake --build \"{build_dir}\" --target install --config Release")

def build_SDL_mixer():
    chdir(script_root + "/" + "imported")
    if not os.path.exists("SDL_mixer"):
        shell("git clone https://github.com/libsdl-org/SDL_mixer.git")
    chdir(script_root + "/" + "imported/SDL_mixer")
    shell("git reset --hard 96ea69fce472dcef7c638199aef8e3c81200573a")
    shell("git submodule update --init --recursive")
    chdir(script_root + "/" + "imported")
    build_dir = script_root + "/imported/build/SDL_mixer"
    install_dir = script_root + "/imported/installed/SDL_mixer"
    sdl3_dir = script_root + "/imported/installed/SDL"
    shell(f"cmake -S SDL_mixer -B \"{build_dir}\" -DSDL3MIXER_MP3_MPG123=ON -DCMAKE_INSTALL_PREFIX=\"{install_dir}\"",
                   env=dict(os.environ, SDL3_DIR=sdl3_dir))
    shell(f"cmake --build \"{build_dir}\" --target install --config Release")

def clone_imgui():
    chdir(script_root + "/" + "imported")
    if not os.path.exists("imgui"):
        shell("git clone --depth=1 https://github.com/ocornut/imgui.git")
    chdir(script_root + "/" + "imported/imgui")
    shell("git reset --hard d42fa46dc6b68863046c22467297933dbe39bb55")

def clone_vulkan_libraries():
    chdir(script_root + "/" + "imported")
    if not os.path.exists("Vulkan-Headers"):
        shell("git clone https://github.com/KhronosGroup/Vulkan-Headers.git")
    chdir(script_root + "/" + "imported/Vulkan-Headers")
    shell("git reset --hard fabe9e2672334fdb9a622d42a2e8f94578952082")
    chdir(script_root + "/" + "imported")
    if not os.path.exists("volk"):
        shell("git clone https://github.com/zeux/volk.git")
    chdir(script_root + "/" + "imported/volk")
    shell("git reset --hard 12e006f60f6f10bc92205612d6875ee539c354ad")
    chdir(script_root + "/" + "imported")
    if not os.path.exists("VulkanMemoryAllocator"):
        shell("git clone https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git")
    chdir(script_root + "/" + "imported/VulkanMemoryAllocator")
    shell("git reset --hard 871913da6a4b132b567d7b65c509600363c0041e")

def build_spvgen():
    chdir(script_root + "/" + "imported")
    if not os.path.exists("spvgen"):
        shell("git clone https://github.com/GPUOpen-Drivers/spvgen.git")
    chdir(script_root + "/" + "imported/spvgen")
    shell("git reset --hard e044c05c3e8694aa57bc438a3c24883c8decba4f")
    chdir(script_root + "/" + "imported/spvgen/external")
    shell("python fetch_external_sources.py")
    chdir(script_root + "/" + "imported/spvgen")
    shell("cmake -S . -B build")
    shell("cmake --build build --target spvgen --config Release")

def generate_project_files():
    chdir(script_root)
    print("radcpp: generate project files ...")
    if sys.platform == "win32":
        vcpkg_root = os.environ["VCPKG_ROOT"]
        vcpkg_toolchain_file = vcpkg_root + "/scripts/buildsystems/vcpkg.cmake"
        shell(f"cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=\"{vcpkg_toolchain_file}\"")

def main() -> int:
    try:
        chdir(script_root)
        components = ["gui", "media", "vulkan"]
        if (len(sys.argv) > 1) and (not "all" in sys.argv):
            components = sys.argv[1:]
        print(f"radcpp: setup libraries for: {components}")
        if "gui" in components:
            build_SDL()
            clone_imgui()
        if "media" in components:
            build_SDL_mixer()
        if "vulkan" in components:
            clone_vulkan_libraries()
            build_spvgen()
        generate_project_files()
        chdir(working_dir_before_execute)
        return 0
    except Exception as e:
        print(e)
        chdir(working_dir_before_execute)
        return -1
    
if __name__ == '__main__':
    sys.exit(main())
