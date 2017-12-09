#pragma once

#include "Line.h"
#include "Display.h"
#include "Sdl.h"
#include "Hero.h"
#include "Current.h"
#include "Map.h"

typedef struct
{
    int a;
    int b;
    Point* zbuff;
    Line camera;
    Display display;
    Sdl sdl;
    Hero hero;
    Current current;
    Map map;
}
Bundle;

// Bundle rasterer. Renders scanlines <a> to <b>. Thread safe.
int xbraster(void* const bundle);
