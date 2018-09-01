#pragma once

#include "Party.h"
#include "Input.h"
#include "Point.h"

typedef struct
{
    // Panning pixels.
    int px;
    int py;

    // x and y tile selected. Stays -1 if nothing was selected.
    Point where;

    // Tile width and height pixels.
    int w;
    int h;

    // The selected tile to be placed. Defaults to empty space.
    int selected;

    // Overview party display, either walling, floring, or ceiling.
    Party party;

    // Scroll velocity with WASD keys.
    Point velocity;
    float acceleration;
    float speed;

    // The wheel is used for scrolling through all available tiles.
    int wheel;
}
Overview;

Overview xzov(void);

Overview xinit(void);

Overview xupdate(Overview, const Input, const int xres);

Overview xbackpan(Overview ov, const Point where, const int xres, const int yres);
