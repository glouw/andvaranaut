import os

oprs = [
    "{", "}", ".", "->", "++", "--", "&",
    "+", "-", "~", "!", "sizeof", "/", "%",
    "<<", ">>", "<", ">", "<=", ">=", "==",
    "!=", "^", "|", "&&", "||", "?", ":", "=",
    "*=", "/=", "%=", "+=", "-=", "<<=", ">>=",
    "&=", "^=", "|=", "#", "##", ";", ":"
]

keys = [
    "break", "case", "continue", "default", "do",
    "return", "sizeof", "static", "switch",
    "typedef", "while", "else", "for", "goto", "if"
]

ends = [ "", "\n", "\r\n" ]

def empty(line):
    return line in ends

def parse(line):
    if not any(key in line for key in keys):
        if not any(opr in line for opr in oprs):
            if not empty(line):
                print(line)

def show(c):
    lines = c.readlines()
    for line in lines:
        parse(line)

def prepare(h):
    print("#pragma once\n")
    print("#include <Types.h>\n")

for path in os.listdir("."):
    if ".c" in path:
        head = path.replace(".c", ".h")
        try:
            with open(head) as h:
                prepare(h)
        except:
            pass
        with open(path) as c:
            show(c)
