#pragma once

#include "Line.h"
#include "Sdl.h"
#include "Hero.h"
#include "Flow.h"
#include "Map.h"
#include "Vram.h"

typedef struct
{
    int a;
    int b;
    Point* zbuff;
    Line camera;
    Vram vram;
    Sdl sdl;
    Hero hero;
    Flow current;
    Flow clouds;
    Map map;
}
Bundle;

int xbraster(void* const bundle);
