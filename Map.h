#pragma once

#include "Point.h"

typedef struct map
{
    int ysz;
    int xsz;
    uint8_t** ceiling;
    uint8_t** walling;
    uint8_t** floring;
    Point where;
    double theta;
}
Map;

Map
Map_Load(const char* const path);

Point
Map_Cast(const Point where, const double radians, const Map map);

bool
Map_In(const Point point, const Map map);

bool
Map_Out(const Point point, const Map map);

void
Map_Unload(const Map map);
