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

Args a_parse(int argc, char* argv[]);
