import os

def command(cmd: str):
    if (exit_code := os.system(cmd)) != 0:
        raise Exception(f"'{cmd}' finished with non-zero exit code {exit_code}")
    

def command_cwd(cmd: str, cwd: str):
    old = os.getcwd()
    try:
        os.chdir(cwd)
        command(cmd)
    finally:
        os.chdir(old)
