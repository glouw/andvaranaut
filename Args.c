#include "Args.h"

#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void print(const Args args)
{
    printf("%d\n%d\n%f\n%d\n%d\n",
        args.xres, args.yres, args.focal, args.vsync, args.fps);
}

// Parses command line arguments.
static Args parsed(Args args, const int argc, const char* argv[])
{
    for(int i = 0; i < argc; i++)
        if(argv[i][0] == '-')
        {
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
    args.yres = 400;
    args.focal = 1.19178; /* About 100 degrees. */
    args.vsync = true;
    args.fps = 60;
    return args;
}

Args xparse(const int argc, const char* argv[])
{
    return parsed(defaults(), argc, argv);
}
