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

Args xparse(int argc, char* argv[]);
