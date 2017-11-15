#include "Args.h"

#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Args xparse(const int argc, const char* argv[])
{
    // TODO: Build a proper string token parser.
    Args args;
    xzero(args);
    // Screen resolution.
    args.xres =
        argc >= 2 ?
        strtol(argv[1], NULL, 0)
        : 800;
    args.yres =
        argc >= 3 ?
        strtol(argv[2], NULL, 0)
        : args.xres * 9.0 / 16.0;
    // Focal Length for Field of View.
    args.focal =
        argc >= 4 ?
        strtof(argv[3], NULL)
        : 1.19178; /* ~100 degrees */
    // Renderer Vertical Sync.
    args.vsync =
        argc >= 5 ?
        strtol(argv[4], NULL, 0)
        : true;
    // Frames per second.
    args.fps =
        argc >= 6 ?
        strtol(argv[5], NULL, 0)
        : 60;
    return args;
}
