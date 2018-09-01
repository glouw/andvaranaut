#pragma once

#include "Line.h"
#include "Sdl.h"
#include "Hero.h"
#include "Flow.h"
#include "Map.h"

typedef struct
{
    int a;
    int b;
    Point* zbuff;
    Line camera;
    uint32_t* pixels;
    int width;
    Sdl sdl;
    Hero hero;
    Flow current;
    Flow clouds;
    Map map;
}
Bundle;

int xbraster(void* const bundle);
