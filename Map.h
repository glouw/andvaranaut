#pragma once

#include "Point.h"
#include "Overview.h"
#include "Trapdoor.h"

typedef struct
{
    // The ASCII value of either the ceiling, walling, or flooring
    // will determine which surface to lookup for the renderer.
    char** ceiling;
    char** walling;
    char** floring;
    int rows;
    int cols;
    Trapdoor trapdoor;
}
Map;

Map xmgen(const int rows, const int cols, const Trapdoor);

void xclose(const Map);

int xisportal(const Map, const Point where);

void xedit(const Map, const Overview);

int xout(const Map, const Point where);

void xmdump(const Map map);
