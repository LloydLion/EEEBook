import os
from typing import Optional
import shutil
from os.path import exists

def _run_shell(cmd: str):
    if (exit_code := os.system(cmd)) != 0:
        raise Exception(f"'{cmd}' finished with non-zero exit code {exit_code}")
    
def _run_shell_cwd(cmd: str, cwd: str):
    old = os.getcwd()
    try:
        os.chdir(cwd)
        _run_shell(cmd)
    finally:
        os.chdir(old)

def run_shell(command: str, cwd: Optional[str] = None):
    if cwd is not None:
        _run_shell_cwd(command, cwd)
    else: _run_shell(command)

def delete_directory(path: str):
    if exists(path):
        shutil.rmtree(path)

def ensure_dir_created(path: str):
    if exists(path) == False:
        os.mkdir(path)

def read_all(path: str) -> str:
    if exists(path) == False:
        return None
    
    with open(path) as file:
        return file.read()
    
def write_all(path: str, content: str) -> None:
    with open(path, '+w') as file:
        file.write(content)

def copy_file(path: str, dest: str):
    shutil.copy(path, dest)
