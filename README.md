# Andvaranaut
![](art/screenshots/2018-02-17-143059_600x300_scrot.png)

A dungeon crawler with procedural dungeon generation, many items, many monsters, many puzzles, and so much more!

## Building
See the Makefile for instructions on how to bulid for Linux, MacOS, and Windows.

## System Requirements
RAM: 256 MB (generous).
CPU: 4 cores or more (for the software renderer).
GPU: Just about anything will do.

## Credits
Item art by Platino.

![](art/screenshots/platino.png)

Color pallette by DawnBringer.

![](art/screenshots/bigfoot_v3.png)

## Special Thanks
Halic (Mac) - Prototype tile and sprite art work.

## Source Tips
* The type name of a function's first argument specifies the function's source file.
* No global state is kept (unless static local variables are counted).
* Source compiles and links cleanly with any ISO C99 / C++98 compliant compiler in less than 10 seconds.
* Cross system portability is aided by only using SDL2 libraries and a custom software renderer.
* The majority of CPU time is spent rendering the floor, ceiling, and walls.
* The GPU is only used for pasting sprites on screen.
* Maps (walls, floors, and ceilings) are stored in 2D character arrays where each character is a tile represented by a non-alpha character.
* Sprites are stored in a linear arrays and are represented by alpha characters.
* The software renderer used for rendering the walls, floors, and ceilings was inspired by the Doom and Wolfenstein3D renderers.

## Notice
Andvaranaut will always be free. Enjoy, and please feel free to contribute, be it this repo or your own fork!
Donations of any sort will be pledged to the free software foundation.
