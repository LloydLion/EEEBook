from scripts.core import *
from typing import Optional
from scripts.utils import delete_directory, ensure_dir_created, copy_file, read_all, write_all
import scripts.gcc_profiles as gcc_profiles

def __change_platform(args: list[str], ctx: CommandContext):
    if '--print' in args:
        print(ctx.meta.data[Metadata.CurrentTargetPlatform])
    elif '--execution-platform' in args:
        ctx.meta.data[Metadata.CurrentTargetPlatform] = 'PC:' + ctx.eplatform.name
    else:
        ctx.meta.data[Metadata.CurrentTargetPlatform] = args[0]

ChangePlatformCommand = ForwardCommand(__change_platform)


def __setup_vs_tasks_files(args: list[str], ctx: CommandContext):
    ensure_dir_created('.vscode')
    copy_file('./scripts/tasks.json', './.vscode/')
    
def __setup_vs_debug_files(args: list[str], ctx: CommandContext):
    ensure_dir_created('.vscode')
    copy_file('./scripts/launch.json', './.vscode/')

def __configure_c_cpp_vs(args: list[str], ctx: CommandContext):
    content = read_all('scripts/c_cpp_properties.json')

    defines_string = "[" + ", ".join(map(lambda x: "\"" + x + "\"", gcc_profiles.profiles[ctx.tplatform.name].defines)) + "]"
    content = content.replace('$(Defines)', defines_string)

    ensure_dir_created('.vscode')
    write_all('.vscode/c_cpp_properties.json', content)


def __platform_io_init_post_action(args: list[str], ctx: CommandContext):
    os.remove('.vscode/extensions.json')
    os.remove('.vscode/launch.json')

def __clear_vs_meta(args: list[str], ctx: CommandContext):
    delete_directory('.vscode')

ClearVSMetaCommand = ForwardCommand(__clear_vs_meta)
SetupVSTasksCommand = ForwardCommand(__setup_vs_tasks_files)
SetupVSDebugCommand = ForwardCommand(__setup_vs_debug_files)
ConfigureCCppPropertiesCommand = ForwardCommand(__configure_c_cpp_vs)
PlatformIOInitCommand = ComposeCommand([PlatformIOCommand("init --ide vscode")], __platform_io_init_post_action)

SetupIDECommand = ComposeCommand([
    ClearVSMetaCommand,

    TargetPlatformDependentCommand({
        "MCU": PlatformIOInitCommand,
        "PC": EmptyCommand(),
        "PC:Linux": SetupVSDebugCommand,
    }), 
    
    SetupVSTasksCommand,

    TargetPlatformDependentCommand({
        "MCU": EmptyCommand(),
        "PC": ConfigureCCppPropertiesCommand 
    })
])

class GCCCompileCommand(LinuxUniversalCommand):
    def execute(self, args: list[str], ctx: CommandContext):
        self.run_python_standalone(ctx.eplatform, 'scripts/gcc_standalone.py', f"{ctx.tplatform.name} {ctx.tplatform.config['compiler']}")

PlatformIOBuildCommand = PlatformIOCommand("run")
GCCBuildCommand = GCCCompileCommand()
BuildCommand = TargetPlatformDependentCommand({"MCU": PlatformIOBuildCommand, "PC": GCCBuildCommand})


def __run_executable(args: list[str], ctx: CommandContext):
    intended_only_for(ctx.tplatform, 'PC')
    mapping = { 'PC:Windows': ".pc\\Windows\\bin\\program.exe", 'PC:Linux': ".pc/Linux/bin/program" }

    if '--ignore-target-platform' not in args and 'PC:' + ctx.eplatform.name != ctx.tplatform.name:
        raise Exception("Your PC platform is not equals to current target platform. Switch target platform or use '--ignore-target-platform' flag")

    run_shell(mapping[ctx.tplatform.name])

PlatformIOUploadCommand = PlatformIOCommand("run --target nobuild --target upload")
RunExecutableCommand = ForwardCommand(__run_executable)
RunCommand = TargetPlatformDependentCommand({'MCU': PlatformIOUploadCommand, 'PC': RunExecutableCommand})

def __clear_binaries(args: list[str], ctx: CommandContext):
    delete_directory('.pio')
    delete_directory('.pc')

    
def __clear_crossplay(args: list[str], ctx: CommandContext):
    os.remove('.crossplay-meta.json')

ClearCommand = ForwardCommand(__clear_binaries)
ClearAllCommand = ComposeCommand([ClearCommand, ClearVSMetaCommand, ForwardCommand(__clear_crossplay)])

def __proxy_debug(args: list[str], ctx: CommandContext):
    if ctx.eplatform.name != "Linux" or ctx.tplatform.name != "PC:Linux":
        raise Exception("Debugging only available for Linux (as execution and target platform)")
    
    if '--build' in args:
        args.remove('--build')
        ctx.run_command('build', [], ctx.eplatform, ctx.tplatform, ctx.meta)

    debugger = ctx.tplatform.config['debugger']
    args_string = "".join(map(lambda x: " \"" + x + "\"", args))
    run_shell(f"{debugger}{args_string}")


ProxyDebugCommand = ForwardCommand(__proxy_debug)


commands: dict[str, Command] = {
    'platform': ChangePlatformCommand,

    'clear': ClearCommand,
    'clear-all': ClearAllCommand,

    'platformio/init': PlatformIOInitCommand,
    'vscode/clear': ClearVSMetaCommand,
    'vscode/tasks': SetupVSTasksCommand,
    'vscode/setup-debug': SetupVSDebugCommand,
    'vscode/config-c-cpp': ConfigureCCppPropertiesCommand,
    'ide': SetupIDECommand,

    'platformio/build': PlatformIOBuildCommand,
    'gcc/build': GCCBuildCommand,
    'build': BuildCommand,

    'platformio/upload': PlatformIOUploadCommand,
    'run_executable': RunExecutableCommand,
    'run': RunCommand,

    'proxy-debug': ProxyDebugCommand
}
