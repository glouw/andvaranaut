#pragma once

#include "Hit.h"
#include "Wall.h"
#include "Gpu.h"
#include "Display.h" 
#include "Blocks.h"
#include "Hero.h"
#include "Line.h"
#include "Traceline.h"

typedef struct
{
    Gpu gpu;
    Display display;
    int yy;
    int res;
}
Frame;

void wrend(const Frame frame, const Wall wall, const Hit hit);
void frend(const Frame frame, const Wall wall, const Traceline traceline, char** const floring);
void crend(const Frame frame, const Wall wall, const Traceline traceline, char** const ceiling);
void srend(const Frame frame, const Wall wall);
void render(const Hero hero, const Blocks blocks, const int res, const Gpu gpu);
