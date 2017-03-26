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

/* Wall renderer */
void wrend(const Scanline scanline, const Wall wall, const Hit hit);
/* Floor renderer */
void frend(
    const Scanline scanline,
    const Wall wall,
    Point wheres[],
    const Traceline traceline,
    char** const floring,
    const float party[]);
/* Ceiling renderer - requires floor renderer */
void crend(
    const Scanline scanline,
    const Wall wall,
    const Point wheres[],
    char** const ceiling);
/* Sky renderer */
void srend(const Scanline scanline, const float theta);
/* Blackness renderer */
void brend(const Scanline scanline);
