#pragma once

#include "Points.h"
#include "Timer.h"
#include "Overview.h"
#include "Sheer.h"
#include "Theme.h"

typedef struct
{
    char** ceiling;
    char** walling;
    char** floring;

    // Map width and height.
    int rows;
    int cols;

    // Trapdoor points for moving up and down map floors.
    Points trapdoors;

    // Each point of interest will have its own themed room.
    // That is, the number of themes is equal to the number of interest points.
    Points interests;
    Theme* themes;

    // These sheers form an optical illusion with the middle and upper ceiling heights.
    // For example, a larger upper sheer will extend the ceiling depth, and a larger
    // middle sheer will extend the height of the walls.
    Sheer middle;
    Sheer upper;

    // Each room occupies at most (grid * grid) area of the map.
    int grid;
}
Map;

Map xzmap();

Map xmgen(const int rows, const int cols, const Points trapdoors, const Points interests, const int grid);

int xmisportal(char** block, const Point where);

void xmedit(const Map, const Overview);

int xmout(const Map, const Point where);

void xmprint(char** block, const int rows, const int cols);

int xmcarvable(const Map, const int x, const int y);

void xmroom(const Map map, const Point where, const int w, const int h, const Party);

void xmpole(const Map, const Point where, const int ascii);

void xmtrapdoors(const Map, const Points trapdoors, const Party);

void xmcorridor(const Map, const Point a, const Point b);

void xmthemett(const Map, const Point where, const Timer);
