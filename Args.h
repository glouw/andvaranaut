#pragma once

typedef struct
{
    int xres;
    int yres;
    float focal;
    int fps;
}
Args;

Args xparse(const int argc, const char* argv[]);
