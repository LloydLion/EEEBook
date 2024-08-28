from utils import *
import re

def list_files(path: str) -> list[str]:
    files = [path + '/' + f for f in os.listdir(path) if f.endswith('.h') and os.path.isfile(os.path.join(path, f))]
    subdirs = [f for f in os.listdir(path) if not os.path.isfile(os.path.join(path, f))]
    for subdir in subdirs:
        files += list_files(path + '/' + subdir)
    return files

files = list_files('include')

for file in files:
    print("\033[H\033[J", end="")
    print(f"processing {file}")
    try:
        content = read_all(file)

        define = file[len('include/')::]
        define = re.sub(r"(\w)([A-Z][a-z])", r"\1 \2", define)
        define = define.upper().replace('/', '_').replace('.', '_').replace(' ', '_')

        by_lines = content.splitlines(True)
        by_lines[0] = f"#ifndef {define}\n"
        by_lines[1] = f"#define {define}\n"

        new_content = "".join(by_lines)

        if new_content == content:
            continue

        print(f'DEFINE={define}\n')

        if len(content) < 200:
            print(content)
        else:
            print(content[:200:])
        ans = input("Should fix? [Y/N]: ")
        if ans != 'Y': continue

        write_all(file, new_content)
    except Exception as ex:
        print("failed :(")
        print(ex)

    
    