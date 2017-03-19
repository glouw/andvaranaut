#pragma once

#include "Gpu.h"

typedef struct
{
    uint32_t* pixels;
    int width;
}
Display;

Display lock(const Gpu gpu);
void unlock(const Gpu gpu);
