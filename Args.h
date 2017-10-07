#pragma once

typedef struct
{
    int xres;
    int yres;
    float scale;
    int fps;
}
Args;

Args xparse(const int argc, const char* argv[]);
