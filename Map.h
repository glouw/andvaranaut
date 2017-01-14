#pragma once

#include "Point.h"

#include <stdint.h>

typedef struct map
{
    int ysz;
    int xsz;
    uint8_t** ceiling;
    uint8_t** walling;
    uint8_t** floring;
}
Map;

Map
Map_Load(const char* const path);

void
Map_Unload(Map map);

Point
Map_Cast(const Point where, const double radians, const Map map);

bool
Map_In(const Point point, const Map map);

bool
Map_Out(const Point point, const Map map);
