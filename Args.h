#pragma once

typedef struct
{
    int xres;
    int yres;
    float focal;
    int vsync;
    int fps;
    float msen;
    int threads;
}
Args;

// Bundles together all command line parameters.
Args xparse(int argc, char* argv[]);
