#pragma once

typedef struct
{
    int xres;
    int yres;
    float focal;
    int vsync;
    int fps;
}
Args;

// Bundles together all command line parameters.
Args xparse(const int argc, const char* argv[]);
