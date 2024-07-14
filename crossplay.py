import sys
import os
import json
from scripts.core import ExecutionPlatform, TargetPlatform, Metadata, CommandContext
from typing import Optional
import os.path as path
from scripts.commands import commands

DEBUG = True

def dbg(line: str):
    if DEBUG:
        print('[crossplay]: ' + line)
        
def run_command(command: str, args: list[str], eplatform: ExecutionPlatform, tplatform: Optional[TargetPlatform], meta: Metadata) -> bool:
    if command not in commands:
        print(f"Unknown command '{command}'")
        return False
    
    try:
        commands[command].execute(args, CommandContext(eplatform, tplatform, meta, run_command))
        return True
    
    except Exception as ex:
        print(f"Command '{command}' finished with error: {ex}")
        return False


def split_list(original_list: list, particular_value) -> list[list]:
    result = []
    temp_list = []
    for i in original_list:
        if i == particular_value:
            result.append(temp_list)
            temp_list = []
        else:
            temp_list.append(i)
    result.append(temp_list)
    return result

def main(args: list[str]):
    if args[0].startswith('-CD='):
        os.chdir(args[0][4::])
        args = args[1::]

    dont_save_meta = False
    if args[0] == '-dont-save-meta':
        dont_save_meta = True
        args = args[1::]


    with open('crossplay-config.json') as config:
        config = json.load(config)

    eplatform = ExecutionPlatform.current(config["ExecutionPlatform"])
    
    if path.exists('.crossplay-meta.json') == False:
        meta = Metadata({})
    else:
        with open('.crossplay-meta.json', 'r+') as meta:
            meta = json.load(meta)
            meta = Metadata(meta)

    def recreate_tplatform(meta: Metadata, current_tplatform: Optional[TargetPlatform]) -> Optional[TargetPlatform]:
        if Metadata.CurrentTargetPlatform not in meta.data:
            return None
        name = meta.data[Metadata.CurrentTargetPlatform]
        if current_tplatform is None or current_tplatform.name != name:
            return TargetPlatform(meta.data[Metadata.CurrentTargetPlatform], config["TargetPlatform"])
        else:
            return current_tplatform

    dbg("Crossplay initialization OK")
    dbg(f"ExecutionPlatform: {eplatform.name} => {eplatform.config}")
    dbg("Command processing started...")

    cmds = split_list(args, '=')

    for command_args in cmds:
        command = command_args[0]
        forward_args = command_args[1::]

        tplatform = recreate_tplatform(meta, None)

        dbg(f"Processing command '{command}' with args {forward_args} and meta: {meta.data}")

        if tplatform is not None: dbg(f"TargetPlatform: {tplatform.name} => {tplatform.config}")
        else: dbg(f"TargetPlatform: None")

        should_continue = run_command(command, forward_args, eplatform, tplatform, meta)

        if should_continue == False:
            break
    
    dbg(f"Metadata after execution: {meta.data}")
    if dont_save_meta:
        dbg("Metadata will not be saved")
    else:
        with open('.crossplay-meta.json', 'w') as o_meta:
            json.dump(meta.data, o_meta)

    dbg("Done")
        

if __name__ == "__main__":
    main(sys.argv[1::])
