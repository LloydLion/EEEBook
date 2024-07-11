import os
import shutil
from ...common import command

def setup_ide(args: list[str], config):
    platformio = config['platformio']
    environment = config['environment']

    command(f"{platformio} project init --ide vscode --environment {environment}")

    shutil.copyfile('./scripts/MCU/ESP32-C3/tasks.json', './.vscode/tasks.json')
    os.remove('./.vscode/extensions.json')
    os.remove('./.vscode/launch.json')

def run(args: list[str], config):
    platformio = config['platformio']
    environment = config['environment']

    if '--build-only' in args:
        run_command = platformio + ' run'
    else: run_command = platformio + ' run --target upload'

    run_command += f' --environment {environment}'

    command(run_command)
