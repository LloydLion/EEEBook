import sys
import os

import scripts.MCU.ESP32.main as MCU_ESP32
import scripts.PC.Linux.main as PC_Linux
import scripts.PC.Windows.main as PC_Windows

from typing import Callable
import json


def platform_dependent(code: dict[str, Callable], platform: str, forward_args: list[str]):
    if platform not in code:
        raise Exception(f"Not supported platform: {platform}. Support list: {code.keys()}")
    code[platform](forward_args, config[platform])

def clear():
    if os.path.exists('./.vscode/c_cpp_properties.json'):
        os.remove('./.vscode/c_cpp_properties.json')
    if os.path.exists('./.vscode/tasks.json'):
        os.remove('./.vscode/tasks.json')
    if os.path.exists('./.vscode/launch.json'):
        os.remove('./.vscode/launch.json')

def setup_ide(args: list[str]):
    platform_dependent({'MCU:ESP32': MCU_ESP32.setup_ide, 'PC:Linux': PC_Linux.setup_ide, "PC:Windows": PC_Windows.setup_ide}, args[0], args[1::])

def run(args: list[str]):
    platform_dependent({'MCU:ESP32': MCU_ESP32.run, 'PC:Linux': PC_Linux.run, "PC:Windows": PC_Windows.run}, args[0], args[1::])

def debug(args: list[str]):
    platform_dependent({'PC:Linux': PC_Linux.debug}, args[0], args[1::])

def main(args: list[str]):
    global config
    with open('crossplay-config.json') as cnf:
        config = json.load(cnf)
    command = args[0]
    forward_args = args[1::]

    if command == 'setup_ide':
        setup_ide(forward_args)
    elif command == 'clear_ide':
        clear()
    elif command == 'switch_ide':
        clear()
        setup_ide(forward_args)
    elif command == 'run':
        run(forward_args)
    elif command == 'debug':
        debug(forward_args)
    else: raise Exception("Invalid command line arguments, these is no help")

if __name__ == "__main__":
    main(sys.argv[1::])
