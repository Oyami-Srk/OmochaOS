#!/usr/bin/python3
# -%- coding: utf-8 -%-
# -%- author: shiroko -%-

"""
To generate modules.h from .c files in src/modules
"""

import time
import re
import yaml

template:str = """// {info}
extern void {fn}();"""

def parse_c_file(fn: str) -> bool:
    yaml_str = ""
    with open(fn, "r") as f:
        s = f.read()
        regx = re.compile(r'/\*([\w\W]*)\*/');
        for i in regx.findall(s):
            yaml_str += i
    return yaml.load(yaml_str)

def generate_from_yaml(obj: dict) -> str:
    if("module" not in obj.keys()):
        return ""
    info:dict = obj["module"]
    return template.format(info=info["summary"], fn=info["module_task"])

def iter_c_files_in_dir(dir:str):
    import os
    files = os.listdir(dir)
    c = ""
    for f in files:
        if not os.path.isdir(f):
            if os.path.splitext(f)[-1] == ".c":
                try:
                    c += generate_from_yaml(parse_c_file(f))
                    c += "\n"
                except:
                    pass
    return c

def insert_to_header_file(fn:str, content:str):
    l = []
    meet_1 = False
    meet_2 = False
    wrote = False

    with open(fn, "r") as f:
        l = f.readlines()
    with open(fn, "w") as f:
        for line in l:
            if meet_1 == meet_2 or "{Insert Above}" in line:
                f.write(line)
            elif meet_1 == True and wrote == False:
                f.write(content)
                wrote = True
            if "{Insert Below}" in line:
                meet_1 = True
            elif "{Insert Above}" in line:
                meet_2 = True




if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description="Automate generate function proto from .c files's head")
    parser.add_argument('-m', '--modules', dest="MODS", help="dir contains .c files")
    parser.add_argument('-a', '--header', dest="HEADER", help="Header to insert")
    args = parser.parse_args()
    insert_to_header_file(args.HEADER, iter_c_files_in_dir(args.MODS))
