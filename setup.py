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

def generate_project_files():
    if sys.platform == "win32":
        vcpkg_root = os.environ["VCPKG_ROOT"]
        vcpkg_toolchain_file = vcpkg_root + "/scripts/buildsystems/vcpkg.cmake"
        shell(f"cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=\"{vcpkg_toolchain_file}\"")

def main() -> int:
    try:
        pushd(script_root)
        tasks = ["SDL", "SDL_mixer", "spvgen"]
        if (len(sys.argv) > 1) and (not "all" in sys.argv):
            tasks = sys.argv[1:]
        if "SDL" in tasks:
            build_SDL()
        if "SDL_mixer" in tasks:
            build_SDL_mixer()
        if "imgui" in tasks:
            clone_imgui()
        generate_project_files()
        popd()
        return 0
    except Exception as e:
        print(e)
        return -1

if __name__ == '__main__':
    sys.exit(main())
