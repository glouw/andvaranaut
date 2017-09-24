#include "Args.h"

#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Args parse(const int argc, const char* argv[])
{
    Args args;
    zero(args);
    // Screen resolution
    args.res =
        argc == 2 ?
        strtol(argv[1], NULL, 0)
        : 512;
    // Field of view scale
    args.scale =
        argc == 3 ?
        strtof(argv[2], NULL)
        // 100 degrees
        : 1.19178;
    // Frames per second
    args.fps =
        argc == 4 ?
        strtol(argv[3], NULL, 0)
        : 60;
    srand(time(0));
    return args;
}
