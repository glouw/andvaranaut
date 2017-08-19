import os, sys

def preps(h):
    h.write("#pragma once\n\n")
    h.write('#include "Types.h"\n\n')

def parse(h, c):
    for line in c.readlines():
        if "extern" in line.split():
            h.write(line.replace("\n", ";\n\n"))

def make_header(source):
    """Makes the header for one .c source file"""
    header = source.replace(".c", ".h")
    # The following headers files must not be generated
    exceptions = ["Util.h", "main.h", "Types.h"]
    if header in exceptions: return
    # If the file cannot be opened then no header file will be made
    with open(header, "w") as h, open(source, "r") as c:
        if not h or not c:
            return
        preps(h)
        parse(h, c)

def main():
    # Will generate the header file for a single .c file
    if len(sys.argv) == 2: make_header(sys.argv[1])
    # Will generate the header file for all the .c files
    else:
        for path in os.listdir("."):
            if ".c" in path: make_header(path)

main()
