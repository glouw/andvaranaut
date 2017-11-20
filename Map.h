#pragma once

#include "Point.h"
#include "Overview.h"

#include <stdbool.h>

typedef struct
{
    char** ceiling;
    char** walling;
    char** floring;
    int rows;
    int cols;
}
Map;

Map xopen(const int level);

void xclose(const Map map);

Map xreopen(const Map map, const int level);

bool xisportal(const Map map, const Point where);

void xedit(const Map map, const Overview ov);

bool xout(const Map map, const float x, const float y);
