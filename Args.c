#include "Args.h"

#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

void help()
{
    xlicense();
    xbomb(
    "-x: x-resolution [800]\n"
    "-y: y-resolution [400]\n"
    "-f: focal length [1.0]\n"
    "-v: VSync        [t,f]\n"
    "-p: FPS          [60]\n");
}

static void print(const Args args)
{
    printf(
    "x-resolution: %d\n"
    "y-resolution: %d\n"
    "focal length: %f\n"
    "VSync       : %s\n"
    "FPS         : %d\n"
    "Using %d thread(s) for the renderer.\n",
    args.xres, args.yres, args.focal, args.vsync ? "t" : "f", args.fps,
    SDL_GetCPUCount());
}

// Parses command line arguments.
static Args parsed(Args args, const int argc, const char* argv[])
{
    for(int i = 0; i < argc; i++)
        if(argv[i][0] == '-')
        {
            if(argv[i][1] == 'h') help();
            // Out of bounds check.
            if(i + 1 == argc)
                break;
            // Parse argument list.
            const char* const next = argv[i + 1];
            switch(argv[i][1])
            {
            case 'x':
                args.xres =
                    strtod(next, NULL);
                break;
            case 'y':
                args.yres =
                    strtod(next, NULL);
                break;
            case 'f':
                args.focal =
                    strtof(next, NULL); /* Notice the float. */
                break;
            case 'v':
                args.vsync =
                    // True and false handling via strings.
                    equals(next, "true")  ? 1 : equals(next, "t") ? 1 :
                    equals(next, "false") ? 0 : equals(next, "f") ? 0 :
                    // True and false handling via integers.
                    strtod(next, NULL) != 0;
                break;
            case 'p':
                args.fps =
                    strtod(next, NULL);
                break;
            default:
                break;
            }
        }
    print(args);
    return args;
}

static Args defaults()
{
    Args args;
    xzero(args);
    args.xres = 800;
    args.yres = 450;
    args.focal = 1.0; /* 90 Degrees. */
    args.vsync = true;
    args.fps = 60;
    return args;
}

Args xparse(const int argc, const char* argv[])
{
    return parsed(defaults(), argc, argv);
}
