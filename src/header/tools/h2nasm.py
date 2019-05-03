#!/usr/local/bin/python3
# -*- coding: utf-8 -*-
# author: Shiroko


import json
import sys
import os
import CppHeaderParser

cwd = os.getcwd()

header_fn: str = sys.argv[-1]
try:
    header = CppHeaderParser.CppHeader(cwd + '/' + header_fn)
except Exception as e:
    print(e, file=sys.stderr)
    sys.exit(1)


def r_add_header_file(cur: CppHeaderParser) -> list:
    p = []
    if len(cur.includes) != 0:
        for inc in cur.includes:
            ch = (CppHeaderParser.CppHeader(
                cwd + '/' + inc.replace('"', '')))
            if ch.headerFileName not in list(map(lambda x: x.headerFileName, p)):
                p.append(ch)
            p.extend(r_add_header_file(p[-1]))
    return p


parents = r_add_header_file(header)


headers = parents
headers.append(header)

defines = list(map(lambda x: x.defines, headers))
typedefs_ori = list(map(lambda x: x.typedefs, headers))
classes = list(map(lambda x: x.classes, headers))

structs = []

typedefs = {}

for typedefs_single_file in typedefs_ori:
    typedefs.update(typedefs_single_file)

defines = [i for sublist in defines for i in sublist]


def r_remap_typedef(td: dict):
    keys = list(td.keys())
    has_changed = False
    for key in keys:
        if td[key] in keys:
            td[key] = td[td[key]]
            has_changed = True
    if has_changed:
        r_remap_typedef(td)


r_remap_typedef(typedefs)

for i in range(0, len(classes)):
    for key in classes[i].keys():
        props = []
        for p in classes[i][key]['properties']['public']:
            props.append({"name": p['name'],
                          "type": p['type'] if p['type'] not in typedefs.keys() else typedefs[p['type']]})
        structs.append({
            "name": classes[i][key]['name'],
            "props": props
        })


#print(json.dumps(structs, indent=2))

#print(json.dumps(typedefs, indent=2))

# print(defines)

typelen = {  # in bit
    "unsigned int": 32,
    "unsigned short": 16,
    "unsigned char": 8,
    "void *": 32,
    "char": 8
}


def generate_struc_nasm(d: dict) -> str:
    s = ""
    for prop in d['props']:
        if prop['type'] in typelen.keys():
            s += ".%s resb %d\n" % (prop['name'], typelen[prop['type']] / 8)
        else:
            if '*' not in prop['type']:
                s += ".%s resb %s_size\n" % (prop['name'], prop['type'].replace(
                    'struct', '').replace(' ', '').strip())
            else:
                s += ".%s resd 1\n" % (prop['name'])

    res = """struc %s
%sendstruc """ % (d['name'], s)
    return res


print("; Auto Generated Code\n\n")

for i in range(0, len(structs)):
    print(";================================")
    print(generate_struc_nasm(structs[i]))
    print(";==============================\n")
