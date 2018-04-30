#pragma once

#include "Points.h"
#include "Overview.h"
#include "Sheer.h"

typedef struct
{
    char** ceiling;
    char** walling;
    char** floring;
    int rows;
    int cols;
    // Trapdoor points for moving up and down map floors.
    Points trapdoors;
    // Points of interests are used to generate themed rooms.
    Points interests;
    Sheer middle;
    Sheer upper;
}
Map;

Map xzmap();

Map xmgen(const int rows, const int cols, const Points trapdoors, const Points interests);

void xmclose(const Map);

int xmisportal(char** block, const Point where);

void xmedit(const Map, const Overview);

int xmout(const Map, const Point where);

void xmprint(char** block, const int rows, const int cols);

void xmdump(const Map);

int xmcarvable(const Map, const int x, const int y);

void xmroom(const Map map, const Point where, const int w, const int h, const Party);

void xmtrapdoors(const Map, const Points trapdoors, const Party);

void xmcorridor(const Map, const Point a, const Point b);
