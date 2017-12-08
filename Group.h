#pragma once

#include "Line.h"
#include "Display.h"
#include "Sdl.h"
#include "Hero.h"
#include "Current.h"
#include "Map.h"

// Common objects used across rendering threads.

typedef struct
{
    Point* zbuff;
    Line camera;
    Display display;
    Sdl sdl;
    Hero hero;
    Current current;
    Map map;
}
Group;
