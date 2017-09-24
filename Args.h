#pragma once

typedef struct
{
    int res;
    float scale;
    int fps;
}
Args;

Args parse(const int argc, const char* argv[]);
