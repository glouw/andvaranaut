![Screenshot](screenshots/2018-02-17-143059_600x300_scrot.png)

Andvaranaut aims to be a retro dungeon crawler with procedural dungeon generation.

    make -C src; ./andvaranaut

# Dependencies

    SDL2-devel

Andvaranaut builds fast on Windows, Linux, and MacOS with its unique software graphics engine.

# Source wise

    src:
        Game engine

    art:
        Game art

    aux:
        Support Python2 scripts (procedural art generation and such). Game will release without aux folder

    screenshots:
        Self explanatory

# Source Tips

    Headers provide a typedef - associated source file provides functionality for the typedef.

    The first type of a function signifies the function's header and source file typedef.

    Functions starting with an 'x' are exported in the header.

    Global variables are avoided at all costs.
