#pragma once

#include "Hit.h"
#include "Wall.h"
#include "Gpu.h"
#include "Display.h"
#include "Traceline.h"

typedef struct
{
    Gpu gpu;
    Display display;
    int y;
    int res;
}
Scanline;

void wrend(const Scanline scanline, const Wall wall, const Hit hit);
void frend(const Scanline scanline, const Wall wall, const Traceline traceline, char** const floring);
void crend(const Scanline scanline, const Wall wall, const Traceline traceline, char** const ceiling);
void srend(const Scanline scanline, const Wall wall);
