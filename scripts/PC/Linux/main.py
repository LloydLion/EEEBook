from .. import pc_shared
from ...common import command, command_cwd
import shutil

def run(args: list[str], config):
    compiler_call = config['compiler']
    pc_shared.run(compiler_call, "Linux", ['-D Linux'])

    print("==COMPILATION FINISHED==")

    if '--build-only' not in args:
        command('./.pc/Linux/bin/program')

def setup_ide(args: list[str], config):
    shutil.copyfile('./scripts/PC/Linux/vs/tasks.json', './.vscode/tasks.json')
    shutil.copyfile('./scripts/PC/Linux/vs/c_cpp_properties.json', './.vscode/c_cpp_properties.json')
    shutil.copyfile('./scripts/PC/Linux/vs/launch.json', './.vscode/launch.json')
    pass

def debug(args: list[str], config):
    command_cwd("gdb " + ' '.join(map(lambda x: '"' + str(x) + '"', args)), './.pc/Linux/bin')
