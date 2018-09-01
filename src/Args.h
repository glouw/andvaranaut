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

Args xzargs(void);

Args xparse(int argc, char* argv[]);
