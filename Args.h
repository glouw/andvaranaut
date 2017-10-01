#pragma once

typedef struct
{
    int xres;
    int yres;
    float scale;
    int fps;
}
Args;

Args parse(const int argc, const char* argv[]);
