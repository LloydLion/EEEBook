import os
import sys
from hashlib import md5
import shutil
from utils import run_shell, ensure_dir_created, read_all, write_all
from os.path import exists
import multiprocessing
from gcc_profiles import *


def compile_file(file: str, compiler_call: str, raw_platform_name: str, flags: list[str]) -> bool:
    try:
        output_name = file.replace('/', '__')

        prep_file = f'./.pc/{raw_platform_name}/obj/{output_name}.p'
        run_shell(f"{compiler_call} -E {file} -o {prep_file} {' '.join(flags)}")
        
        new_sum = md5(read_all(prep_file).encode()).hexdigest()
        precompiled_sum = read_all(prep_file + '.sum')

        if (new_sum != precompiled_sum):
            print(f"File {file} has been changed, recompiling...")
            run_shell(f"{compiler_call} -g -c {file} -o ./.pc/{raw_platform_name}/obj/{output_name}.o {' '.join(flags)}")
            write_all(prep_file + '.sum', new_sum)
        else:
            print(f"File {file} hasn't been changed, compilation skipped")

        return True
    except:
        return False
    
def list_files_to_compile(path: str, target_platform: str) -> list[str]:
    spec_file_path = f"{path}/platform-specific.ini"
    if exists(spec_file_path):
        platform_filter = read_all(spec_file_path)
        if target_platform.startswith(platform_filter) == False:
            return []

    files = [path + '/' + f for f in os.listdir(path) if f.endswith('.cpp') and os.path.isfile(os.path.join(path, f))]
    subdirs = [f for f in os.listdir(path) if not os.path.isfile(os.path.join(path, f))]
    for subdir in subdirs:
        files += list_files_to_compile(path + '/' + subdir, target_platform)
    return files


def compile_wrap(kwargs: dict) -> str:
    if compile_file(**kwargs):
        return None
    else: return kwargs['file']
    
def compile_all(path: str, target_platform: str, compiler_call: str, profile: PlatformProfile, flags: list[str]) -> list[str]:

    files = list_files_to_compile(path, target_platform)

    args = map(lambda x:
    {
        'file': x,
        'compiler_call': compiler_call,
        'raw_platform_name': profile.raw_name,
        'flags': flags
    }, files)

    pool = multiprocessing.Pool(os.cpu_count())
    errors = pool.map(compile_wrap, args)

    return list(filter(lambda x: x is not None, errors))
    

def main(args: list[str]):
    target_platform = args[0]
    compiler_call = args[1]
    print(f"GCC standalone started. Building for '{target_platform}', using '{compiler_call}'")

    profile = profiles[target_platform]

    ensure_dir_created(f'./.pc')
    ensure_dir_created(f'./.pc/{profile.raw_name}')
    ensure_dir_created(f'./.pc/{profile.raw_name}/bin')
    ensure_dir_created(f'./.pc/{profile.raw_name}/obj')

    flags = ['-Iinclude'] + list(map(lambda x: '-D' + x, profile.defines)) + profile.additional_compiler_flags

    errors = compile_all('src', target_platform, compiler_call, profile, flags)
    for error in errors:
        print("Failed to compile:", error)

    if len(errors) == 0:
        run_shell(f"{compiler_call} -g ./.pc/{profile.raw_name}/obj/**.o -o ./.pc/{profile.raw_name}/bin/program")

        for library_directory in profile.libraries:
            shutil.copytree(library_directory, f"./.pc/{profile.raw_name}/bin", dirs_exist_ok=True)

        print("Done")

    else:
        print("Linking skipped because compilation errors")

if __name__ == "__main__":
    main(sys.argv[1::])
else:
    raise Exception("Not supposed to be imported")