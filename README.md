![Screenshot](scn/2018-02-17-143059_600x300_scrot.png)

Andvaranaut aims to be a retro dungeon crawler with procedural dungeon generation.

# Building

Andvaranaut is portable. See the Makefile for instructions on how to bulid for Linux, MacOS, and Windows.

# Dependencies

* SDL2-devel

# Source wise

src: Game engine

art: Game art

aux: Support Python2 scripts (procedural art generation and such). Game will ship without aux folder

scn: Screenshots

# Source Tips

* Headers provide a typedef - associated source file provides functionality for the typedef.

* The first type of a function signifies the function's header and source file typedef.

* Functions starting with an 'x' are exported in the header.

* Types that use data (like struct typedefs) have an "xz" prefixed function for setting the type to zero.
This is done by returning a static initialized version of the struct. These functions can be found at the top of .c files.
They are exported in headers for all types to use.

* Global variables are prohibited.
