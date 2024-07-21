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
        generate_project_files()
        popd()
        return 0
    except Exception as e:
        print(e)
        return -1

if __name__ == '__main__':
    sys.exit(main())
