#pragma once

#include "Points.h"
#include "Overview.h"

typedef struct
{
    // The ASCII value of either the ceiling, walling, or flooring
    // will determine which surface to lookup for the renderer.
    char** ceiling;
    char** walling;
    char** floring;
    int rows;
    int cols;
    Points trapdoors;
}
Map;

Map xmgen(const int rows, const int cols, const Points);

void xclose(const Map);

int xisportal(const Map, const Point where);

void xedit(const Map, const Overview);

int xout(const Map, const Point where);

void xmprint(char** block, const int rows, const int cols);

void xmdump(const Map);

int xcarvable(const Map map, const int x, const int y);

void xroom(const Map map, const Point where, const int w, const int h, const Party);

void xtrapdoors(const Map, const Points trapdoors, const Party);

void xcorridor(const Map, const Point a, const Point b);
