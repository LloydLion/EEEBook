import sys
import os
import json
import platform
from typing import Optional
from abc import ABC, abstractmethod
from typing import Optional, Callable
from dataclasses import dataclass
from scripts.utils import run_shell

class ExecutionPlatform:
    def __init__(self, name: str, config: dict) -> None:
        self.name = name
        self.config = config[name]

    @staticmethod
    def current(config):
        return ExecutionPlatform(platform.system(), config)
    
    def dependent_config(self, mapping: dict[str, str]):
        return self.config[mapping[self.name]]
    
class TargetPlatform:
    def __init__(self, name: str, config: dict) -> None:
        result = {}
        result.update(config.get(name, {}))

        sections = name.split(':')
        if len(sections) != 2:
            raise Exception("Invalid target platform name. Excepted format: 'FAMILY:NAME'")
        
        [a, b] = sections
        indict = config.get(a, {})

        for el in indict.keys():
            if not el.startswith(':'):
                result[el] = indict[el]

        indict = indict.get(':' + b, {})
        result.update(indict)

        self.family = a
        self.name = name
        self.config = result

class Metadata:
    CurrentTargetPlatform = 'CurrentTargetPlatform'

    def __init__(self, meta: dict) -> None:
        self.data = meta

@dataclass
class CommandContext:
    eplatform: ExecutionPlatform
    tplatform: Optional[TargetPlatform]
    meta: Metadata
    run_command: Callable

def intended_only_for(tplatform: TargetPlatform, supported_platform_family: str):
    if tplatform.family != supported_platform_family:
        raise Exception(f"Commands of this type are intended only for the target '{supported_platform_family}' family platform")


class Command(ABC):
    @abstractmethod
    def execute(self, args: list[str], ctx: CommandContext):
        raise NotImplementedError()
    
class TargetPlatformIndependentCommand(Command, ABC):
    pass
    
class ForwardCommand(TargetPlatformIndependentCommand):
    def __init__(self, method) -> None:
        self.method = method

    def execute(self, args: list[str], ctx: CommandContext):
        self.method(args, ctx)

class LinuxUniversalCommand(TargetPlatformIndependentCommand, ABC):
    def run_python_standalone(self, eplatform: ExecutionPlatform, standalone_path: str, args: str, cwd: Optional[str] = None):
        python = eplatform.dependent_config({"Linux": "python", "Windows": "wsl-python"})
        self.run_linux_command(eplatform, f"{python} \"{standalone_path}\" {args}", cwd)

    def run_linux_command(self, eplatform: ExecutionPlatform, command: str, cwd: Optional[str] = None):
        if eplatform.name == 'Linux':
            run_shell(command, cwd)
        else:
            wsl = eplatform.config['wsl']
            run_shell(f"{wsl} {command}", cwd)

class PlatformIOCommand(TargetPlatformIndependentCommand):
    def __init__(self, command) -> None:
        self.command = command

    def execute(self, args: list[str], ctx: CommandContext):
        intended_only_for(ctx.tplatform, 'MCU')

        platformio = ctx.tplatform.config['platformio']
        environment = ctx.tplatform.config['environment']
        run_shell(f"{platformio} {self.command} --environment {environment}")

class ComposeCommand(Command):
    def __init__(self, cmds: list[Command], post_action: Optional[Callable] = None) -> None:
        self.cmds = cmds
        self.post_action = post_action

    def execute(self, args: list[str], ctx: CommandContext):
        for command in self.cmds:
            command.execute(args, ctx)
        if self.post_action is not None:
            self.post_action(args, ctx)

class EmptyCommand(TargetPlatformIndependentCommand):
    def execute(self, args: list[str], ctx: CommandContext):
        pass

class TargetPlatformDependentCommand(Command):
    def __init__(self, mapping: dict[str, Command]) -> None:
        self.mapping = mapping
    
    def execute(self, args: list[str], ctx: CommandContext):
        def run(key) -> None:
            if key in self.mapping:
                self.mapping[key].execute(args, ctx)
                return True
            return False

        if run(ctx.tplatform.name): return
        if run(ctx.tplatform.family): return

        raise Exception(f"Not supported target platform '{ctx.tplatform.name}'" +
            f"List of supported platforms and platforms families: {self.mapping.keys()}")
