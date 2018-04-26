![Screenshot](scn/2018-02-17-143059_600x300_scrot.png)

Andvaranaut aims to be a retro dungeon crawler with procedural dungeon generation.

# Building

See the Makefile for instructions on how to bulid for Linux, MacOS, and Windows.

# Folders

src: Game engine

art: Game art

aux: Supporting Python2 scripts (procedural art generation and such). Game will ship without aux folder

scn: Screenshots

# Source Code Tips

* Headers provide a typede. Associated source file provides functionality for the typedef.

* The first type of a function signifies the function's header and source file typedef.

* Functions starting with an 'x' are exported in the header.

* Functions take types on the stack as by-value copies. Functions take types on the heap by reference.
This was done to prevent accidental freeing of stack based memory.

* Types that use data (like struct typedefs) have an "xz" prefixed function for setting the type to zero.
This is done by returning a static initialized version of the struct. These functions can be found at the
top of .c files and exported in .h headers for all types to use.

* Global variables are prohibited.

* Andvaranaut is portable. Large libraries with heavy dependencies are avoided.

* The Makefile uses link time optimization and the fastest non-conforming IEEE floating point optimizations.

* Andvaranaut compiles cleanly with a variety of C compilers: gcc, clang, and MINGW32.

* Steps were taken to have Andvarnaut compile with C++ compilers. This allows Andvaranaut to one day support
C++ source modules if Andvaranaut ever grows beyond a small two-man team.

* Development will continue in C for the fast compile times while the Andvaranaut source is small (less than 25,000 lines).
