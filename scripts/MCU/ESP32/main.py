import os
import shutil

def setup_ide(args: list[str], config):
    platformio = config['platformio']
    if (exit_code := os.system(f"{platformio} project init --ide vscode")) != 0:
        raise Exception(f"'{platformio} project init' failed to run. Exit code: {exit_code}")
    shutil.copyfile('./scripts/MCU/ESP32/tasks.json', './.vscode/tasks.json')
    os.remove('./.vscode/extensions.json')
    os.remove('./.vscode/launch.json')
    pass

def run(args: list[str], config):
    platformio = config['platformio']
    if '--build-only' in args:
        command = platformio + ' run'
    else: command = platformio + ' run --target upload'

    if (exit_code := os.system(command)) != 0:
        raise Exception(f"'platformio run' failed to run. Exit code: {exit_code}")