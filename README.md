![Screenshot](screenshots/2018-02-17-143059_600x300_scrot.png)

Andvaranaut aims to be a retro dungeon crawler with procedural dungeon generation.

This project started as a self-study to fast compiling scalable
C software with fast link time optimization and maximum system portability.

Functions are naturally grouped by their typedefs. Externed functions made visible by header
files are protected by prefix 'x'. Externed functions may optionally be prefixed with the
first few letters of the type to prevent extern name clashes.

Types are passed by value to functions to maximize cache efficiency. Only one type may be
returned by a function to avoid circular header dependency. Pointers to types may not be
passed to functions as this violates the MISO (Multiple Input Single Output) piping structure
and creates circular header dependency.

Andvaranaut strives for maximum portability and can be cleanly compiled with
g++, gcc, clang, mingw32-gcc, mingw32-g++. For gcc c99 is required.
The graphics engine was written entirely in software with SDL2 to create
seamless portability across Mac, Linux, and Windows. To further aid portability,
SDL2 derivatives like SDL2_TTF are used.

Game mechanics are now being implemented.

    make -C src; ./andvaranaut
