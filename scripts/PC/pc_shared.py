import os
from hashlib import md5
from ..common import command

def _ensure_dir(path: str):
    if os.path.exists(path) == False:
        os.mkdir(path)

def _read_all(path: str) -> str:
    if os.path.exists(path) == False:
        return None
    
    with open(path) as file:
        return file.read()
    
def _write_all(path: str, content: str) -> None:
    with open(path, '+w') as file:
        file.write(content)


def _compile_all(path: str, compiler_call, platform_name, flags) -> list[str]:
    compile_errors = []

    # List all .cpp files in current directory
    files = [f for f in os.listdir(path) if f.endswith('.cpp') and os.path.isfile(os.path.join(path, f))]

    for file in files:
        try:
            output_name = path.replace('/', '__') + '__' + file

            prep_file = f'./.pc/{platform_name}/obj/{output_name}.p'
            command(f"{compiler_call} -E {path}/{file} -o {prep_file} {' '.join(flags)}")
            
            new_sum = md5(_read_all(prep_file).encode()).hexdigest()
            precompiled_sum = _read_all(prep_file + '.sum')

            if (new_sum != precompiled_sum):
                print(f"File {path}/{file} has been changed, recompiling...")
                command(f"{compiler_call} -g -c {path}/{file} -o ./.pc/{platform_name}/obj/{output_name}.o {' '.join(flags)}")
                _write_all(prep_file + '.sum', new_sum)
            else:
                print(f"File {path}/{file} hasn't been changed, compilation skipped")
        except:
            compile_errors.append(f"{path}/{file}")

    subdirs = [f for f in os.listdir(path) if not os.path.isfile(os.path.join(path, f))]
    for subdir in subdirs:
        compile_errors += _compile_all(path + '/' + subdir, compiler_call, platform_name, flags)

    return compile_errors

def setup_ide():
    pass

def run(compiler_call: str, platform_name: str, additional_compiler_flags: list[str]):
    _ensure_dir(f'./.pc/{platform_name}')
    _ensure_dir(f'./.pc/{platform_name}/bin')
    _ensure_dir(f'./.pc/{platform_name}/obj')

    errors =_compile_all('src', compiler_call, platform_name, ['-Iinclude'] + additional_compiler_flags)
    for error in errors:
        print("Failed to compile:", error)

    if len(errors) == 0:
        command(f"{compiler_call} -g ./.pc/{platform_name}/obj/**.o -o ./.pc/{platform_name}/bin/program")
    else:
        print("Linking skipped because compilation errors")
    