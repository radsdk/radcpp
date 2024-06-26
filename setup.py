import os
import sys
import subprocess
import shutil

script_root = os.path.dirname(os.path.realpath(__file__))
working_dir_stack = list()

def pushd(dir):
    global working_dir_stack
    working_dir_stack.append(os.getcwd())
    os.chdir(dir)
    print("Working dir:", os.getcwd())

def popd():
    global working_dir_stack
    os.chdir(working_dir_stack.pop())
    print("Working dir:", os.getcwd())

def shell(command : str, env = os.environ):
    print("Execute:", command)
    subprocess.run(command, shell=True, env=env)

def remove_dir(dir : str):
    if os.path.isdir(str):
        shutil.rmtree(dir)

def build_SDL():
    global script_root
    pushd("imported")
    build_dir = script_root + "/imported/build/SDL"
    install_dir = script_root + "/imported/installed/SDL"
    shell(f"cmake -S SDL -B \"{build_dir}\" -DCMAKE_INSTALL_PREFIX=\"{install_dir}\"")
    shell(f"cmake --build \"{build_dir}\" --target install --config Release")
    popd()

def build_SDL_mixer():
    global script_root
    pushd("imported")
    pushd("SDL_mixer/external")
    shell("git submodule update --init --recursive")
    popd() # imported/
    build_dir = script_root + "/imported/build/SDL_mixer"
    install_dir = script_root + "/imported/installed/SDL_mixer"
    sdl3_dir = script_root + "/imported/installed/SDL"
    shell(f"cmake -S SDL_mixer -B \"{build_dir}\" -DCMAKE_INSTALL_PREFIX=\"{install_dir}\"",
                   env=dict(os.environ, SDL3_DIR=sdl3_dir))
    shell(f"cmake --build \"{build_dir}\" --target install --config Release")
    popd() # root

def generate_project_files():
    if sys.platform == "win32":
        vcpkg_root = os.environ["VCPKG_ROOT"]
        vcpkg_toolchain_file = vcpkg_root + "/scripts/buildsystems/vcpkg.cmake"
        shell(f"cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=\"{vcpkg_toolchain_file}\"")

def main() -> int:
    try:
        pushd(script_root)
        build_SDL()
        build_SDL_mixer()
        generate_project_files()
        popd()
        return 0
    except Exception as e:
        print(e)
        return -1

if __name__ == '__main__':
    sys.exit(main())
