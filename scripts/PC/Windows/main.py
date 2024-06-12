from .. import pc_shared
from ...common import command, command_cwd
from distutils import dir_util
import shutil

def run(args: list[str], config):
    compiler_call = f"{config['wsl']} {config['compiler']}"
    pc_shared.run(compiler_call, "Windows", ['-D Windows'])

    dir_util.copy_tree("./scripts/PC/Windows/dll", "./.pc/Windows/bin")

    print("==COMPILATION FINISHED==")

    if '--build-only' not in args:
        command('.\\.pc\\Windows\\bin\\program.exe')

def setup_ide(args: list[str], config):
    shutil.copyfile('./scripts/PC/Windows/vs/tasks.json', './.vscode/tasks.json')
    shutil.copyfile('./scripts/PC/Windows/vs/c_cpp_properties.json', './.vscode/c_cpp_properties.json')
    pass
