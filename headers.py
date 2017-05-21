import os

def preps(h):
    h.write("#pragma once\n\n")
    h.write('#include "Types.h"\n\n')

def parse(h, c):
    for line in c.readlines():
        if "extern" in line.split():
            h.write(line.replace("\n", ";\n\n"))

for path in os.listdir("."):
    if ".c" in path:
        head = path.replace(".c", ".h")
        # Exceptions
        if head == "Util.h" or head == "main.h":
            continue
        h = open(head, "w")
        c = open(path, "r")
        preps(h)
        parse(h, c)
        h.close()
        c.close()
