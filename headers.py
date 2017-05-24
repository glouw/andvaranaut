# Generates a header file from a source file.
#   Functions to be exported must be marked with extern.
#   Functions to be kept private must be marked with
#   static for the sake of the linker.

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
        # Util:
        #   for the Handwritten macros
        # Main:
        #   main will never have a header file
        # Types:
        #   for safety
        exceptions = ["Util.h", "main.h", "Types.h"]
        if head in exceptions:
            continue
        h = open(head, "w")
        c = open(path, "r")
        preps(h)
        parse(h, c)
        h.close()
        c.close()
